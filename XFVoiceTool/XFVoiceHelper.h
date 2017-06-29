/*
 * Ѷ�����Ժϳɹ�����
 * ͨ�������߿����Զ���һ�����ֺϳ�Ϊ��Ƶ�ļ�����ͨ���ص����������ϳɽ���
 * 
 */
#pragma once
#include <vector>

class XFVoiceHelper
{
private:
	BOOL m_bLogined;//�Ƿ��ѵ�¼
public:
	XFVoiceHelper();
	~XFVoiceHelper(void);

	/* strFileName:�������Ƶ�ļ�����ȫ·����
	 * strContent:��Ҫ�ϳ�Ϊ��Ƶ���ı����ݣ��ı����ܹ���������������Ҫ��ε��ã��������ڲ���������
	 * m_strVoiceName:����Ա��Ů��xiaoyan���У�xiaoyu��
	 * dwSpeed:����
	 * dwPitch:����
	 * dwNumMode:���ַ�����ʽ����2017��10�£�=1��������ǧһ��������ʮ�£�=2���������������������£�=3��������һ����ʮ�£�
	 * ����ֵ:���ɵ���Ƶ���ȣ���λ���룩
	 */
	DWORD Excute(CString strFileName, CString strContent, CString strVoiceName = _T("xiaoyan"), DWORD dwSpeed = 50, DWORD dwPitch = 50, DWORD dwNumMode = 3);
	//���б��е���Ƶ�ļ�ƴ�ӳ�һ��������Ƶ��������ָ�����ȵľ���
	//ƴ����ʽΪ----�ļ�0�����0���ļ�1�����1��......�ļ�n�����n
	//�ļ���Ӧ���ͼ����һ��
	//ע�⣬�����λΪ���룬��ʱ����λΪ��
	static DWORD CombineAudioFile(const std::vector<CString> & vecFileList, std::vector<DWORD> vecAudioGap, DWORD dwWholeTime, CString strRetFilePath);
};

