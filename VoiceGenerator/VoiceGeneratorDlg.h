
// VoiceGeneratorDlg.h : 头文件
//

#pragma once
#include <vector>
#include <set>
#include "afxcmn.h"

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
	std::set<TCHAR> m_setSplitMark;
	CProgressCtrl m_ctrProgress;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	BOOL GenerateAudio(std::vector<CString> vecStrContent, CString strVoiceName = _T("xiaoyan"), DWORD dwSpeed = 50, DWORD dwPitch = 50);
	void GenerateOneContent(CString strAudioPath, CString strContent, CString strVoiceName, DWORD dwSpeed, DWORD dwPitch, std::vector<DWORD> &vecDwGap, std::vector<CString> &vecFileList);
	void ReadConfig();
	void InitialLayout();
	CWnd * GetDlgSafeItem(int nID);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnGenerate();
	afx_msg void OnBnClickedBtnListen();
	afx_msg void OnBnClickedBtnOpenPath();
	afx_msg void OnBnClickedBtnOpenfile();
};
