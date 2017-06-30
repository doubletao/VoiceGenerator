// XFVoiceTool.h : XFVoiceTool DLL ����ͷ�ļ�
//

#pragma once

#ifdef _DEBUG
#	pragma comment(lib, "XFVoiceToolD.lib")
#else
#	pragma comment(lib, "XFVoiceTool.lib")
#endif

#ifdef XFVOICE_TOOL_EXPORTS
#define XFVOICE_TOOL_API __declspec(dllexport)
#else
#define XFVOICE_TOOL_API __declspec(dllimport)
#endif

#include "..\XFVoiceTool\XFVoiceApi.h"
#include "..\XFVoiceTool\SYGlobalFunction.h"