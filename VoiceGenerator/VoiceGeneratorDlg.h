
// VoiceGeneratorDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>
#include <set>
#include "afxcmn.h"

// CVoiceGeneratorDlg �Ի���
class CVoiceGeneratorDlg : public CDialogEx
{
// ����
public:
	CVoiceGeneratorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VOICEGENERATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	BOOL m_bIsPlaying;
	std::vector<std::pair<CString, CString>> m_vecPairVoicer;
	std::set<TCHAR> m_setSplitMark;
	CProgressCtrl m_ctrProgress;

	// ���ɵ���Ϣӳ�亯��
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
