/*
 * 讯飞语言合成工具类
 * 通过本工具可以自动将一段文字合成为音频文件，并通过回调函数反馈合成进度
 * 
 */
#pragma once
#include <vector>

class XFVoiceHelper
{
private:
	BOOL m_bLogined;//是否已登录
public:
	XFVoiceHelper();
	~XFVoiceHelper(void);

	/* strFileName:输出的音频文件名（全路径）
	 * strContent:需要合成为音频的文本内容，文本不能过长（），过长需要多次调用，本函数内部不做处理
	 * m_strVoiceName:播音员（女：xiaoyan，男：xiaoyu）
	 * dwSpeed:语速
	 * dwPitch:声调
	 * dwNumMode:数字发音方式（如2017年10月，=1，读作两千一百零七年十月，=2，读作二零幺七年幺零月，=3读作二零一七年十月）
	 * 返回值:生成的音频长度（单位毫秒）
	 */
	DWORD Excute(CString strFileName, CString strContent, CString strVoiceName = _T("xiaoyan"), DWORD dwSpeed = 50, DWORD dwPitch = 50, DWORD dwNumMode = 3);
	//将列表中的音频文件拼接成一个完整音频，并插入指定长度的静音
	//拼接形式为----文件0，间隔0，文件1，间隔1，......文件n，间隔n
	//文件数应当和间隔数一致
	//注意，间隔单位为毫秒，总时长单位为秒
	static DWORD CombineAudioFile(const std::vector<CString> & vecFileList, std::vector<DWORD> vecAudioGap, DWORD dwWholeTime, CString strRetFilePath);
};

