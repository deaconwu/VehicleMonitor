// CAlertData.cpp: 实现文件
//

#include "pch.h"
#include "MsgHandle.h"
#include "CAlertData.h"
#include "afxdialogex.h"
#include "CInfoRecord.h"
#include "UserMessage.h"

UCHAR g_chVin[MAX_VEHICLENUM][VIN_LENGTH] = {};
static UINT g_nNumLoaded = 0;
static UINT g_nVehicleNum = 0;
static bool g_bStopFlag = false;

DWORD WINAPI OnLoadVinThread(LPVOID lparam);
DWORD WINAPI OnAlertThread(LPVOID lparam);

// CAlertData 对话框

IMPLEMENT_DYNAMIC(CAlertData, CDialogEx)

CAlertData::CAlertData(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ALERTDATA, pParent)
{

}

CAlertData::~CAlertData()
{
}


BOOL CAlertData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listVins.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listVins.InsertColumn(1, _T("Vin码"), LVCFMT_CENTER, 130);
	m_listVins.InsertColumn(2, _T("报警"), LVCFMT_CENTER, 60);
	//m_listVins.InsertColumn(3, _T("电压65534"), LVCFMT_CENTER, 70);
	m_listVins.InsertColumn(3, _T("温度254"), LVCFMT_CENTER, 60);

	return TRUE;
}

void CAlertData::LoadList()
{
	g_nVehicleNum = CInfoRecord::GetInstance()->GetVins(g_chVin);
	
	UINT nLoadNum = NUM_VINLOADED_FIRST;
	INT nLeftNum = g_nVehicleNum - nLoadNum;
	if (g_nVehicleNum <= NUM_VINLOADED_PERTIME)
	{
		nLoadNum = g_nVehicleNum;
	}

	for (UINT i = 0; i < nLoadNum; i++)
	{
		CString csStr(g_chVin[i]);
		//csStr.Format(_T("%s"), chVin[i]);
		m_listVins.InsertItem(i, csStr);
		m_listVins.SetItemText(i, 0, csStr);
		//m_listVins.SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
	}

	g_nNumLoaded = nLoadNum;

	if (nLeftNum > 0)
	{
		UINT nThreadCount = nLeftNum / NUM_VINLOADED_PERTIME + 1;
		for (UINT i=0; i<nThreadCount; i++)
		{
			DWORD dwThreadId;
			HANDLE hThread = CreateThread(NULL, NULL, OnLoadVinThread, this->m_hWnd, 0, &dwThreadId);
		}
	}
}

void CAlertData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VINS, m_listVins);
}

void CAlertData::OnLauch()
{
	g_bStopFlag = false;

	DWORD exitCode;
	BOOL ret = ::GetExitCodeThread(m_hThread, &exitCode);
	if (exitCode != STILL_ACTIVE)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;

		DWORD dwThreadId;
		m_hThread = CreateThread(NULL, NULL, OnAlertThread, this->m_hWnd, 0, &dwThreadId);
	}
}

void CAlertData::OnDestroy()
{
	g_bStopFlag = true;
}

DWORD WINAPI OnAlertThread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	while (1)
	{
		if (g_bStopFlag)
		{
			break;
		}

		STALERTDATA stData;
		memset(&stData, 0, sizeof(stData));

		CInfoRecord::GetInstance()->OnAlert(stData);

		SendMessage(hWnd, UM_ALERT, (WPARAM)&stData, 0);

		Sleep(1000);
	}

	return 0;
}

DWORD WINAPI OnLoadVinThread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	PostMessage(hWnd, UM_LOADVINS, NULL, 0);

	return 0;
}

BEGIN_MESSAGE_MAP(CAlertData, CDialogEx)
	ON_MESSAGE(UM_ALERT, &CAlertData::OnGetData)
	ON_MESSAGE(UM_LOADVINS, &CAlertData::OnLoadVin)
END_MESSAGE_MAP()


// CAlertData 消息处理程序
LRESULT CAlertData::OnGetData(WPARAM wParam, LPARAM lParam)
{
	STALERTDATA* pMsg = (STALERTDATA*)wParam;

	CString csStr;
	for (UINT i = 0; i < g_nVehicleNum; i++)
	{
		csStr = _T("");
		csStr.Format(_T("%u"), pMsg->iAlertTimes[i]);
		m_listVins.SetItemText(i, 1, csStr);

// 		csStr = _T("");
// 		csStr.Format(_T("%u"), pMsg->iVoltageException[i]);
// 		m_listVins.SetItemText(i, 2, csStr);

		csStr = _T("");
		csStr.Format(_T("%u"), pMsg->iTempException[i]);
		m_listVins.SetItemText(i, 2, csStr);
	}

	return 0;
}

LRESULT CAlertData::OnLoadVin(WPARAM wParam, LPARAM lParam)
{
	UINT iStartIndex = g_nNumLoaded;
	UINT iEndIndex = g_nNumLoaded + NUM_VINLOADED_PERTIME;
	if (iEndIndex > g_nVehicleNum)
	{
		iEndIndex = g_nVehicleNum;
	}

	for (UINT i = iStartIndex; i < iEndIndex; i++)
	{
		CString csStr(g_chVin[i]);

		m_listVins.InsertItem(i, csStr);
		m_listVins.SetItemText(i, 0, csStr);
		//m_listVins.SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
	}

	g_nNumLoaded = iEndIndex;

	return 0;
}