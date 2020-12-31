#pragma once

#include "StepConfig.h"

class CStepDlgBase
{
public:
	CStepDlgBase()
	{
		memset(&m_bi, 0, sizeof(m_bi));
		memset(&m_SqliteLog, 0, sizeof(m_SqliteLog));

		strcpy(m_SqliteLog.dbName, "log_vehicle");
		//m_bi.ulFlags = BIF_BROWSEINCLUDEFILES | BIF_NEWDIALOGSTYLE;
	}

	void SetHwnd(HWND hwnd) { m_hPWnd = hwnd; }

	void InputStep1Date(SYSTEMTIME& stDate) { memcpy(&m_step1Date, &stDate, sizeof(SYSTEMTIME)); }

protected:
	HWND m_hPWnd;
	BROWSEINFO m_bi;
	STSQLITELOG m_SqliteLog;
	SYSTEMTIME m_step1Date;	//配置的数据接收时间
};