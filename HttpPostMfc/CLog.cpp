#include "pch.h"

CLog* CLog::m_pInstance = NULL;

HANDLE g_hMutex = CreateMutex(NULL, FALSE, _T("CLog"));

CLog* CLog::GetInstance()
{
	//懒汉式单例模式
	if (NULL == m_pInstance)	//每次上锁存在性能问题，导致线程阻塞；已有实例，直接返回
	{
		//保证线程安全，避免都判断实例不存在，都会进行实例化
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