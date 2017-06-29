/*
 * 主要是一些具有通用性的工具函数类
 * 2015.09.07 杜江涛
 * 中广上洋
*/
#pragma once
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <afxmt.h>
using std::string;
using std::queue;

class SYCGlobalFunction
{
public:
	static CString GetNewGUID();//生成新的GUID
	static HRESULT GuidToString(const GUID& guid, CString& str);//转换GUID到字符串
	static void GetPathAndName(const CString &strSrcFilePath, CString &strPath, CString &strName);//根据文件全名获取文件路径及文件名
	static CString GetUnDuplicateFileName(const CString &strSrcFilePath);//根据指定文件名获取一个不重名的文件名(通过添加(n)的形式来避免重名)
	static BOOL IsFileExists(const CString &strSrcFilePath);//判定文件是否存在
	static string ConverCStringToStdString(CString & cstrValue);//CString与string之间的转换
	static string ConverCStringToStdString(CString & cstrValue, DWORD dwFormatW);//CString与string之间的转换
	static CString ConvertStdStringToCString(const string & strValue);//CString与string之间的转换
	static CString GetFilePath(CString strFullFileName);//获取文件路径
	static CString GetFileName(CString strFullFileName);//获取不带扩展名的文件名
	static CString GetFileNameWithExt(CString strFullFileName);//获取带扩展名的文件名
	static CString GetFileNameExt(const CString strSrcFileName);//获取文件扩展名.
	static CString FMT(LPCTSTR lpszFormat, ...);//把不定长的参数格式化
	static BOOL MakeSureDirectoryExists(CString strFilePath, CString & strErrInfo);//确保文件路径存在，如果不存在则创建，返回true表示路径存在或重建成功
	static void LoadBmpFileToCbitmap(const CString & strFilePath, CBitmap & bm);//将指定BMP文件装载到指定Cbitmap
	static CString GetSmartFileSize(unsigned long long llFileSize);//获取文件大小的文本描述
	static unsigned long long GetFileSize(const CString & strFulePath);//获取指定文件大小
	static void SetBootStart(BOOL bStart, CString strVariable);//设定当前程序开机启动，参数决定写入注册表的变量名
	static CString GetCurPath();//获取当前exe路径
	static CString GetNameOfComputer();//获取本机名字
	static CString ConvertNumToCString(DWORD dwValue);//转换DWORD到csing
	static CString ConvertNumToCString(int nValue);//转换整数到csing
	static CString ConvertNumToCString(float fValue);//转换浮点数到csing
	static CString ConvertNumToCString(double dValue);//转换双精度小数到csing
	static int ConvertCStringToInt(CString strValue);//转换cstring到整数
	static float ConvertCStringToFloat(CString strValue);//转换cstring到浮点数
	static double ConvertCStringToDouble(CString strValue);//转换cstring到双精度小数
	static int ConvertStdSringToInt(string & strValue);//转换string到整数
	static DWORD ConvertStdSringToDWORD(string & strValue);//转换string到无符号整数
	static float ConvertStdStringToFloat(string & strValue);//转换string到浮点数
	static double ConvertStdStringToDouble(string & strValue);//转换string到双精度小数
	static std::vector<string> SplitStdString(string & strValue, std::set<char> & setMark, BOOL bIncludeMark);//将strValue以setMark分割为string数组，bIncludeMark指定分割结果是否包含分隔符本身
	static std::vector<CString> SplitCString(CString & strValue, std::set<TCHAR> & setMark, BOOL bIncludeMark);//将strValue以setMark分割为string数组，bIncludeMark指定分割结果是否包含分隔符本身
	static CString ConvertNumToChinese(DWORD nNum);//转换数字到中文（目前仅支持正数且仅支持到千级别）
	static BOOL CanConvertToNum(CString strNum);//判定一个串是否可以被转换为数字（仅判定首字符是否是数字）
	static std::vector<CString> ScanFilePath(CString strFilePath);//遍历指定路径，获取路径下的文件名列表（不递归文件夹）
	static BOOL IsDirectory(CString strPath);// 是否是文件夹
	static BOOL DeleteFileOrPath(CString strFilePath);//删除文件（夹）
	static BOOL RenameFileOrPath(CString strDesPath, CString strSrcPath);//改名文件（夹）
	static BOOL CopyFileOrPath(CString strDesPath, CString strSrcPath);//拷贝文件（夹）
};

class CLogProcessor
{
private:
	queue<CString> m_queStrLogs;
	CRITICAL_SECTION m_lockLog;

	CWinThread * m_pThLog;
	CEvent m_hStopEvent;

	CLogProcessor(void);
public:
	~CLogProcessor(void);
	static CLogProcessor & Instance();

	UINT static LogThread(LPVOID pParam);

	void AddLogInfo(CString strLog);
	void WriteOneLog(CString strLog);
};

//时间调试器的宏开关
#define TIME_DEBUG_RECORD_OPEN

//时间调试器的便捷方式，会自动记录函数名
#ifndef TIME_DEBUG_RECORD
#define TIME_DEBUG_RECORD \
	TimerRecorder tm(__FUNCTION__);
#endif

#ifndef TIME_DEBUG_RECORD_FILTER
#define TIME_DEBUG_RECORD_FILTER(strFilter) \
	TimerRecorder tm(__FUNCTION__, (strFilter));
#endif

//时间调试器类，在构造时记下当前cpu时钟，在析构时输出其生命周期时长，用来进行耗时调试
class TimerRecorder
{
	friend class CLogProcessor;
private:
	static BOOL g_bShutDown;//总开关
	static CString g_strFilter;//过滤器，当不为空时，仅输出过滤器相同的信息
	static BOOL g_bFileMode;//true指定通过文件方式输出，false指定通过trace方式输出
	static CString g_strFileName;//输出到文件时的文件名
	int m_nBorn;
	CString m_strInfo;
	CString m_strFilter;
private:
	void WriteToFile(CString strMsg);
	void WriteToOutput(CString strMsg);
public:
	TimerRecorder(CString strInfo, LPCTSTR lpFilter = NULL);
	TimerRecorder(const char * pStrInfo, const char * pFilter = NULL);
	~TimerRecorder();
	static void ReadConfig(CString strConfigPath = _T(""));//从配置文件中读取配置
	void Record(BOOL bResetTime = TRUE, LPCTSTR lpStr = NULL);
};
