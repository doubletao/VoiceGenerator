
// VoiceGenerator.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVoiceGeneratorApp:
// �йش����ʵ�֣������ VoiceGenerator.cpp
//

class CVoiceGeneratorApp : public CWinApp
{
public:
	CVoiceGeneratorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVoiceGeneratorApp theApp;