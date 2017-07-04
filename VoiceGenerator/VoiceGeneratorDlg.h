
// VoiceGeneratorDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>

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

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	void GenerateAudio(std::vector<CString> vecStrContent, CString strVoiceName = _T("xiaoyan"), DWORD dwSpeed = 50, DWORD dwPitch = 50);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnGenerate();
	afx_msg void OnBnClickedBtnListen();
	afx_msg void OnBnClickedBtnOpenPath();
};