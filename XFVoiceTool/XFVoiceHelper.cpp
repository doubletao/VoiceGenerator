#include "StdAfx.h"
#include "XFVoiceHelper.h"
#include "..\include\qtts.h"
#include "..\include\msp_cmn.h"
#include "..\include\msp_errors.h"
#include "SYGlobalFunction.h"

#ifdef _WIN64
#pragma comment(lib,"msc_x64.lib")//x64
#else
#pragma comment(lib,"msc.lib")//x86
#endif

/* wav��Ƶͷ����ʽ */
typedef struct _wave_pcm_hdr
{
	char            riff[4];                // = "RIFF"
	int				size_8;                 // = FileSize - 8
	char            wave[4];                // = "WAVE"
	char            fmt[4];                 // = "fmt "
	int				fmt_size;				// = ��һ���ṹ��Ĵ�С : 16

	short int       format_tag;             // = PCM : 1
	short int       channels;               // = ͨ���� : 1
	int				samples_per_sec;        // = ������ : 8000 | 6000 | 11025 | 16000
	int				avg_bytes_per_sec;      // = ÿ���ֽ��� : samples_per_sec * bits_per_sample / 8
	short int       block_align;            // = ÿ�������ֽ��� : wBitsPerSample / 8
	short int       bits_per_sample;        // = ����������: 8 | 16

	char            data[4];                // = "data";
	int				data_size;              // = �����ݳ��� : FileSize - 44 
} wave_pcm_hdr;

/* Ĭ��wav��Ƶͷ������ */
wave_pcm_hdr default_wav_hdr = 
{
	{ 'R', 'I', 'F', 'F' },
	0,
	{'W', 'A', 'V', 'E'},
	{'f', 'm', 't', ' '},
	16,
	1,
	1,
	16000,
	32000,
	2,
	16,
	{'d', 'a', 't', 'a'},
	0  
};

XFVoiceHelper::XFVoiceHelper()
	: m_bLogined(FALSE)
{
	int ret = MSPLogin(NULL, NULL, "appid = 589c478e, work_dir = .");
	if (MSP_SUCCESS != ret)
	{
		TRACE(_T("xunfei voice:MSPLogin failed,err=%d\n"), ret);
		m_bLogined = FALSE;
	}
	else
	{
		m_bLogined = TRUE;
	}
}

XFVoiceHelper::~XFVoiceHelper(void)
{
	int ret = MSPLogout(); //�˳���¼
	if (MSP_SUCCESS != ret)
	{
		TRACE(_T("xunfei voice:MSPLogout failed,err=%d\n"), ret);
	}
	else
	{
		m_bLogined = TRUE;
	}
}

DWORD XFVoiceHelper::Excute(CString strFileName, CString strContent, CString strVoiceName /*= _T("xiaoyan")*/, DWORD dwSpeed /*= 50*/, DWORD dwPitch /*= 50*/, DWORD dwNumMode /*= 3*/)
{
	DWORD dwRet = 0;
	int nErrCode = -1;
	if (!m_bLogined)
	{
		TRACE(_T("XFVoiceHelper:Not Logined\n"));
		return dwRet;
	}
	//���ɲ���
	CString strParam;
	strParam.Format(_T("voice_name = %s, text_encoding = gb2312, sample_rate = 1600, speed = %d, volume = 50, pitch = %d, rdn = %d"),
		strVoiceName, dwSpeed, dwPitch, dwNumMode);
	std::string session_begin_params = SYCGlobalFunction::ConverCStringToStdString(strParam).c_str();
	//���ò����������Ự
	const char * sessionID = QTTSSessionBegin(session_begin_params.c_str(), &nErrCode);
	if (MSP_SUCCESS != nErrCode)
	{
		TRACE(_T("xunfei voice:QTTSSessionBegin failed,err=%d\n"), nErrCode);
		return dwRet;
	}
	if (sessionID)
	{
		//���ı�
		std::string szContent = SYCGlobalFunction::ConverCStringToStdString(strContent, 936);
		nErrCode = QTTSTextPut(sessionID, szContent.c_str(), szContent.length(), NULL);
		if (MSP_SUCCESS != nErrCode)
		{
			TRACE(_T("QTTSTextPut failed, error code: %d.\n"), nErrCode);
			QTTSSessionEnd(sessionID, "TextPutError");
			return dwRet;
		}
		//open�ļ���д���ļ�ͷ,fopen_s��������·����ʧ��
		wave_pcm_hdr wav_hdr      = default_wav_hdr;
		FILE * fp = NULL;
		nErrCode = _wfopen_s(&fp, strFileName, _T("wb+"));
		if (NULL == fp || nErrCode != 0)
		{
			TRACE(_T("XFVoiceHelper:Open File: %s failed, err=%d\n"), strFileName, nErrCode);
			return dwRet;
		}

		//д��Ƶͷ
		fwrite(&wav_hdr, sizeof(wav_hdr) ,1, fp);

		//ѭ����buffer��д���ļ�
		int synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;
		unsigned int audio_len = 0;
		while (1)
		{
			/* ��ȡ�ϳ���Ƶ */
			const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &nErrCode);
			if (MSP_SUCCESS != nErrCode)
			{
				TRACE(_T("QTTSTextPut failed, error code: %d.\n"), nErrCode);
				fclose(fp);
				nErrCode = QTTSSessionEnd(sessionID, "Normal");
				if (MSP_SUCCESS != nErrCode)
				{
					TRACE(_T("QTTSSessionEnd failed, error code: %d.\n"), nErrCode);
				}
				return dwRet;
			}
			if (NULL != data)
			{
				fwrite(data, audio_len, 1, fp);
				wav_hdr.data_size += audio_len; //����data_size��С
			}
			if (MSP_TTS_FLAG_DATA_END == synth_status)
				break;
			Sleep(50); //��ֹƵ��ռ��CPU
		}//�ϳ�״̬synth_statusȡֵ����ġ�Ѷ��������API�ĵ���
		/* ����wav�ļ�ͷ���ݵĴ�С */
		wav_hdr.size_8 += wav_hdr.data_size + (sizeof(wav_hdr) - 8);

		/* ��������������д���ļ�ͷ��,��Ƶ�ļ�Ϊwav��ʽ */
		fseek(fp, 4, 0);
		fwrite(&wav_hdr.size_8,sizeof(wav_hdr.size_8), 1, fp); //д��size_8��ֵ
		fseek(fp, 40, 0); //���ļ�ָ��ƫ�Ƶ��洢data_sizeֵ��λ��
		fwrite(&wav_hdr.data_size,sizeof(wav_hdr.data_size), 1, fp); //д��data_size��ֵ

		//д��ر��ļ�
		fclose(fp);

		//������ϣ��˳��Ự
		nErrCode = QTTSSessionEnd(sessionID, "Normal");
		if (MSP_SUCCESS != nErrCode)
		{
			TRACE(_T("QTTSSessionEnd failed, error code: %d.\n"), nErrCode);
			return dwRet;
		}
		dwRet = (float)wav_hdr.data_size * 1000 / wav_hdr.avg_bytes_per_sec;
	}
	return dwRet;
}

DWORD XFVoiceHelper::CombineAudioFile(const std::vector<CString> & vecFileList, std::vector<DWORD> vecAudioGap, DWORD dwWholeTime, CString strRetFilePath)
{
	BOOL bRet = FALSE;
	if (vecFileList.size() == 1 && vecFileList.at(0).Compare(strRetFilePath) == 0)
	{
		// �ļ��ظ�
		return TRUE;
	}

	//��ʱ��תΪ����
	dwWholeTime *= 1000;
	//�ļ��б�sizeӦ�úͼ����Ϣһ�£��������
	if (vecFileList.size() == vecAudioGap.size())
	{
		int nSize = vecFileList.size();
		//��֤�ļ��б����ļ��Ƿ񶼴��ڲ�ͳ���ļ���ʱ��
		float fVoiceTime = 0;
		for (int i = 0; i < nSize; i++)
		{
			CString strFile = vecFileList[i];
			//�ж���ʽΪ�ļ���С����Ϊһ��wavͷ��С
			if(SYCGlobalFunction::GetFileSize(strFile) < sizeof(wave_pcm_hdr))
			{
				TRACE(_T("XFVoiceHelper::CombineAudioFile:input file err(%s)\n"), strFile);
				return bRet;
			}
			wave_pcm_hdr wav_hdrR = {0};
			FILE * fpRead = NULL;
			int nErrCode = _wfopen_s(&fpRead, strFile, _T("rb"));
			if (NULL == fpRead || nErrCode != 0)
			{
				TRACE(_T("XFVoiceHelper::CombineAudioFile:open file err(%s)\n"), strFile, nErrCode);
				return bRet;
			}
			//����Ƶͷ
			fread(&wav_hdrR, 1, sizeof(wav_hdrR), fpRead);
			fVoiceTime += (double)wav_hdrR.data_size / wav_hdrR.avg_bytes_per_sec;
			//����ص�
			fclose(fpRead);
		}
		//��ʱ��תΪ����
		fVoiceTime *= 1000;
		//��Ҫ����ʱ����ȥ������ʱ�����õ���϶��ʱ��
		float fGapScale = 1.0f;
		if (dwWholeTime > 0)
		{
			if (dwWholeTime > fVoiceTime)
			{
				float fWholeGap = dwWholeTime - fVoiceTime;
				DWORD dwCfgGap = 0;
				//���ݿ�϶��ʱ�����趨��϶ʱ��ı�ֵ�����϶ʱ������ű�
				for (int i = 0; i < vecAudioGap.size(); i++)
				{
					dwCfgGap += vecAudioGap[i];
				}
				fGapScale = fWholeGap / dwCfgGap;
			}
			else
			{
				fGapScale = 0.0f;
			}
		}
		for (int i = 0; i < vecAudioGap.size(); i++)
		{
			vecAudioGap[i] *= fGapScale;
		}
		//open����ļ���д���ļ�ͷ,fopen_s��������·����ʧ��
		wave_pcm_hdr wav_hdrW      = default_wav_hdr;
		FILE * fpWrite = NULL;
		int nErrCode = _wfopen_s(&fpWrite, strRetFilePath, _T("wb"));
		if (NULL == fpWrite || nErrCode != 0)
		{
			TRACE(_T("XFVoiceHelper::CombineAudioFile:open file err(%s)\n"), strRetFilePath, nErrCode);
			return bRet;
		}
		//д��Ƶͷ
		fwrite(&wav_hdrW, sizeof(wav_hdrW) ,1, fpWrite);
		const DWORD dwBufferSize = 1024 * 1024;
		char * pBuffer = new char[dwBufferSize];
		//ѭ�����ļ���������Ƶ��д��
		for (int i = 0; i < nSize; i++)
		{
			CString strFile = vecFileList[i];
			wave_pcm_hdr wav_hdrR = {0};
			FILE * fpRead = NULL;
			int nErrCode = _wfopen_s(&fpRead, strFile, _T("rb"));
			if (NULL == fpRead || nErrCode != 0)
			{
				TRACE(_T("XFVoiceHelper::CombineAudioFile:open file err(%s)\n"), strFile, nErrCode);
				return bRet;
			}
			//����Ƶͷ
			fread(&wav_hdrR, 1, sizeof(wav_hdrR), fpRead);
			wav_hdrW.data_size += wav_hdrR.data_size;
			//������Ƶ��д��
			memset(pBuffer, 0, dwBufferSize);
			int nRemain = wav_hdrR.data_size;
			while(nRemain > 0)
			{
				int nAlready = fread(pBuffer, 1, dwBufferSize, fpRead);
				fwrite(pBuffer, nAlready, 1, fpWrite);
				nRemain -= nAlready;
			}
			//д����
			memset(pBuffer, 0, dwBufferSize);
			DWORD dwMuteSize = (float)vecAudioGap[i] / 1000 * wav_hdrR.avg_bytes_per_sec;
			fwrite(pBuffer, dwMuteSize, 1, fpWrite);
			wav_hdrW.data_size += dwMuteSize;
			//д��ص�
			fclose(fpRead);
		}
		delete[] pBuffer;
		/* ����wav�ļ�ͷ���ݵĴ�С */
		wav_hdrW.size_8 += wav_hdrW.data_size + (sizeof(wav_hdrW) - 8);
		/* ��������������д���ļ�ͷ��,��Ƶ�ļ�Ϊwav��ʽ */
		fseek(fpWrite, 4, 0);
		fwrite(&wav_hdrW.size_8,sizeof(wav_hdrW.size_8), 1, fpWrite); //д��size_8��ֵ
		fseek(fpWrite, 40, 0); //���ļ�ָ��ƫ�Ƶ��洢data_sizeֵ��λ��
		fwrite(&wav_hdrW.data_size,sizeof(wav_hdrW.data_size), 1, fpWrite); //д��data_size��ֵ
		//д��ر��ļ�
		fclose(fpWrite);
		bRet = TRUE;
	}
	else
	{
		ASSERT(FALSE);
		TRACE(_T("XFVoiceHelper::CombineAudioFile:param count err\n"));
	}
	return bRet;
}
