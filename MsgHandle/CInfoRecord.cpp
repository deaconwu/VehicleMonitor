#include "pch.h"
#include "CInfoRecord.h"
#include "AlertCalc.h"
#include "TimeStamp.h"
#include <time.h>
#include "CWarningRank.h"
#include "UserMessage.h"

CInfoRecord* CInfoRecord::m_pInstance = NULL;

static bool g_bTerminate = false;

UCHAR* g_recMem = (UCHAR*)malloc(SHAREMEM_SIZE);
//UCHAR g_recMem[SHAREMEM_SIZE] = {};
SYSTEMTIME g_datePre = {};

char* g_pFileBuff = (char*)malloc(MAX_FILE_SIZE);

HANDLE g_hMutex = CreateMutex(NULL, FALSE, _T("InfoRecord"));

DWORD WINAPI OnRecThread(LPVOID lparam);

CInfoRecord::CInfoRecord() : m_vehicleNum(0)
{
	memset(m_chVin, 0, sizeof(m_chVin));
	memset(m_pVehicleRec, 0, sizeof(m_pVehicleRec));
	memset(m_iCurRecIndex, 0, sizeof(m_iCurRecIndex));
	memset(m_alertTimes, 0, sizeof(m_alertTimes));
	memset(m_bTodayJoin, 0, sizeof(m_bTodayJoin));
	memset(m_sTime, 0, sizeof(m_sTime));
	memset(m_voltageException, 0, sizeof(m_voltageException));
	memset(m_tempException, 0, sizeof(m_tempException));
	memset(m_warningValueTimes, 0, sizeof(m_warningValueTimes));
	memset(m_chRefreshVin, 0, sizeof(m_chRefreshVin));
	memset(g_recMem, 0, SHAREMEM_SIZE);
}

void CInfoRecord::OnReset()
{
	WaitForSingleObject(g_hMutex, INFINITE);
	for (UINT i=0; i<m_vehicleNum; i++)
	{
		LONGLONG offset = i * RECNUM_PER_VEHICEL * sizeof(STRECDATA);
		m_pVehicleRec[i] = (STRECDATA*)(g_recMem+offset);
	}
	
	memset(m_iCurRecIndex, 0, sizeof(m_iCurRecIndex));
	memset(m_alertTimes, 0, sizeof(m_alertTimes));
	memset(m_bTodayJoin, 0, sizeof(m_bTodayJoin));
	memset(g_recMem, 0, SHAREMEM_SIZE);
	memset(m_sTime, 0, sizeof(m_sTime));
	memset(m_warningValueTimes, 0, sizeof(m_warningValueTimes));
	ReleaseMutex(g_hMutex);
}

void CInfoRecord::SaveBin()
{
	FILE *fpRead = fopen("Vins.txt", "rb");

	FILE *fpWrite = fopen("Vins.dat", "wb+");

	char strVin[VIN_LENGTH+1] = {};

	while (!feof(fpRead))
	{
		fgets((char*)strVin, VIN_LENGTH + 1, fpRead);

		fprintf(fpWrite, "%s", strVin);

		fgets((char*)strVin, 3, fpRead);
	}

	fclose(fpRead);
	fclose(fpWrite);
}

bool CInfoRecord::ReadVin()
{
// 	TCHAR tchPath[1024] = {};
// 	GetCurrentDirectory(1024, tchPath);
// 
// 	char chPath[1024] = {};
// 	int iLength = WideCharToMultiByte(CP_ACP, 0, tchPath, -1, NULL, 0, NULL, NULL);
// 	WideCharToMultiByte(CP_ACP, 0, tchPath, -1, chPath, iLength, NULL, NULL);
// 	memcpy(&chPath[iLength - 1], "\\Vins.txt", sizeof("\\Vins.txt"));

	FILE *fpRead = fopen("Vins.dat", "rb");

	// 	char chBuf[BUFFER_SIZE] = {};
 	fseek(fpRead, 0, SEEK_END);
 	long len = ftell(fpRead);
 	fseek(fpRead, 0, SEEK_SET);
 	fread((char*)m_chVin, 1, len, fpRead);
 	fclose(fpRead);

	m_vehicleNum = len / VIN_LENGTH;
	for (UINT i=0; i<m_vehicleNum; i++)
	{
		m_pVehicleRec[i] = (STRECDATA*)(g_recMem + i *RECNUM_PER_VEHICEL * sizeof(STRECDATA));
	}

// 	char strVin[1024] = {};
// 	LONGLONG offset = 0;
// 	while (!feof(fpRead))
// 	{
// 		fgets((char*)m_chVin[m_vehicleNum], VIN_LENGTH+3, fpRead);
// 
// 		offset = m_vehicleNum * RECNUM_PER_VEHICEL * sizeof(STRECDATA);
// 		m_pVehicleRec[m_vehicleNum] = (STRECDATA*)(g_recMem+offset);
// 
// 		m_vehicleNum += 1;
// 	}
// 
// 	fclose(fpRead);

	GetLocalTime(&g_datePre);

	return true;
}

INT CInfoRecord::FindVinPos(UCHAR pVin[])
{
	INT left = 0;
	INT right = m_vehicleNum - 1;
	INT mid = -1;

	while (left <= right)
	{
		mid = (left + right) / 2;
		if (memcmp((char*)pVin, (char*)m_chVin[mid], VIN_LENGTH) == 0)
		{
			return mid;
		}
		else if (memcmp((char*)pVin, (char*)m_chVin[mid], VIN_LENGTH) < 0)
		{
			right = mid - 1;
		}
		else
		{
			left = mid + 1;
		}
	}

	return -1;
}

void CInfoRecord::InsertRec(INT iPos, char* pBuf)
{
	//记录
	m_bTodayJoin[iPos] = 1;
	GetLocalTime(&m_sTime[iPos]);

	UCHAR iIndex = m_iCurRecIndex[iPos];
	STRECDATA* pRec = m_pVehicleRec[iPos];
	pRec = pRec + iIndex;
	memcpy(pRec, pBuf, sizeof(STRECDATA));

	//欠压
	if (pRec->F6_3 >= 1 && pRec->F6_3 <= 2827)
	{
		m_warningValueTimes[0][iPos] += 1;
	}

	//过压
	if (pRec->F6_1 >= 4250 && pRec->F6_1 <= 15000)
	{
		m_warningValueTimes[1][iPos] += 1;
	}

	//电压异常
	if (pRec->F6_1 == 65534 || pRec->F6_3 == 65534)
	{
		m_warningValueTimes[2][iPos] += 1;
	}

	//过温
	if (pRec->F6_5 >= 95 && pRec->F6_5 != 254)
	{
		m_warningValueTimes[3][iPos] += 1;
	}

	//温度异常
	if (pRec->F6_5 == 254 || pRec->F6_7 == 254)
	{
		m_warningValueTimes[4][iPos] += 1;
	}

	//电压警报
	if (CheckAlertFlag(pRec->F7_0, 5) || CheckAlertFlag(pRec->F7_0, 6))
		m_warningValueTimes[5][iPos] += 1;

	//温度警报
	if (CheckAlertFlag(pRec->F7_0, 0) || CheckAlertFlag(pRec->F7_0, 1) || CheckAlertFlag(pRec->F7_0, 12) || CheckAlertFlag(pRec->F7_0, 17))
		m_warningValueTimes[6][iPos] += 1;

	if (pRec->F7_0 > 0)
	{
		m_alertTimes[iPos][0] += 1;

		for (UCHAR i = 0; i < ALERT_CATEGORY_NUM - 1; i++)
		{
			if (CheckAlertFlag(pRec->F7_0, i))
			{
				m_alertTimes[iPos][i + 1] += 1;
			}
		}
	}

// 	if (pRec->F6_3 == 0)
// 	{
// 		m_voltageValueTimes[0][iPos] += 1;
// 	}
// 	else if (pRec->F6_3>=1 && pRec->F6_3<=2650)
// 	{
// 		m_voltageValueTimes[1][iPos] += 1;
// 	}
// 	else if (pRec->F6_3 >= 2651 && pRec->F6_3 <= 4200)
// 	{
// 		m_voltageValueTimes[2][iPos] += 1;
// 	}
// 	else if (pRec->F6_3 >= 4201 && pRec->F6_3 <= 15000)
// 	{
// 		m_voltageValueTimes[3][iPos] += 1;
// 	}
// 	else if (pRec->F6_3 >= 15001 && pRec->F6_3 <= 65533)
// 	{
// 		m_voltageValueTimes[4][iPos] += 1;
// 	}
// 	else if (pRec->F6_3 == 65534)
// 	{
// 		m_voltageValueTimes[5][iPos] += 1;
// 	}
// 	else if (pRec->F6_3 == 65535)
// 	{
// 		m_voltageValueTimes[6][iPos] += 1;
// 	}

// 	if (pRec->F6_1 == 65534 || pRec->F6_3 == 65534)
// 	{
// 		m_voltageException[iPos] += 1;
// 	}

// 	if (pRec->F6_5 == 254 || pRec->F6_7 == 254)
// 	{
// 		m_tempException[iPos] += 1;
// 	}

	m_iCurRecIndex[iPos] = (m_iCurRecIndex[iPos] + 1) % RECNUM_PER_VEHICEL;

	if (memcmp(m_chRefreshVin, m_chVin[iPos], VIN_LENGTH) == 0)
	{
		PostMessage(m_hWndMain, UM_REFRESHREC, NULL, 0);
	}
}

void CInfoRecord::OnParse(char buf[], int len)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	if (g_datePre.wDay != st.wDay || g_datePre.wMonth != st.wMonth || g_datePre.wYear != st.wYear)
	{
		CWarningRank::GetInstance()->OnSaveList(g_datePre.wYear, g_datePre.wMonth, g_datePre.wDay);
		g_datePre.wYear = st.wYear;
		g_datePre.wMonth = st.wMonth;
		g_datePre.wDay = st.wDay;
		OnReset();
	}

	char bufRecv[BUFFER_SIZE] = {};
	memcpy(bufRecv, buf, len);

	const char* pDir = (char*)strtok(bufRecv, " ");
//	FILE *pFile = NULL;

 	while (pDir)
 	{
// 		pFile = fopen(pDir, "rb");
// 		if (NULL == pFile)
// 		{
// 			CString str = L"";
// 			TCHAR tchVin[200] = {};
// 			MultiByteToWideChar(CP_ACP, 0, pDir, -1, tchVin, 30);
// 			str.Format(_T("读取文件失败%s"), tchVin);
// 			MessageBox(m_hWndMain, str, _T("提示"), MB_OK | MB_ICONERROR);
// 		}
// 		else
// 		{
// 			fclose(pFile);
// 		}

 		DWORD dwThreadId;
		HANDLE hThread = CreateThread(NULL, NULL, OnRecThread, (LPVOID)pDir, 0, &dwThreadId);

 		pDir = (char*)strtok(NULL, " ");
 	}
}

DWORD WINAPI OnRecThread(LPVOID lparam)
{
	char strDir[200] = {};
	strcpy(strDir, (const char*)lparam);

	Sleep(1000);

	if (g_bTerminate)
		return 0;

	FILE *pFile = fopen(strDir, "rb");
	if (NULL == pFile)
	{
		Sleep(1500);
		if (g_bTerminate)
			return 0;

		pFile = fopen(strDir, "rb");
		if (NULL == pFile)
			return 0;
	}

	char chItem[4] = {};
	size_t ret = fseek(pFile, 0, SEEK_SET);
	ret = fread(chItem, 1, 4, pFile);
	UINT itemNum = 0;
	memcpy(&itemNum, chItem, 4);
	itemNum += 1;

	if (itemNum>5000)
	{
		itemNum = 5000;
	}

	char chFileBuff[MAX_FILE_SIZE] = {};
	long offset = 0;
	memset(chFileBuff, 0, MAX_FILE_SIZE);
 	ret = fseek(pFile, 4, SEEK_SET);
 	ret = fread(chFileBuff, 1, itemNum*(VIN_LENGTH + sizeof(STRECDATA)), pFile);
	fclose(pFile);

	if (ret == 0)
	{
		return 0;
	}

	char* pBuf = chFileBuff;

	while (itemNum>0)
	{
		if (g_bTerminate)
			break;

		//vin码
		UCHAR chVin[VIN_LENGTH] = {};
		memcpy(chVin, pBuf, VIN_LENGTH);

		pBuf = pBuf + VIN_LENGTH;

		//查找
		INT iPos = CInfoRecord::GetInstance()->FindVinPos(chVin);
		if (iPos < 0)
		{
			pBuf = pBuf + sizeof(STRECDATA);
			itemNum--;
			continue;
		}

		WaitForSingleObject(g_hMutex, INFINITE);
		CInfoRecord::GetInstance()->InsertRec(iPos, pBuf);
		ReleaseMutex(g_hMutex);

		pBuf = pBuf + sizeof(STRECDATA);

		itemNum--;
	}

	return 0;
}

void CInfoRecord::OnTerminate()
{
	g_bTerminate = true;
}

void CInfoRecord::OnStatistic(STSTATISTICDATA &stData)
{
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);

	time_t tNow = ConvertSecondsTime(stNow);//stNow.wHour * 60 * 60 + stNow.wMinute * 60 + stNow.wSecond;

	for (UINT i=0; i<m_vehicleNum; i++)
	{
		if (m_bTodayJoin[i])
		{
			stData.iSumToday += 1;
			stData.iMileageSum += m_pVehicleRec[i]->F1_4;

			if (m_pVehicleRec[i]->F7_0 > 0)
			{
				stData.iDefaultToday += 1;
			}

			if (m_pVehicleRec[i]->F1_1 == 1 || m_pVehicleRec[i]->F1_1 == 2)
			{
				stData.iRechargeToday += 1;
			}

			time_t tRec = ConvertSecondsTime(m_sTime[i]);//m_sTime[i].wHour* 60 * 60 + m_sTime[i].wMinute * 60 + m_sTime[i].wSecond;

			if ((tNow >= tRec) && (tNow - tRec <= TIME_OFFLINE_DISTANCE))
			{
				stData.iOnlineToday += 1;
			}
			else
			{
				stData.iOfflineToday += 1;
			}

// 			SYSTEMTIME stRec = {};
// 			ConvertDateTime(tRec, stRec);
		}
	}
}

void CInfoRecord::OnAlert(STALERTDATA &stData)
{
	for (UINT i = 0; i < m_vehicleNum; i++)
	{
		stData.iAlertTimes[i] = m_alertTimes[i][0];
		stData.iVoltageException[i] = m_voltageException[i];
		stData.iTempException[i] = m_tempException[i];
	}
}

void CInfoRecord::OnVoltage(STVOLTAGEDATA &stData)
{
	memcpy(&stData, m_voltageValueTimes, sizeof(STVOLTAGEDATA));
}

void CInfoRecord::OnWarning(STWARNINGDATA &stData)
{
	memcpy(&stData, m_warningValueTimes, sizeof(STWARNINGDATA));
}

bool CInfoRecord::QueryLatestInfo(UCHAR pVin[], STRECDATA &stData, UINT alertTimes[])
{
	INT iPos = FindVinPos(pVin);
	if (iPos < 0)
	{
		return false;
	}

	UCHAR iIndex = m_iCurRecIndex[iPos];
	stData = *m_pVehicleRec[iPos];

	memcpy(alertTimes, &m_alertTimes[iPos], ALERT_CATEGORY_NUM*sizeof(UINT));

	return true;
}

void CInfoRecord::QueryHistoryInfo(UCHAR pVin[], STRECDATA arrRec[], UCHAR& iIndex)
{
	INT iPos = FindVinPos(pVin);
	if (iPos < 0)
	{
		return;
	}

	WaitForSingleObject(g_hMutex, INFINITE);

	iIndex = m_iCurRecIndex[iPos];
	LONGLONG offset = iPos * RECNUM_PER_VEHICEL * sizeof(STRECDATA);
	STRECDATA* pRecBegin = (STRECDATA*)(g_recMem + offset);

	memcpy(arrRec, pRecBegin, RECNUM_PER_VEHICEL * sizeof(STRECDATA));

	ReleaseMutex(g_hMutex);
}

UINT CInfoRecord::GetVins(UCHAR chVin[][VIN_LENGTH])
{
	memcpy(chVin, m_chVin, sizeof(m_chVin));
	return m_vehicleNum;
}
