#include "pch.h"

CLog* CLog::m_pInstance = NULL;

HANDLE g_hMutex = CreateMutex(NULL, FALSE, _T("CLog"));

CLog* CLog::GetInstance()
{
	//����ʽ����ģʽ
	if (NULL == m_pInstance)	//ÿ�����������������⣬�����߳�����������ʵ����ֱ�ӷ���
	{
		//��֤�̰߳�ȫ�����ⶼ�ж�ʵ�������ڣ��������ʵ����
		WaitForSingleObject(g_hMutex, INFINITE);
		if (NULL == m_pInstance)
		{
			m_pInstance = new CLog;
		}
		ReleaseMutex(g_hMutex);
	}

	return m_pInstance;
}

void CLog::OpenFile()
{
	CreateDirectory("Log", NULL);

	SYSTEMTIME st;
	GetLocalTime(&st);

	char chFileName[50] = {};
	sprintf(chFileName, "Log//%02u_%02u_%02u.log", st.wYear, st.wMonth, st.wDay);

	m_pLogFile = fopen(chFileName, "ab+");
}

void CLog::CloseFile()
{
	fclose(m_pLogFile);
}

void CLog::Input(eLogLevel eLevel, char* pContent)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	switch (eLevel)
	{
	case eLogLevel_Info:
	{
		fprintf(m_pLogFile, "[INFO] %02u:%02u:%02u %s\n", st.wHour, st.wMinute, st.wSecond, pContent);
		break;
	}
	case eLogLevel_Debug:
	{
#ifdef _DEBUG
#endif
		break;
	}
	case eLogLevel_Err:
	{
		fprintf(m_pLogFile, "[ERROR] %02u:%02u:%02u %s\n", st.wHour, st.wMinute, st.wSecond, pContent);
		break;
	}
	}
}