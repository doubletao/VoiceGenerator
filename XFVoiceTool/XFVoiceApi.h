#pragma once
#include <vector>

/* strFileName:�������Ƶ�ļ�����ȫ·����
 * strContent:��Ҫ�ϳ�Ϊ��Ƶ���ı����ݣ��ı����ܹ���������������Ҫ��ε��ã��������ڲ���������
 * m_strVoiceName:����Ա��Ů��xiaoyan���У�xiaoyu��
 * dwSpeed:����
 * dwPitch:����
 * dwNumMode:���ַ�����ʽ����2017��10�£�=1��������ǧһ��������ʮ�£�=2���������������������£�=3��������һ����ʮ�£�
 * ����ֵ:�������ɵ���Ƶ�ļ����ȣ�ʧ����Ϊ0
 */
XFVOICE_TOOL_API DWORD XunFei_Voive_Excute(CString strFileName, CString strContent, CString strVoiceName = _T("xiaoyan"), DWORD dwSpeed = 50, DWORD dwPitch = 50, DWORD dwNumMode = 1);
//���б��е���Ƶ�ļ�ƴ�ӳ�һ��������Ƶ��������ָ�����ȵľ���
XFVOICE_TOOL_API DWORD XunFei_Voive_CombineAudioFile(const std::vector<CString> & vecFileList, const std::vector<DWORD> & vecAudioGap, DWORD dwWholeTime, CString strRetFilePath);