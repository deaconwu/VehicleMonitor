#include "pch.h"
#include "StepConfig.h"
#include "psapi.h"
#include <tlhelp32.h> 

void FilterConfig(char* pDst, const char* pSrc)
{
	const char* pTmp = pSrc;
	while (*pTmp != '\"')
	{
		pTmp++;
	}

	while (*(++pTmp) != '\"')
	{
		*pDst++ = *pTmp;
	}

	return;
}

void Str2Time(SYSTEMTIME& stTime, const char* pTimeStr, bool bDateOnly)
{
	const char* pTmp = pTimeStr;
	char chYear[5] = {};
	unsigned char iIndex = 0;
	while (*pTmp != '-')
	{
		chYear[iIndex++] = *pTmp++;
	}
	stTime.wYear = atoi(chYear);
	pTmp++;

	char chMon[3] = {};
	iIndex = 0;
	while (*pTmp != '-')
	{
		chMon[iIndex++] = *pTmp++;
	}
	if (chMon[0]==0)
	{
		stTime.wMonth = atoi(&chMon[1]);
	}
	else
	{
		stTime.wMonth = atoi(chMon);
	}
	pTmp++;

	char chDay[3] = {};
	iIndex = 0;
	if (bDateOnly)
	{
		chDay[iIndex++] = *pTmp++;
		chDay[iIndex++] = *pTmp;
	}
	else
	{
		while (*pTmp != ' ')
		{
			chDay[iIndex++] = *pTmp++;
		}
	}
	
	if (chDay[0] == 0)
	{
		stTime.wDay = atoi(&chDay[1]);
	}
	else
	{
		stTime.wDay = atoi(chDay);
	}

	if (bDateOnly)
	{
		return;
	}

	pTmp++;

	char chHour[3] = {};
	iIndex = 0;
	while (*pTmp != ':')
	{
		chHour[iIndex++] = *pTmp++;
	}
	if (chHour[0] == 0)
	{
		stTime.wHour = atoi(&chHour[1]);
	}
	else
	{
		stTime.wHour = atoi(chHour);
	}
	pTmp++;

	char chMin[3] = {};
	iIndex = 0;
	while (*pTmp != ':')
	{
		chMin[iIndex++] = *pTmp++;
	}
	if (chMin[0] == 0)
	{
		stTime.wMinute = atoi(&chMin[1]);
	}
	else
	{
		stTime.wMinute = atoi(chMin);
	}
	pTmp++;

	char chSec[3] = {};
	iIndex = 0;
	while (*pTmp != '\0')
	{
		chSec[iIndex++] = *pTmp++;
	}
	if (chSec[0] == 0)
	{
		stTime.wSecond = atoi(&chSec[1]);
	}
	else
	{
		stTime.wSecond = atoi(chSec);
	}
}

DWORD GetProcessidFromName(LPCTSTR name)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return 0;
	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (strcmp(pe.szExeFile, name) == 0)
		{
			id = pe.th32ProcessID;

			break;
		}
	}
	CloseHandle(hSnapshot);
	return id;
}