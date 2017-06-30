#pragma once
#include <vector>

/* strFileName:输出的音频文件名（全路径）
 * strContent:需要合成为音频的文本内容，文本不能过长（），过长需要多次调用，本函数内部不做处理
 * m_strVoiceName:播音员（女：xiaoyan，男：xiaoyu）
 * dwSpeed:语速
 * dwPitch:声调
 * dwNumMode:数字发音方式（如2017年10月，=1，读作两千一百零七年十月，=2，读作二零幺七年幺零月，=3读作二零一七年十月）
 * 返回值:返回生成的音频文件长度，失败则为0
 */
XFVOICE_TOOL_API DWORD XunFei_Voive_Excute(CString strFileName, CString strContent, CString strVoiceName = _T("xiaoyan"), DWORD dwSpeed = 50, DWORD dwPitch = 50, DWORD dwNumMode = 1);
//将列表中的音频文件拼接成一个完整音频，并插入指定长度的静音
XFVOICE_TOOL_API DWORD XunFei_Voive_CombineAudioFile(const std::vector<CString> & vecFileList, const std::vector<DWORD> & vecAudioGap, DWORD dwWholeTime, CString strRetFilePath);