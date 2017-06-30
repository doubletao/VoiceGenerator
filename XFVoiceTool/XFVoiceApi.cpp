#include "StdAfx.h"
#include "XFVoiceApi.h"
#include "XFVoiceHelper.h"

XFVOICE_TOOL_API DWORD XunFei_Voive_Excute(CString strFileName, CString strContent, CString strVoiceName /*= _T("xiaoyan")*/, DWORD dwSpeed /*= 50*/, DWORD dwPitch /*= 50*/, DWORD dwNumMode /*= 1*/)
{
	XFVoiceHelper helper;
	return helper.Excute(strFileName, strContent, strVoiceName, dwSpeed, dwPitch, dwNumMode);
}

XFVOICE_TOOL_API DWORD XunFei_Voive_CombineAudioFile(const std::vector<CString> & vecFileList, const std::vector<DWORD> & vecAudioGap, DWORD dwWholeTime, CString strRetFilePath)
{
	return XFVoiceHelper::CombineAudioFile(vecFileList, vecAudioGap, dwWholeTime, strRetFilePath);
}
