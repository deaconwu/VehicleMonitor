// CStatistics.cpp: 实现文件
//

#include "pch.h"
#include "MsgHandle.h"
#include "CStatistics.h"
#include "afxdialogex.h"
#include "CInfoRecord.h"
#include "UserMessage.h"
#include "CRecordYestoday.h"
#include "CRecordLastweek.h"

static bool g_bStopFlag = false;
DWORD WINAPI OnStatisThread(LPVOID lparam);

// CStatistics 对话框

IMPLEMENT_DYNAMIC(CStatistics, CDialogEx)

CStatistics::CStatistics(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STATISTICS, pParent)
{
	m_hThread = NULL;
}

CStatistics::~CStatistics()
{
}

void CStatistics::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CStatistics::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLESUMTODAY))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEONLINETODAY))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEOFFLINETODAY))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEFAULTTODAY))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGETODAY))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_MILEAGESUN))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_CARBONEMISSION))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_FUELTHRIFT))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_POWERCONSUME))->EnableWindow(false);

	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLESUMYESTODAY))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGEYESTODAY))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGEYETIMESYESTODAY))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGESECYESTODAY))->EnableWindow(false);

	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEONLINELASTWEEK))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEOFFLINELASTWEEK))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEUNLOCATEDLASTWEEK))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEFAULTLASTWEEK))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGELASTWEEK))->EnableWindow(false);

	((CEdit*)GetDlgItem(IDC_EDIT_MILEAGES1))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_MILEAGES2))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_MILEAGES3))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_MILEAGES4))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_MILEAGES5))->EnableWindow(false);

	LoadPreRec();

	return TRUE;
}

void CStatistics::LoadPreRec()
{
	FILE *pF = fopen("RecYestoday.dat", "rb");
	if (NULL == pF)
	{
		return;
	}

	char buf[BUFFER_SIZE] = {};
	CString csStr;

	fseek(pF, 0, SEEK_END);
	long len = ftell(pF);
	fseek(pF, 0, SEEK_SET);
	fread(buf, 1, len, pF);
	fclose(pF);

	const char* pValue = (char*)strtok(buf, ",");
	if (pValue == NULL)
		return;

	UINT iJoin = atoi(pValue);
	csStr = _T("");
	csStr.Format(_T("%u"), iJoin);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLESUMYESTODAY))->SetWindowText(csStr);

	pValue = (char*)strtok(NULL, ",");
	UINT iRechargeNum = atoi(pValue);
	csStr = _T("");
	csStr.Format(_T("%u"), iRechargeNum);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGEYESTODAY))->SetWindowText(csStr);

	pValue = (char*)strtok(NULL, ",");
	UINT iRechargeTimes = atoi(pValue);
	csStr = _T("");
	csStr.Format(_T("%u"), iRechargeTimes);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGEYETIMESYESTODAY))->SetWindowText(csStr);

	pValue = (char*)strtok(NULL, ",");
	ULONGLONG iRechargeSeconds = atoll(pValue);
	csStr = _T("");
	csStr.Format(_T("%llu"), iRechargeSeconds);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGESECYESTODAY))->SetWindowText(csStr);

	pF = fopen("RecLastWeek.dat", "rb");
	if (NULL == pF)
	{
		return;
	}

	memset(buf, 0, sizeof(buf));
	fseek(pF, 0, SEEK_END);
	len = ftell(pF);
	fseek(pF, 0, SEEK_SET);
	fread(buf, 1, len, pF);
	fclose(pF);

	pValue = (char*)strtok(buf, ",");
	if (pValue == NULL)
		return;

	UINT iOnline = atoi(pValue);
	csStr = _T("");
	csStr.Format(_T("%u"), iOnline);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEONLINELASTWEEK))->SetWindowText(csStr);

	pValue = (char*)strtok(NULL, ",");
	UINT iOffline = atoi(pValue);
	csStr = _T("");
	csStr.Format(_T("%u"), iOffline);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEOFFLINELASTWEEK))->SetWindowText(csStr);

	pValue = (char*)strtok(NULL, ",");
	UINT iFault = atoi(pValue);
	csStr = _T("");
	csStr.Format(_T("%u"), iFault);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEFAULTLASTWEEK))->SetWindowText(csStr);
	
	pValue = (char*)strtok(NULL, ",");
	UINT iRecharge = atoi(pValue);
	csStr = _T("");
	csStr.Format(_T("%u"), iRecharge);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGELASTWEEK))->SetWindowText(csStr);

	pValue = (char*)strtok(NULL, ",");
	UINT iUnlocated = atoi(pValue);
	csStr = _T("");
	csStr.Format(_T("%u"), iUnlocated);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEUNLOCATEDLASTWEEK))->SetWindowText(csStr);
}

void CStatistics::OnLauch()
{
	g_bStopFlag = false;

	DWORD exitCode;
	BOOL ret = ::GetExitCodeThread(m_hThread, &exitCode);
	if (exitCode != STILL_ACTIVE)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;

		DWORD dwThreadId;
		m_hThread = CreateThread(NULL, NULL, OnStatisThread, this->m_hWnd, 0, &dwThreadId);
	}
}

void CStatistics::OnDestroy()
{
	g_bStopFlag = true;
}

void CStatistics::OnYestodayRec()
{
	STSTATISTICDATAYESTODAY stData = {};

	CRecordYestoday::GetInstance()->OnFetch(stData);

	CString csStr;

	csStr = _T("");
	csStr.Format(_T("%u"), stData.iJoin);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLESUMYESTODAY))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), stData.iRechargeNum);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGEYESTODAY))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), stData.iRechargeTimes);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGEYETIMESYESTODAY))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%llu"), stData.iRechargeSeconds);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGESECYESTODAY))->SetWindowText(csStr);
}

void CStatistics::OnLastWeekRec()
{
	STSTATISTICDATALASTWEEK stData = {};

	CRecordLastweek::GetInstance()->OnFetch(stData);

	CString csStr;

	csStr = _T("");
	csStr.Format(_T("%u"), stData.iOnline);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEONLINELASTWEEK))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), stData.iOffline);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEOFFLINELASTWEEK))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), stData.iUnlocated);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEUNLOCATEDLASTWEEK))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), stData.iFault);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEFAULTLASTWEEK))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), stData.iRecharge);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGELASTWEEK))->SetWindowText(csStr);
}

DWORD WINAPI OnStatisThread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	while (1)
	{
		if (g_bStopFlag)
		{
			break;
		}

		STSTATISTICDATATODAY stData = {};
		CInfoRecord::GetInstance()->OnStatisticToday(stData);
		SendMessage(hWnd, UM_STATISTIC, (WPARAM)&stData, 0);

// 		STMSGMILEAGERANKSEQ stRank = {};
// 		CInfoRecord::GetInstance()->OnMileageRank(stRank);
// 		SendMessage(hWnd, UM_MILEAGERANK, (WPARAM)&stRank, 0);

		Sleep(1000);
	}

	return 0;
}

BEGIN_MESSAGE_MAP(CStatistics, CDialogEx)
	ON_MESSAGE(UM_STATISTIC, &CStatistics::OnGetData)
	ON_MESSAGE(UM_MILEAGERANK, &CStatistics::OnGetMileageRank)
END_MESSAGE_MAP()


// CStatistics 消息处理程序
LRESULT CStatistics::OnGetData(WPARAM wParam, LPARAM lParam)
{
	STSTATISTICDATATODAY* pMsg = (STSTATISTICDATATODAY*)wParam;

	CString csStr;

	//累计行驶里程
	csStr = _T("");
	csStr.Format(_T("%.5f"), (long double)pMsg->iMileageSum / 10 / 10000);
	((CEdit*)GetDlgItem(IDC_EDIT_MILEAGESUN))->SetWindowText(csStr);

	//今日充电车辆数
	csStr = _T("");
	csStr.Format(_T("%lu"), pMsg->iRecharge);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGETODAY))->SetWindowText(csStr);

	//今日故障车辆数
	csStr = _T("");
	csStr.Format(_T("%lu"), pMsg->iFault);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEFAULTTODAY))->SetWindowText(csStr);

	//今日在线车辆数
	csStr = _T("");
	csStr.Format(_T("%lu"), pMsg->iOnline);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEONLINETODAY))->SetWindowText(csStr);

	//今日离线车辆数
	csStr = _T("");
	csStr.Format(_T("%lu"), pMsg->iOffline);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEOFFLINETODAY))->SetWindowText(csStr);

	//接入车辆数
	csStr = _T("");
	csStr.Format(_T("%lu"), pMsg->iJoin);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLESUMTODAY))->SetWindowText(csStr);

	//累计碳减排
	csStr = _T("");
	csStr.Format(_T("%.4f"), (0.0031 / 6) * ((long double)pMsg->iMileageSum / 10 / 10000));
	//csStr.Append(_T(" 万升"));
	((CEdit*)GetDlgItem(IDC_EDIT_CARBONEMISSION))->SetWindowText(csStr);

	//累计节油量
	csStr = _T("");
	csStr.Format(_T("%.2f"), (1.75 / 6) * ((long double)pMsg->iMileageSum / 10 / 10000));
	//csStr.Append(_T(" 万升"));
	((CEdit*)GetDlgItem(IDC_EDIT_FUELTHRIFT))->SetWindowText(csStr);

	//累计耗电量
	csStr = _T("");
	csStr.Format(_T("%.2f"), (5.68 / 6) * ((long double)pMsg->iMileageSum / 10 / 10000));
	//csStr.Append(_T(" 万千瓦时"));
	((CEdit*)GetDlgItem(IDC_EDIT_POWERCONSUME))->SetWindowText(csStr);

	return 0;
}

LRESULT CStatistics::OnGetMileageRank(WPARAM wParam, LPARAM lParam)
{
	STMSGMILEAGERANKSEQ* pMsg = (STMSGMILEAGERANKSEQ*)wParam;

	CString csStr;
	TCHAR tchVin[VIN_LENGTH + 1] = {};

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, (char*)(pMsg->stNode[0].chVin), -1, tchVin, VIN_LENGTH);
#else
	strcpy(tchVin, pMsg->stNode[0].chVin);
#endif
	((CStatic*)GetDlgItem(IDC_STATICVIN1))->SetWindowText(tchVin);

	csStr = _T("");
	csStr.Format(_T("%.1f"), (long double)pMsg->stNode[0].iMileage/10);
	((CStatic*)GetDlgItem(IDC_EDIT_MILEAGES1))->SetWindowText(csStr);

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, (char*)(pMsg->stNode[1].chVin), -1, tchVin, VIN_LENGTH);
#else
	strcpy(tchVin, pMsg->stNode[1].chVin);
#endif
	((CStatic*)GetDlgItem(IDC_STATICVIN2))->SetWindowText(tchVin);

	csStr = _T("");
	csStr.Format(_T("%.1f"), (long double)pMsg->stNode[1].iMileage/10);
	((CStatic*)GetDlgItem(IDC_EDIT_MILEAGES2))->SetWindowText(csStr);

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, (char*)(pMsg->stNode[2].chVin), -1, tchVin, VIN_LENGTH);
#else
	strcpy(tchVin, pMsg->stNode[2].chVin);
#endif
	((CStatic*)GetDlgItem(IDC_STATICVIN3))->SetWindowText(tchVin);

	csStr = _T("");
	csStr.Format(_T("%.1f"), (long double)pMsg->stNode[2].iMileage/10);
	((CStatic*)GetDlgItem(IDC_EDIT_MILEAGES3))->SetWindowText(csStr);

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, (char*)(pMsg->stNode[3].chVin), -1, tchVin, VIN_LENGTH);
#else
	strcpy(tchVin, pMsg->stNode[3].chVin);
#endif
	((CStatic*)GetDlgItem(IDC_STATICVIN4))->SetWindowText(tchVin);

	csStr = _T("");
	csStr.Format(_T("%.1f"), (long double)pMsg->stNode[3].iMileage/10);
	((CStatic*)GetDlgItem(IDC_EDIT_MILEAGES4))->SetWindowText(csStr);

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, (char*)(pMsg->stNode[4].chVin), -1, tchVin, VIN_LENGTH);
#else
	strcpy(tchVin, pMsg->stNode[4].chVin);
#endif
	((CStatic*)GetDlgItem(IDC_STATICVIN5))->SetWindowText(tchVin);

	csStr = _T("");
	csStr.Format(_T("%.1f"), (long double)pMsg->stNode[4].iMileage/10);
	((CStatic*)GetDlgItem(IDC_EDIT_MILEAGES5))->SetWindowText(csStr);
	
	return 0;
}