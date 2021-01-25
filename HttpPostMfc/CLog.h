#pragma once

enum eLogLevel
{
	eLogLevel_Info = 0,
	eLogLevel_Debug,
	eLogLevel_Err,
};

class CLog
{
public:
	static CLog* GetInstance();

	void OpenFile();

	void CloseFile();

	void Input(eLogLevel eLevel, char* pContent);

private:
	CLog() {}

	FILE* m_pLogFile;

	static CLog* m_pInstance;
};