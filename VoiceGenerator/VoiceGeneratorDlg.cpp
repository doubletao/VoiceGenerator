
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
}

BEGIN_MESSAGE_MAP(CVoiceGeneratorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_GENERATE, &CVoiceGeneratorDlg::OnBnClickedBtnGenerate)
	ON_BN_CLICKED(IDC_BTN_LISTEN, &CVoiceGeneratorDlg::OnBnClickedBtnListen)
	ON_BN_CLICKED(IDC_BTN_OPEN_PATH, &CVoiceGeneratorDlg::OnBnClickedBtnOpenPath)
END_MESSAGE_MAP()


// CVoiceGeneratorDlg ��Ϣ�������

void CVoiceGeneratorDlg::GenerateAudio(std::vector<CString> vecStrContent)
{
	CString strBinPath = SYCGlobalFunction::GetCurPath();
	CString strAudioPath = strBinPath + _T("\\Audio\\");
	CString strErr;
	SYCGlobalFunction::MakeSureDirectoryExists(strAudioPath, strErr);
	std::vector<DWORD> vecDwGap;
	std::vector<CString> vecFileList;
	for (int i = 0; i < vecStrContent.size(); i++)
	{
		CString strContent = vecStrContent[i];
		CString strFilePath;
		strFilePath.Format(_T("%s%s.wav"), strAudioPath, strContent);
		if (!SYCGlobalFunction::IsFileExists(strFilePath))
		{
			XunFei_Voive_Excute(strFilePath, strContent);
		}
		vecDwGap.push_back(50);
		vecFileList.push_back(strFilePath);
	}
	if (vecFileList.size() > 0
		&& vecFileList.size() == vecDwGap.size())
	{
	}
	XunFei_Voive_CombineAudioFile(vecFileList, vecDwGap, 0, strAudioPath + _T("combined.wav"));
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



void CVoiceGeneratorDlg::OnBnClickedBtnGenerate()
{
	CEdit * pEditSrc = (CEdit *)GetDlgItem(IDC_EDIT_CONTENT);
	if (pEditSrc && ::IsWindow(pEditSrc->GetSafeHwnd()))
	{
		//�������ؼ��л�ȡ�ǿ���
		std::vector<CString> vecContent;
		std::set<TCHAR> setMark;
		setMark.insert(_T('\n'));
		CString strTmp;
		pEditSrc->GetWindowText(strTmp);
		strTmp.Replace(_T('\r'), _T(''));
		std::vector<CString> vecStrContent = SYCGlobalFunction::SplitCString(strTmp, setMark, FALSE);
		GenerateAudio(vecStrContent);
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
