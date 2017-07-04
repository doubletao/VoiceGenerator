
// VoiceGeneratorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VoiceGenerator.h"
#include "VoiceGeneratorDlg.h"
#include "afxdialogex.h"
#include "..\include\XFVoiceTool.h"
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const static int MAX_SECTION = 6000;//�ε���󳤶�


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVoiceGeneratorDlg �Ի���




CVoiceGeneratorDlg::CVoiceGeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVoiceGeneratorDlg::IDD, pParent)
	, m_bIsPlaying(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVoiceGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_GENERATE, m_ctrProgress);
}

BEGIN_MESSAGE_MAP(CVoiceGeneratorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_GENERATE, &CVoiceGeneratorDlg::OnBnClickedBtnGenerate)
	ON_BN_CLICKED(IDC_BTN_LISTEN, &CVoiceGeneratorDlg::OnBnClickedBtnListen)
	ON_BN_CLICKED(IDC_BTN_OPEN_PATH, &CVoiceGeneratorDlg::OnBnClickedBtnOpenPath)
	ON_BN_CLICKED(IDC_BTN_OPENFILE, &CVoiceGeneratorDlg::OnBnClickedBtnOpenfile)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CVoiceGeneratorDlg ��Ϣ�������

const int MAX_XUNFEI_WORDS = 50;//Ѷ�ɵ������װ���ַ���
BOOL CVoiceGeneratorDlg::GenerateAudio(std::vector<CString> vecStrContent, CString strVoiceName/* = _T("xiaoyan")*/, DWORD dwSpeed/* = 50*/, DWORD dwPitch/* = 50*/)
{
	m_ctrProgress.SetPos(0);
	BOOL bRet = FALSE;
	CString strBinPath = SYCGlobalFunction::GetCurPath();
	CString strAudioPath = strBinPath + _T("\\Audio\\");
	CString strErr;
	SYCGlobalFunction::MakeSureDirectoryExists(strAudioPath, strErr);
	std::vector<DWORD> vecDwGap;
	std::vector<CString> vecFileList;
	for (int i = 0; i < vecStrContent.size(); i++)
	{
		CString strContent = vecStrContent[i];
		//���������������з�
		if (strContent.GetLength() > MAX_XUNFEI_WORDS)
		{
			CString strSplit;
			while(strContent.GetLength() > MAX_XUNFEI_WORDS)
			{
				strSplit = strContent.Left(MAX_XUNFEI_WORDS);
				strContent = strContent.Mid(MAX_XUNFEI_WORDS);
				GenerateOneContent(strAudioPath, strSplit, strVoiceName, dwSpeed, dwPitch, vecDwGap, vecFileList);
			}
			if (strContent.GetLength() > 0)
			{
				GenerateOneContent(strAudioPath, strContent, strVoiceName, dwSpeed, dwPitch, vecDwGap, vecFileList);
			}
		}
		else
		{
			GenerateOneContent(strAudioPath, strContent, strVoiceName, dwSpeed, dwPitch, vecDwGap, vecFileList);
		}
		m_ctrProgress.SetPos(i * 100.0 / vecStrContent.size());
	}
	m_ctrProgress.SetPos(100);
	CString strRetFile = strAudioPath + _T("combined.wav");
	if (SYCGlobalFunction::IsFileExists(strRetFile))
	{
		::DeleteFile(strRetFile);
	}
	if (vecFileList.size() > 0
		&& vecFileList.size() == vecDwGap.size())
	{
		XunFei_Voive_CombineAudioFile(vecFileList, vecDwGap, 0, strRetFile);
	}
	if (SYCGlobalFunction::IsFileExists(strRetFile))
	{
		bRet = TRUE;
	}
	return bRet;
}

void CVoiceGeneratorDlg::GenerateOneContent(CString strAudioPath, CString strContent, CString strVoiceName, DWORD dwSpeed, DWORD dwPitch, std::vector<DWORD> &vecDwGap, std::vector<CString> &vecFileList)
{
	if (!strContent.IsEmpty())
	{
		CString strFilePath;
		strFilePath.Format(_T("%s%s.wav"), strAudioPath, SYCGlobalFunction::GetNewGUID());
		if (SYCGlobalFunction::IsFileExists(strFilePath))
		{
			::DeleteFile(strFilePath);
		}
		XunFei_Voive_Excute(strFilePath, strContent, strVoiceName, dwSpeed, dwPitch);
		if (SYCGlobalFunction::IsFileExists(strFilePath))
		{
			vecDwGap.push_back(50);
			vecFileList.push_back(strFilePath);
		}
	}
}

void CVoiceGeneratorDlg::ReadConfig()
{
	m_vecPairVoicer.clear();

	CString strConfigPath = SYCGlobalFunction::GetCurPath() + _T("\\Ѷ�������б�.ini");

	CString strKey;
	TCHAR tmpBuf[MAX_PATH] = {0};
	TCHAR chKey[MAX_SECTION]={0};
	TCHAR szKey[1024] = {0};

	{
		CString strSection = _T("�������б�");
		DWORD dwKeyBufferSize = GetPrivateProfileSection(strSection, chKey, MAX_SECTION, strConfigPath);
		for (int n = 0, k = 0; n < dwKeyBufferSize; n++)
		{
			if (chKey[n] == 0)
			{
				szKey[k] = 0;
				strKey = szKey;

				CString strKeyName = strKey.Left(strKey.Find('='));
				CString strKeyValue = strKey.Mid(strKey.Find('=') + 1);

				m_vecPairVoicer.push_back(std::make_pair(strKeyName, strKeyValue));

				ZeroMemory(szKey, 1024);
				k = 0;
			}
			else
			{
				szKey[k] = chKey[n];
				k++;
			}
		}
	}

	{
		CString strSection = _T("�ָ����б�");
		DWORD dwKeyBufferSize = GetPrivateProfileSection(strSection, chKey, MAX_SECTION, strConfigPath);
		for (int n = 0, k = 0; n < dwKeyBufferSize; n++)
		{
			if (chKey[n] == 0)
			{
				szKey[k] = 0;
				strKey = szKey;

				CString strKeyName = strKey.Left(strKey.Find('='));
				CString strKeyValue = strKey.Mid(strKey.Find('=') + 1);

				if (strKeyValue.GetLength() > 0)
				{
					m_setSplitMark.insert(strKeyValue[0]);
				}

				ZeroMemory(szKey, 1024);
				k = 0;
			}
			else
			{
				szKey[k] = chKey[n];
				k++;
			}
		}
	}
}

void CVoiceGeneratorDlg::InitialLayout()
{
	CWnd * pStcXunfei = GetDlgSafeItem(IDC_STATIC1);
	CWnd * pStcVoice = GetDlgSafeItem(IDC_STATIC2);
	CWnd * pCombVoicer = GetDlgSafeItem(IDC_COMBO_VOICER);
	CWnd * pStcSpeed = GetDlgSafeItem(IDC_STATIC3);
	CWnd * pEditSpeed = GetDlgSafeItem(IDC_EDIT_SPEED);
	CWnd * pStcPitch = GetDlgSafeItem(IDC_STATIC4);
	CWnd * pEditPitch = GetDlgSafeItem(IDC_EDIT_PITCH);
	CWnd * pEditContent = GetDlgSafeItem(IDC_EDIT_CONTENT);
	CWnd * pBtnLoadFile = GetDlgSafeItem(IDC_BTN_OPENFILE);
	CWnd * pBtnOpenPath = GetDlgSafeItem(IDC_BTN_OPEN_PATH);
	CWnd * pBtnGenerate = GetDlgSafeItem(IDC_BTN_GENERATE);
	CWnd * pBtnListen = GetDlgSafeItem(IDC_BTN_LISTEN);
	CWnd * pProgress = GetDlgSafeItem(IDC_PROGRESS_GENERATE);
	if (pStcXunfei
		&& pStcVoice
		&& pCombVoicer
		&& pStcSpeed
		&& pEditSpeed
		&& pStcPitch
		&& pEditPitch
		&& pEditContent
		&& pBtnLoadFile
		&& pBtnOpenPath
		&& pBtnGenerate
		&& pBtnListen
		&& pProgress)
	{
		CRect rtClient;
		GetClientRect(rtClient);
		int nWholeWidth = rtClient.Width();
		int nWholeHeight = rtClient.Height();
		CRect rtTmp;
		pStcXunfei->GetWindowRect(rtTmp);
		int nStcXunFeiWidth = rtTmp.Width();
		pStcVoice->GetWindowRect(rtTmp);
		int nStcVoicerWidth = rtTmp.Width();
		pCombVoicer->GetWindowRect(rtTmp);
		int nCombVoicerWidth = rtTmp.Width();
		pStcSpeed->GetWindowRect(rtTmp);
		int nStcSpeedWidth = rtTmp.Width();
		pEditSpeed->GetWindowRect(rtTmp);
		int nEditSpeedWidth = rtTmp.Width();
		pStcPitch->GetWindowRect(rtTmp);
		int nStcPitchWidth = rtTmp.Width();
		pEditPitch->GetWindowRect(rtTmp);
		int nEditPitchWidth = rtTmp.Width();
		int nBtnWidth = 80;
		int nBtnHeight = 22;
		int nGap = 3;
		//��������
		int nFirstGap = (nWholeWidth - nStcXunFeiWidth - nStcVoicerWidth - nCombVoicerWidth - nStcSpeedWidth - nEditSpeedWidth - nStcPitchWidth - nEditPitchWidth - nGap * 5) / 3;
		pStcXunfei->MoveWindow(nGap, nGap, nStcXunFeiWidth, nBtnHeight);
		pStcVoice->MoveWindow(nGap + nStcXunFeiWidth + nFirstGap, nGap, nStcVoicerWidth, nBtnHeight);
		pCombVoicer->MoveWindow(nGap + nStcXunFeiWidth + nFirstGap + nStcVoicerWidth + nGap, nGap, nCombVoicerWidth, nBtnHeight);
		pStcSpeed->MoveWindow(nGap + nStcXunFeiWidth + nFirstGap + nStcVoicerWidth + nGap + nCombVoicerWidth + nFirstGap, nGap, nStcSpeedWidth, nBtnHeight);
		pEditSpeed->MoveWindow(nGap + nStcXunFeiWidth + nFirstGap + nStcVoicerWidth + nGap + nCombVoicerWidth + nFirstGap + nStcSpeedWidth + nGap, nGap, nEditSpeedWidth, nBtnHeight);
		pStcPitch->MoveWindow(nGap + nStcXunFeiWidth + nFirstGap + nStcVoicerWidth + nGap + nCombVoicerWidth + nFirstGap + nStcSpeedWidth + nGap + nEditSpeedWidth + nFirstGap, nGap, nStcPitchWidth, nBtnHeight);
		pEditPitch->MoveWindow(nGap + nStcXunFeiWidth + nFirstGap + nStcVoicerWidth + nGap + nCombVoicerWidth + nFirstGap + nStcSpeedWidth + nGap + nEditSpeedWidth + nFirstGap + nStcPitchWidth + nGap, nGap, nEditPitchWidth, nBtnHeight);
		//�м�
		pEditContent->MoveWindow(nGap, nGap + nBtnHeight + nGap, nWholeWidth - nGap * 2, nWholeHeight - nGap * 4 - nBtnHeight * 2);
		//�ײ�����
		int nBottomGap = nWholeWidth - nBtnWidth * 4 - nGap * 4;
		pBtnLoadFile->MoveWindow(nGap, nWholeHeight - nGap - nBtnHeight, nBtnWidth, nBtnHeight);
		pBtnOpenPath->MoveWindow(nGap + nBtnWidth + nGap, nWholeHeight - nGap - nBtnHeight, nBtnWidth, nBtnHeight);
		pProgress->MoveWindow(nGap + nBtnWidth + nGap + nBtnWidth + nGap, nWholeHeight - nGap - nBtnHeight, nBottomGap - nGap, nBtnHeight);
		pBtnGenerate->MoveWindow(nGap + nBtnWidth + nGap + nBtnWidth + nBottomGap, nWholeHeight - nGap - nBtnHeight, nBtnWidth, nBtnHeight);
		pBtnListen->MoveWindow(nGap + nBtnWidth + nGap + nBtnWidth + nBottomGap + nBtnWidth + nGap, nWholeHeight - nGap - nBtnHeight, nBtnWidth, nBtnHeight);
	}
	Invalidate(FALSE);
}

CWnd * CVoiceGeneratorDlg::GetDlgSafeItem(int nID)
{
	CWnd * pWnd = GetDlgItem(nID);
	if (pWnd && ::IsWindow(pWnd->GetSafeHwnd()))
	{
		return pWnd;
	}
	return NULL;
}

BOOL CVoiceGeneratorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	ReadConfig();
	GetDlgItem(IDC_EDIT_SPEED)->SetWindowText(_T("50"));
	GetDlgItem(IDC_EDIT_PITCH)->SetWindowText(_T("50"));
	CComboBox * pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_VOICER);
	if (pCombo)
	{
		for (int i = 0; i < m_vecPairVoicer.size(); i++)
		{
			CString strVoicer = m_vecPairVoicer[i].second;
			pCombo->AddString(strVoicer);
		}
		pCombo->SetCurSel(0);
	}
	m_ctrProgress.SetRange(0, 100);
	InitialLayout();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVoiceGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVoiceGeneratorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVoiceGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVoiceGeneratorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	InitialLayout();
}

void CVoiceGeneratorDlg::OnDestroy()
{
	CString strBinPath = SYCGlobalFunction::GetCurPath();
	CString strAudioPath = strBinPath + _T("\\Audio");
	SYCGlobalFunction::DeleteFileOrPath(strAudioPath);
}

void CVoiceGeneratorDlg::OnBnClickedBtnGenerate()
{
	CWaitCursor waitCursor;
	CEdit * pEditSpeed = (CEdit *)GetDlgItem(IDC_EDIT_SPEED);
	CEdit * pEditPitch = (CEdit *)GetDlgItem(IDC_EDIT_PITCH);
	CComboBox * pComboVoicer = (CComboBox *)GetDlgItem(IDC_COMBO_VOICER);
	CEdit * pEditSrc = (CEdit *)GetDlgItem(IDC_EDIT_CONTENT);
	if (pEditSrc && ::IsWindow(pEditSrc->GetSafeHwnd())
		&& pEditSpeed && ::IsWindow(pEditSpeed->GetSafeHwnd())
		&& pEditPitch && ::IsWindow(pEditPitch->GetSafeHwnd())
		&& pComboVoicer && ::IsWindow(pComboVoicer->GetSafeHwnd())
		)
	{
		int nSel = pComboVoicer->GetCurSel();
		CString strVoicer = m_vecPairVoicer[nSel].first;
		CString strSpeed;
		pEditSpeed->GetWindowText(strSpeed);
		DWORD dwSpeed = SYCGlobalFunction::ConvertCStringToInt(strSpeed);
		CString strPitch;
		pEditPitch->GetWindowText(strPitch);
		DWORD dwPitch = SYCGlobalFunction::ConvertCStringToInt(strPitch);
		//�������ؼ��л�ȡ�ǿ���
		std::vector<CString> vecContent;
		CString strTmp;
		pEditSrc->GetWindowText(strTmp);
		std::vector<CString> vecStrContent = SYCGlobalFunction::SplitCString(strTmp, m_setSplitMark, FALSE);
		if(GenerateAudio(vecStrContent, strVoicer, dwSpeed, dwPitch))
		{
			AfxMessageBox(_T("���ɳɹ�"));
		}
	}
}


void CVoiceGeneratorDlg::OnBnClickedBtnListen()
{
	if (m_bIsPlaying)
	{
		PlaySound(NULL, NULL, NULL);
		m_bIsPlaying = FALSE;
	}
	else
	{
		CString strFile = SYCGlobalFunction::GetCurPath() + _T("\\Audio\\combined.wav");
		if (SYCGlobalFunction::IsFileExists(strFile))
		{
			PlaySound(strFile, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			m_bIsPlaying = TRUE;
		}
	}
}


void CVoiceGeneratorDlg::OnBnClickedBtnOpenPath()
{
	CString strPath = SYCGlobalFunction::GetCurPath() + _T("\\Audio\\");
	CString strErr;
	SYCGlobalFunction::MakeSureDirectoryExists(strPath, strErr);
	ShellExecute(NULL, _T("open"), strPath, NULL, strPath, SW_SHOWNORMAL);
}


void CVoiceGeneratorDlg::OnBnClickedBtnOpenfile()
{
	CFileDialog dlg(TRUE, _T("*.txt"), NULL, OFN_FILEMUSTEXIST, _T("*.txt|*.txt||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		CEdit * pEditSrc = (CEdit *)GetDlgItem(IDC_EDIT_CONTENT);
		if (pEditSrc && ::IsWindow(pEditSrc->GetSafeHwnd()))
		{
			pEditSrc->SetWindowText(_T(""));
			CString strFile = dlg.GetPathName();
			TCHAR* old_locale = _tcsdup( _tsetlocale(LC_CTYPE,NULL) );
			_tsetlocale( LC_CTYPE, _T("chs"));
			CStdioFile file;
			if (file.Open(strFile, CFile::modeRead))
			{
				CString strBuffer;
				while(file.ReadString(strBuffer))
				{
					int nLength = pEditSrc->SendMessage(WM_GETTEXTLENGTH);
					pEditSrc->SetSel(nLength,  nLength);
					pEditSrc->ReplaceSel(strBuffer + _T("\r\n"));
				}
				file.Close();
			}
			_tsetlocale( LC_CTYPE, old_locale );
			free( old_locale );
		}
	}
}
