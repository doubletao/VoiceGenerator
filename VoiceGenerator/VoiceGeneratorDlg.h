
// VoiceGeneratorDlg.h : 头文件
//

#pragma once
#include <vector>

// CVoiceGeneratorDlg 对话框
class CVoiceGeneratorDlg : public CDialogEx
{
// 构造
public:
	CVoiceGeneratorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VOICEGENERATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	BOOL m_bIsPlaying;
	std::vector<std::pair<CString, CString>> m_vecPairVoicer;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	BOOL GenerateAudio(std::vector<CString> vecStrContent, CString strVoiceName = _T("xiaoyan"), DWORD dwSpeed = 50, DWORD dwPitch = 50);
	void ReadConfig();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnGenerate();
	afx_msg void OnBnClickedBtnListen();
	afx_msg void OnBnClickedBtnOpenPath();
	afx_msg void OnBnClickedBtnOpenfile();
};
