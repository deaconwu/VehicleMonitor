// CStatistics.cpp: 实现文件
//

#include "pch.h"
#include "MsgHandle.h"
#include "CStatistics.h"
#include "afxdialogex.h"
#include "CInfoRecord.h"
#include "UserMessage.h"

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

	return TRUE;
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

DWORD WINAPI OnStatisThread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	while (1)
	{
		if (g_bStopFlag)
		{
			break;
		}

		STSTATISTICDATA stData;
		memset(&stData, 0, sizeof(stData));

		CInfoRecord::GetInstance()->OnStatistic(stData);

		SendMessage(hWnd, UM_STATISTIC, (WPARAM)&stData, 0);

		Sleep(1000);
	}

	return 0;
}

BEGIN_MESSAGE_MAP(CStatistics, CDialogEx)
	ON_MESSAGE(UM_STATISTIC, &CStatistics::OnGetData)
END_MESSAGE_MAP()


// CStatistics 消息处理程序
LRESULT CStatistics::OnGetData(WPARAM wParam, LPARAM lParam)
{
	STSTATISTICDATA* pMsg = (STSTATISTICDATA*)wParam;

	CString csStr;

	//累计行驶里程
	csStr = _T("");
	csStr.Format(_T("%.5f"), (long double)pMsg->iMileageSum / 10 / 10000);
	((CEdit*)GetDlgItem(IDC_EDIT_MILEAGESUN))->SetWindowText(csStr);

	//今日充电车辆数
	csStr = _T("");
	csStr.Format(_T("%lu"), pMsg->iRechargeToday);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLERECHARGETODAY))->SetWindowText(csStr);

	//今日故障车辆数
	csStr = _T("");
	csStr.Format(_T("%lu"), pMsg->iDefaultToday);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEFAULTTODAY))->SetWindowText(csStr);

	//今日在线车辆数
	csStr = _T("");
	csStr.Format(_T("%lu"), pMsg->iOnlineToday);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEONLINETODAY))->SetWindowText(csStr);

	//今日离线车辆数
	csStr = _T("");
	csStr.Format(_T("%lu"), pMsg->iOfflineToday);
	((CEdit*)GetDlgItem(IDC_EDIT_VEHICLEOFFLINETODAY))->SetWindowText(csStr);

	//接入车辆数
	csStr = _T("");
	csStr.Format(_T("%lu"), pMsg->iSumToday);
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