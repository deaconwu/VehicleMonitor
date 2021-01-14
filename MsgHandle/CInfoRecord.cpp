#include "pch.h"
#include "CInfoRecord.h"
#include "AlertCalc.h"
#include "TimeStamp.h"
#include <time.h>
#include "CWarningRank.h"
#include "UserMessage.h"
#include "CRecordYestoday.h"
#include "CRecordLastweek.h"

CInfoRecord* CInfoRecord::m_pInstance = NULL;
UCHAR m_chVinEx[MAX_VEHICLENUM][VIN_LENGTH] = {};
UINT m_vehicleNumEx = 0;
UCHAR m_iCurRecIndex[MAX_VEHICLENUM];
static bool g_bTerminate = false;

static char* g_pMileageRankMem = (char*)malloc(sizeof(STVINMILEAGELINK)*NUM_MILEAGERANK);

UCHAR* g_recMem = (UCHAR*)malloc(SHAREMEM_SIZE);
//UCHAR g_recMem[SHAREMEM_SIZE] = {};
SYSTEMTIME g_datePre = {};

char* g_pFileBuff = (char*)malloc(MAX_FILE_SIZE);

HANDLE g_hMutex = CreateMutex(NULL, FALSE, _T("InfoRecord"));

DWORD WINAPI OnRecThread(LPVOID lparam);

CInfoRecord::CInfoRecord() : m_vehicleNum(0), m_chargeTimes(0), m_chargeSeconds(0)/*, m_vehicleNumEx(0)*/
{
	memset(m_chVinEx, 0, sizeof(m_chVinEx));
	memset(m_chVin, 0, sizeof(m_chVin));
	memset(m_pVehicleRec, 0, sizeof(m_pVehicleRec));
	memset(m_iCurRecIndex, 0, sizeof(m_iCurRecIndex));
	memset(m_alertTimes, 0, sizeof(m_alertTimes));
	memset(m_bTodayJoin, 0, sizeof(m_bTodayJoin));
	memset(m_sTime, 0, sizeof(m_sTime));
// 	memset(m_voltageException, 0, sizeof(m_voltageException));
// 	memset(m_tempException, 0, sizeof(m_tempException));
	memset(m_warningValueTimes, 0, sizeof(m_warningValueTimes));
	memset(m_chRefreshVin, 0, sizeof(m_chRefreshVin));
	memset(g_recMem, 0, SHAREMEM_SIZE);
}

CInfoRecord* CInfoRecord::GetInstance()
{
	//懒汉式单例模式
	if (NULL == m_pInstance)	//每次上锁存在性能问题，导致线程阻塞；已有实例，直接返回
	{
		//保证线程安全，避免都判断实例不存在，都会进行实例化
		WaitForSingleObject(g_hMutex, INFINITE);
		if (NULL == m_pInstance)
		{
			m_pInstance = new CInfoRecord;
		}
		ReleaseMutex(g_hMutex);
	}

	return m_pInstance;
}

void CInfoRecord::OnReset()
{
	WaitForSingleObject(g_hMutex, INFINITE);
	for (UINT i=0; i<m_vehicleNum; i++)
	{
// 		LONGLONG offset = i * RECNUM_PER_VEHICEL * sizeof(STRECDATA);
// 		m_pVehicleRec[i] = (STRECDATA*)(g_recMem+offset);
		m_pVehicleRec[i] = NULL;
	}
	
	memset(m_iCurRecIndex, 0, sizeof(m_iCurRecIndex));
	memset(m_alertTimes, 0, sizeof(m_alertTimes));
	memset(m_bTodayJoin, 0, sizeof(m_bTodayJoin));
	memset(g_recMem, 0, SHAREMEM_SIZE);
	memset(m_sTime, 0, sizeof(m_sTime));
	memset(m_warningValueTimes, 0, sizeof(m_warningValueTimes));

	m_chargeTimes = 0;
	m_chargeSeconds = 0;

	ReleaseMutex(g_hMutex);
}

void CInfoRecord::SaveBin()
{
	FILE *fpRead = fopen("Vins.txt", "rb");

	FILE *fpWrite = fopen("Vins.dat", "wb");

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

void CInfoRecord::SaveTxt()
{
	FILE *fpRead = fopen("Vins.dat", "rb");
	FILE *fpWrite = fopen("Vins.txt", "wb");

	UCHAR* pVin = (UCHAR*)malloc(MAX_VEHICLENUM*VIN_LENGTH);

	fseek(fpRead, 0, SEEK_END);
	long len = ftell(fpRead);
	fseek(fpRead, 0, SEEK_SET);
	fread((char*)pVin, 1, len, fpRead);
	fclose(fpRead);

	UINT nNum = len / VIN_LENGTH;
	char chVin[VIN_LENGTH+1] = {};

	for (UINT i=0; i<nNum; i++)
	{
		memcpy(chVin, pVin+i*VIN_LENGTH, VIN_LENGTH);
		fprintf(fpWrite, "%s", chVin);
		if (i != nNum-1)
		{
			fprintf(fpWrite, "\n");
		}
	}

	fclose(fpWrite);
	free(pVin);
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
// 	for (UINT i=0; i<m_vehicleNum; i++)
// 	{
// 		m_pVehicleRec[i] = (STRECDATA*)(g_recMem + i *RECNUM_PER_VEHICEL * sizeof(STRECDATA));
// 	}

 	memcpy(m_chVinEx, m_chVin, sizeof(m_chVin));
 	m_vehicleNumEx = m_vehicleNum;

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

void CInfoRecord::WriteVin()
{
	if (m_vehicleNumEx <= m_vehicleNum)
		return;

	if (m_vehicleNumEx > MAX_VEHICLENUM)
	{
		m_vehicleNumEx = MAX_VEHICLENUM;
	}

	FILE *fpWrite = fopen("Vins.dat", "wb");

	UCHAR iNum = 0;
	for (UCHAR i=0; i<VIN_LENGTH; i++)
	{
		if (m_chVinEx[m_vehicleNumEx-1][i] != ' ')
		{
			iNum++;
		}
	}

	if (iNum==VIN_LENGTH)
	{
		fprintf(fpWrite, "%.*s", m_vehicleNumEx*VIN_LENGTH, m_chVinEx);
	}
	else
	{
		fprintf(fpWrite, "%.*s", (m_vehicleNumEx-1)*VIN_LENGTH, m_chVinEx);
	}

	fclose(fpWrite);
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

INT CInfoRecord::FindVinPosEx(UCHAR pVin[])
{
	WaitForSingleObject(g_hMutex, INFINITE);

	INT left = 0;
	INT right = m_vehicleNumEx - 1;
	INT mid = -1;

	while (left <= right)
	{
		mid = (left + right) / 2;
		if (memcmp((char*)pVin, (char*)m_chVinEx[mid], VIN_LENGTH) == 0)
		{
			return mid;
		}
		else if (memcmp((char*)pVin, (char*)m_chVinEx[mid], VIN_LENGTH) < 0)
		{
			right = mid - 1;
		}
		else
		{
			left = mid + 1;
		}
	}

	ReleaseMutex(g_hMutex);

	return -1;
}

void CInfoRecord::InsertVinAndSort(UCHAR pVin[])
{
	if (m_vehicleNumEx >= MAX_VEHICLENUM)
	{
		return;
	}

	if (pVin[0] >= '0' && pVin[0] <= '9')
	{
		return;
	}

	UCHAR iNum = 0;
	UCHAR iAlpha = 0;
	for (int i = 0; i < VIN_LENGTH; i++)
	{
		if (pVin[i] < '0' || (pVin[i] > '9' && pVin[i] < 'A') || (pVin[i] > 'Z' && pVin[i] < 'a') || pVin[i]>'z')
			return;

		if (pVin[i] >= '0' && pVin[i] <= '9')
			iNum += 1;

		if ((pVin[i] >= 'A' && pVin[i] <= 'Z') || (pVin[i] >= 'a' && pVin[i] <= 'z'))
			iAlpha += 1;
	}

	if (VIN_LENGTH==iNum || VIN_LENGTH==iAlpha)
	{
		return;
	}

	WaitForSingleObject(g_hMutex, INFINITE);
	if (m_vehicleNumEx == 0)
	{
		memcpy(m_chVinEx[0], pVin, VIN_LENGTH);
		m_vehicleNumEx += 1;
		ReleaseMutex(g_hMutex);
		return;
	}

	long iLeft = 0;
	long iRight = m_vehicleNumEx - 1;

	while (iLeft <= iRight)
	{
		long iMid = (iLeft + iRight) / 2;
		if (memcmp((char*)pVin, (char*)m_chVinEx[iMid], VIN_LENGTH) > 0)
		{
			iLeft = iMid + 1;
		}
		else
		{
			iRight = iMid - 1;
		}
	}

	for (INT i = m_vehicleNumEx - 1; i >= iLeft; i--)
	{
		memcpy(m_chVinEx[i + 1], m_chVinEx[i], VIN_LENGTH);
	}

	memcpy(m_chVinEx[iLeft], (char*)pVin, VIN_LENGTH);

	m_vehicleNumEx += 1;
	ReleaseMutex(g_hMutex);
}

void CInfoRecord::InsertRec(INT iPos, char* pBuf)
{
	//记录
	m_bTodayJoin[iPos] = 1;
	GetLocalTime(&m_sTime[iPos]);

	if (NULL == m_pVehicleRec[iPos])
	{
		m_pVehicleRec[iPos] = (STRECDATA*)(g_recMem + iPos * RECNUM_PER_VEHICEL * sizeof(STRECDATA));
	}

	UCHAR iIndex = m_iCurRecIndex[iPos];
	STRECDATA* pRec = m_pVehicleRec[iPos];
	pRec = pRec + iIndex;
	memcpy(pRec, pBuf, sizeof(STRECDATA));

	if (pRec->F1_1 == 1 || pRec->F1_1 == 2)
	{
		m_chargeTimes += 1;

		UCHAR iIndexPre = (iIndex - 1 + RECNUM_PER_VEHICEL) % RECNUM_PER_VEHICEL;

		STRECDATA* pRecPre = pRec + iIndexPre;

		if (pRecPre->F1_1 == 1 || pRecPre->F1_1 == 2)
		{
			//连续2条记录为充电状态，计算充电时长
			USHORT iYear = pRec->F8[0];
			if (iYear<1000)
			{
				iYear = (m_sTime[iPos].wYear / 100) * 100 + iYear % 100;
			}

			time_t tPre = ConvertSecondsTime(iYear, pRecPre->F8[1], pRecPre->F8[2], pRecPre->F8[3], pRecPre->F8[4], pRecPre->F8[5]);

			iYear = pRec->F8[0];
			if (iYear < 1000)
			{
				iYear = (m_sTime[iPos].wYear / 100) * 100 + iYear % 100;
			}

			time_t tCur = ConvertSecondsTime(iYear, pRec->F8[1], pRec->F8[2], pRec->F8[3], pRec->F8[4], pRec->F8[5]);

			if (tCur > tPre)
			{
				time_t tDiff = tCur - tPre;
				m_chargeSeconds += tDiff;
			}
		}
	}

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
	if (pRec->F6_5 >= 95 && pRec->F6_5 <= 250)
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
		//保存每日统计
		OnSaveStatistics(g_datePre.wYear, g_datePre.wMonth, g_datePre.wDay);

		//保存每日警报
		CWarningRank::GetInstance()->OnSaveList(g_datePre.wYear, g_datePre.wMonth, g_datePre.wDay);

		WaitForSingleObject(g_hMutex, INFINITE);

		//记录昨日数据, 在线、离线、充电、故障车辆数依据时间为昨日24点
		MoveToYestoday();

		if (st.wDayOfWeek == 0)
		{
			//记录上周数据, 在线、离线、充电、故障车辆数依据时间为上周日24点
			MoveToLastWeek();
		}

		ReleaseMutex(g_hMutex);

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

	if (g_bTerminate)
		return 0;

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

	if (g_bTerminate)
		return 0;

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
			iPos = CInfoRecord::GetInstance()->FindVinPosEx(chVin);
			if (iPos < 0)
			{
				CInfoRecord::GetInstance()->InsertVinAndSort(chVin);	//录入新vin码，下次程序启动读出
			}

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
	WriteVin();
	g_bTerminate = true;
}

void CInfoRecord::OnSaveStatistics(WORD wYear, WORD wMonth, WORD wDay)
{
	char strFileName[100] = {};
	sprintf(strFileName, "Statistics_%d-%d-%d.txt", wYear, wMonth, wDay);

	FILE *fpWrite = fopen(strFileName, "wb+");
	if (NULL == fpWrite)
	{
		return;
	}

	STSTATISTICDATATODAY stData = {};
	CInfoRecord::GetInstance()->OnStatisticToday(stData);

	fprintf(fpWrite, "接入车辆数: %u\n", stData.iJoin);
	fprintf(fpWrite, "在线车辆数: %u\n", stData.iOnline);
	fprintf(fpWrite, "离线车辆数: %u\n", stData.iOffline);
	fprintf(fpWrite, "故障车辆数: %u\n", stData.iFault);
	fprintf(fpWrite, "充电车辆数: %u\n\n", stData.iRecharge);
	fprintf(fpWrite, "累计行驶里程: %.5f\n", (long double)stData.iMileageSum / 10 / 10000);
	fprintf(fpWrite, "累计碳减排: %.4f\n", (0.0031 / 6) * ((long double)stData.iMileageSum / 10 / 10000));
	fprintf(fpWrite, "累计节油量: %.2f\n", (1.75 / 6) * ((long double)stData.iMileageSum / 10 / 10000));
	fprintf(fpWrite, "累计耗电量: %.2f\n", (5.68 / 6) * ((long double)stData.iMileageSum / 10 / 10000));

	fclose(fpWrite);
}

void CInfoRecord::OnStatisticToday(STSTATISTICDATATODAY &stData)
{
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);

	time_t tNow = ConvertSecondsTime(stNow);//stNow.wHour * 60 * 60 + stNow.wMinute * 60 + stNow.wSecond;

	for (UINT i=0; i<m_vehicleNum; i++)
	{
		if (NULL == m_pVehicleRec[i])
			continue;

		if (m_bTodayJoin[i])
		{
			stData.iJoin += 1;

			//有效值才计入累计值
			if (m_pVehicleRec[i]->F1_4 > 0 && m_pVehicleRec[i]->F1_4 <= 9999999)
			{
				stData.iMileageSum += m_pVehicleRec[i]->F1_4;
			}

			if (m_pVehicleRec[i]->F7_0 > 0)
			{
				stData.iFault += 1;
			}

			if (m_pVehicleRec[i]->F1_1 == 1 || m_pVehicleRec[i]->F1_1 == 2)
			{
				stData.iRecharge += 1;
			}

			time_t tRec = ConvertSecondsTime(m_sTime[i]);//m_sTime[i].wHour* 60 * 60 + m_sTime[i].wMinute * 60 + m_sTime[i].wSecond;

			if ((tNow >= tRec) && (tNow - tRec <= TIME_OFFLINE_DISTANCE))
			{
				stData.iOnline += 1;
			}
			else
			{
				stData.iOffline += 1;
			}

// 			SYSTEMTIME stRec = {};
// 			ConvertDateTime(tRec, stRec);
		}
	}
}

void CInfoRecord::OnMileageRank(STMSGMILEAGERANKSEQ &msgSeq)
{
	memset(g_pMileageRankMem, 0, sizeof(STVINMILEAGELINK)*NUM_MILEAGERANK);

	PSTVINMILEAGELINK pNode = NULL;
	PSTVINMILEAGELINK pLast = NULL;
	UINT iRankNum = 0;
	UINT iValue = 0;

	for (UINT i = 0; i < m_vehicleNum; i++)
	{
		//有值的才排序
		if (NULL== m_pVehicleRec[i] || 0==m_pVehicleRec[i]->F1_4)
		{
			continue;
		}

		//有效值才排序
// 		if (m_pVehicleRec[i]->F1_4==0xFFFFFFFE || m_pVehicleRec[i]->F1_4 == 0xFFFFFFFF)
// 		{
// 			continue;
// 		}

		//有效值才排序
		if (m_pVehicleRec[i]->F1_4 > 9999999)
		{
			continue;
		}

		iValue = m_pVehicleRec[i]->F1_4;

		if (pNode == NULL)
		{
			//链表为空
			pNode = (PSTVINMILEAGELINK)(g_pMileageRankMem + iRankNum * sizeof(STVINMILEAGELINK));
			if (NULL == pNode)
				return;

			pNode->pPre = NULL;
			pNode->pNext = NULL;
			pLast = pNode;

			pLast->iMileage = m_pVehicleRec[i]->F1_4;

			memcpy(pLast->chVin, m_chVin[i], (VIN_LENGTH));
			iRankNum += 1;
		}
		else if (iRankNum < NUM_MILEAGERANK)
		{
			//链表结点数未满NUM_MILEAGERANK，插入新结点
			PSTVINMILEAGELINK pPrev = pLast;
			while (pPrev != NULL)
			{
				//与链表每个节点比较，从末尾开始比
				if (iValue <= pPrev->iMileage)
				{
					PSTVINMILEAGELINK pNew = (PSTVINMILEAGELINK)(g_pMileageRankMem + iRankNum * sizeof(STVINMILEAGELINK));
					pNew->iMileage = iValue;
					memcpy(pNew->chVin, m_chVin[i], (VIN_LENGTH));

					pNew->pNext = NULL;
					if (pPrev->pNext != NULL)
					{
						pPrev->pNext->pPre = pNew;
						pNew->pNext = pPrev->pNext;
					}
					else
					{
						//新结点插入链尾
						pLast = pNew;
					}

					pNew->pPre = pPrev;
					pPrev->pNext = pNew;

					iRankNum += 1;
					break;
				}

				pPrev = pPrev->pPre;
			}

			if (pPrev == NULL)
			{
				//排名最大
				PSTVINMILEAGELINK pNew = (PSTVINMILEAGELINK)(g_pMileageRankMem + iRankNum * sizeof(STVINMILEAGELINK));
				pNew->iMileage = iValue;
				memcpy(pNew->chVin, m_chVin[i], (VIN_LENGTH));
				pNew->pPre = NULL;
				pNew->pNext = pNode;
				pNode->pPre = pNew;
				pNode = pNew;
				iRankNum += 1;
			}
		}
		else
		{
			//链表结点数已达到NUM_MILEAGERANK，移除尾结点

			if (iValue <= pLast->iMileage)	//比不过最小的，排不上名
				continue;
			
			PSTVINMILEAGELINK pPrev = pLast;
			pPrev = pPrev->pPre;	//不比最小的更小，从倒数第二个结点开始比较

			while (pPrev != NULL)
			{
				//与链表每个节点比较，从末尾开始比
				if (iValue <= pPrev->iMileage)
				{
					//清空链尾结点内存，放入新结点内容
					PSTVINMILEAGELINK pNew = pLast;

					pLast = pLast->pPre;
					pLast->pNext = NULL;

					memset(pNew, 0, sizeof(STVINMILEAGELINK));
					pNew->iMileage = iValue;
					memcpy(pNew->chVin, m_chVin[i], (VIN_LENGTH));

					if (pPrev->pNext != NULL)
					{
						pPrev->pNext->pPre = pNew;
						pNew->pNext = pPrev->pNext;
					}
					else
					{
						//新结点插入链尾
						pLast = pNew;
					}

					pNew->pPre = pPrev;
					pPrev->pNext = pNew;

					break;
				}

				pPrev = pPrev->pPre;
			}

			if (pPrev == NULL)
			{
				//排名最大
				PSTVINMILEAGELINK pNew = pLast;//(PSTVINMILEAGELINK)(g_pMileageRankMem + (NUM_MILEAGERANK - 1) * sizeof(STVINMILEAGELINK));
				pLast = pLast->pPre;

				memset(pNew, 0, sizeof(STVINMILEAGELINK));
				pNew->iMileage = iValue;
				memcpy(pNew->chVin, m_chVin[i], (VIN_LENGTH));

				pNew->pPre = NULL;
				pNew->pNext = pNode;
				pNode->pPre = pNew;
				pNode = pNew;

				pLast->pNext = NULL;
			}
		}
	}

	UINT iInertNum = 0;
	while (pNode != NULL)
	{
		memcpy(msgSeq.stNode[iInertNum].chVin, pNode->chVin, VIN_LENGTH);
		msgSeq.stNode[iInertNum].iMileage = pNode->iMileage;
		pNode = pNode->pNext;

		iInertNum++;
	}
}

void CInfoRecord::OnAlert(STALERTDATA &stData)
{
	for (UINT i = 0; i < m_vehicleNum; i++)
	{
		stData.iAlertTimes[i] = m_alertTimes[i][0];
// 		stData.iVoltageException[i] = m_voltageException[i];
// 		stData.iTempException[i] = m_tempException[i];
	}
}

void CInfoRecord::OnVoltage(STVOLTAGEDATA &stData)
{
//	memcpy(&stData, m_voltageValueTimes, sizeof(STVOLTAGEDATA));
}

void CInfoRecord::OnWarning(STWARNINGDATA &stData)
{
	memcpy(&stData, m_warningValueTimes, sizeof(STWARNINGDATA));
}

bool CInfoRecord::QueryLatestInfo(UCHAR pVin[], STRECDATA &stData, UINT alertTimes[])
{
	WaitForSingleObject(g_hMutex, INFINITE);
	INT iPos = FindVinPos(pVin);
	if (iPos < 0 || NULL == m_pVehicleRec[iPos])
	{
		ReleaseMutex(g_hMutex);
		return false;
	}

	UCHAR iIndex = m_iCurRecIndex[iPos];
	stData = *m_pVehicleRec[iPos];

	memcpy(alertTimes, &m_alertTimes[iPos], ALERT_CATEGORY_NUM*sizeof(UINT));
	ReleaseMutex(g_hMutex);

	return true;
}

void CInfoRecord::QueryHistoryInfo(UCHAR pVin[], STRECDATA arrRec[], UCHAR& iIndex)
{
	WaitForSingleObject(g_hMutex, INFINITE);
	INT iPos = FindVinPos(pVin);
	if (iPos < 0)
	{
		ReleaseMutex(g_hMutex);
		return;
	}

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

void CInfoRecord::MoveToYestoday()
{
	UINT iJoin = 0;
	UINT iChargeNum = 0;
	for (UINT i = 0; i < m_vehicleNum; i++)
	{
		if (NULL == m_pVehicleRec[i])
		{
			continue;
		}

		if (m_bTodayJoin[i])
		{
			iJoin += 1;
		}

		if (m_pVehicleRec[i]->F1_1 == 1 || m_pVehicleRec[i]->F1_1 == 2)
		{
			iChargeNum += 1;
		}
	}

	CRecordYestoday::GetInstance()->OnReset(iJoin, iChargeNum, m_chargeTimes, m_chargeSeconds);
	PostMessage(m_hWndMain, UM_YESTODAYREC, NULL, 0);
}

void CInfoRecord::MoveToLastWeek()
{
	SYSTEMTIME stPre;
	stPre.wYear = g_datePre.wYear;
	stPre.wMonth = g_datePre.wMonth;
	stPre.wDay = g_datePre.wDay;
	stPre.wHour = 23;
	stPre.wMinute = 59;
	stPre.wSecond = 59;
	time_t tPre = ConvertSecondsTime(stPre);

	UINT iOnline = 0;
	UINT iOffline = 0;
	UINT iFault = 0;
	UINT iRecharge = 0;
	UINT iUnlocated = 0;

	for (UINT i = 0; i < m_vehicleNum; i++)
	{
		if (NULL == m_pVehicleRec[i])
		{
			continue;
		}

		if (m_pVehicleRec[i]->F1_1 == 1 || m_pVehicleRec[i]->F1_1 == 2)
		{
			iRecharge += 1;
		}

		if (m_pVehicleRec[i]->F5_0 == 0 && m_pVehicleRec[i]->F5_1 == 0)
		{
			iUnlocated += 1;
		}

		if (m_pVehicleRec[i]->F7_0 > 0)
		{
			iFault += 1;
		}

		if (m_bTodayJoin[i])
		{
			time_t tRec = ConvertSecondsTime(m_sTime[i]);

			if ((tPre >= tRec) && (tPre - tRec <= TIME_OFFLINE_DISTANCE))
			{
				iOnline += 1;
			}
			else
			{
				iOffline += 1;
			}
		}
	}

	CRecordLastweek::GetInstance()->OnReset(iOnline, iOffline, iFault, iRecharge, iUnlocated);
	PostMessage(m_hWndMain, UM_LASTWEEKREC, NULL, 0);
}