// CVoltageData.cpp: 实现文件
//

#include "pch.h"
#include "MsgHandle.h"
#include "CVoltageData.h"
#include "afxdialogex.h"
#include "CInfoRecord.h"
#include "UserMessage.h"
#include "CVoltageRank.h"

static UINT g_nNumLoaded = 0;
static UINT g_nVehicleNum = 0;
static bool g_bStopFlag = true;

DWORD WINAPI OnLoadVehicleThread(LPVOID lparam);

DWORD WINAPI OnVoltageThread(LPVOID lparam);

// CVoltageData 对话框

IMPLEMENT_DYNAMIC(CVoltageData, CDialogEx)

CVoltageData::CVoltageData(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VOLTAGE, pParent)
{

}

CVoltageData::~CVoltageData()
{
}

BOOL CVoltageData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listVins.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listVins.InsertColumn(1, _T("Vin码"), LVCFMT_CENTER, 130);
	m_listVins.InsertColumn(2, _T("0"), LVCFMT_CENTER, 50);
	m_listVins.InsertColumn(3, _T("1-2650"), LVCFMT_CENTER, 60);
	m_listVins.InsertColumn(4, _T("2651-4200"), LVCFMT_CENTER, 65);
	m_listVins.InsertColumn(5, _T("4201-15000"), LVCFMT_CENTER, 70);
	m_listVins.InsertColumn(6, _T("15001-65533"), LVCFMT_CENTER, 80);
	m_listVins.InsertColumn(7, _T("65534"), LVCFMT_CENTER, 50);
	m_listVins.InsertColumn(8, _T("65535"), LVCFMT_CENTER, 50);

	return TRUE;
}

void CVoltageData::LoadList(HWND hWnd)
{
	m_hWndMain = hWnd;
	//UCHAR chVin[MAX_VEHICLENUM][VIN_LENGTH + 1] = {};
	g_nVehicleNum = CInfoRecord::GetInstance()->GetVehicleNum();

	UINT nLoadNum = NUM_VINLOADED_FIRST;
	UINT nLeftNum = g_nVehicleNum - nLoadNum;
	if (g_nVehicleNum <= NUM_VINLOADED_PERTIME)
	{
		nLoadNum = g_nVehicleNum;
	}

	for (UINT i = 0; i < nLoadNum; i++)
	{
		//CString csStr(chVin[i]);
		m_listVins.InsertItem(i, _T(""));
		//m_listVins.SetItemText(i, 0, csStr);
		m_listVins.SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
	}

	g_nNumLoaded = nLoadNum;

	if (nLeftNum > 0)
	{
		UINT nThreadCount = nLeftNum / NUM_VINLOADED_PERTIME + 1;
		for (UINT i = 0; i < nThreadCount; i++)
		{
			DWORD dwThreadId;
			HANDLE hThread = CreateThread(NULL, NULL, OnLoadVehicleThread, this->m_hWnd, 0, &dwThreadId);
		}
	}
}

void CVoltageData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VINS, m_listVins);
}

void CVoltageData::OnLauch()
{
	g_bStopFlag = false;

	CVoltageRank::GetInstance()->OnLaunchRank(this->m_hWnd);

// 	DWORD exitCode;
// 	BOOL ret = ::GetExitCodeThread(m_hThread, &exitCode);
// 	if (exitCode != STILL_ACTIVE)
// 	{
// 		CloseHandle(m_hThread);
// 		m_hThread = NULL;
// 
// 		DWORD dwThreadId;
// 		m_hThread = CreateThread(NULL, NULL, OnVoltageThread, this->m_hWnd, 0, &dwThreadId);
// 	}
}

void CVoltageData::OnDestroy()
{
	g_bStopFlag = true;
	CVoltageRank::GetInstance()->OnStopRank();
}


DWORD WINAPI OnLoadVehicleThread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	PostMessage(hWnd, UM_LOADVINS, NULL, 0);

	return 0;
}

DWORD WINAPI OnVoltageThread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	while (1)
	{
		if (g_bStopFlag)
		{
			break;
		}

		STVOLTAGEDATA stData;
		memset(&stData, 0, sizeof(stData));

		CInfoRecord::GetInstance()->OnVoltage(stData);

		SendMessage(hWnd, UM_VOLTAGE, (WPARAM)&stData, 0);

		Sleep(1000);
	}

	return 0;
}


BEGIN_MESSAGE_MAP(CVoltageData, CDialogEx)
	ON_MESSAGE(UM_VOLTAGERANK, &CVoltageData::OnGetData)
	ON_MESSAGE(UM_LOADVINS, &CVoltageData::OnLoadVehicle)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_VINS, &CVoltageData::OnLvnColumnclickListVins)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_VINS, &CVoltageData::OnLvnItemchangedListVins)
END_MESSAGE_MAP()


// CVoltageData 消息处理程序
LRESULT CVoltageData::OnGetData(WPARAM wParam, LPARAM lParam)
{
	STMSGVOLTAGERANKSEQ* pMsg = (STMSGVOLTAGERANKSEQ*)wParam;
	//LockWindowUpdate();
	for (UINT i = 0; i < pMsg->iNum; i++)
	{
		if (g_bStopFlag)
		{
			break;
		}

		CString csStr(pMsg->stNode[i].chVin);
		m_listVins.SetItemText(i, 0, csStr);

		csStr = _T("");
		csStr.Format(_T("%u"), pMsg->stNode[i].iValueTimes[0]);
		m_listVins.SetItemText(i, 1, csStr);

		csStr = _T("");
		csStr.Format(_T("%u"), pMsg->stNode[i].iValueTimes[1]);
		m_listVins.SetItemText(i, 2, csStr);

		csStr = _T("");
		csStr.Format(_T("%u"), pMsg->stNode[i].iValueTimes[2]);
		m_listVins.SetItemText(i, 3, csStr);

		csStr = _T("");
		csStr.Format(_T("%u"), pMsg->stNode[i].iValueTimes[3]);
		m_listVins.SetItemText(i, 4, csStr);

		csStr = _T("");
		csStr.Format(_T("%u"), pMsg->stNode[i].iValueTimes[4]);
		m_listVins.SetItemText(i, 5, csStr);

		csStr = _T("");
		csStr.Format(_T("%u"), pMsg->stNode[i].iValueTimes[5]);
		m_listVins.SetItemText(i, 6, csStr);

		csStr = _T("");
		csStr.Format(_T("%u"), pMsg->stNode[i].iValueTimes[6]);
		m_listVins.SetItemText(i, 7, csStr);
	}
	//UnlockWindowUpdate();

	return 0;
}

LRESULT CVoltageData::OnLoadVehicle(WPARAM wParam, LPARAM lParam)
{
	UINT iStartIndex = g_nNumLoaded;
	UINT iEndIndex = g_nNumLoaded + NUM_VINLOADED_PERTIME;
	if (iEndIndex > g_nVehicleNum)
	{
		iEndIndex = g_nVehicleNum;
	}

	for (UINT i = iStartIndex; i < iEndIndex; i++)
	{
		m_listVins.InsertItem(i, _T(""));
		//m_listVins.SetItemText(i, 0, csStr);
		m_listVins.SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
	}

	g_nNumLoaded = iEndIndex;

	return 0;
}

void CVoltageData::OnLvnColumnclickListVins(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CVoltageRank::GetInstance()->NotifyType(pNMLV->iSubItem - 1);
}


void CVoltageData::OnLvnItemchangedListVins(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	if (LVIF_STATE != pNMLV->uChanged || (pNMLV->uNewState != 3))
		return;

	if (g_bStopFlag)
	{
		return;
	}

	CString strVin = L"";
	int count = m_listVins.GetItemCount();
	for (int i = 0; i < m_listVins.GetItemCount(); i++)
	{
		if (m_listVins.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			//str.Format(_T("选中了第%d行"), i);
			//AfxMessageBox(str);
			//m_listRank.SetCheck(i, FALSE);
			m_listVins.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
			strVin = m_listVins.GetItemText(i, 0);
			if (strVin.IsEmpty())
				return;

			char chVin[VIN_LENGTH] = {};

			int iLength = WideCharToMultiByte(CP_ACP, 0, strVin, -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, strVin, -1, chVin, iLength, NULL, NULL);

			::SendMessage(m_hWndMain, UM_QUERYVINS, (WPARAM)chVin, 0);

			return;
		}
	}

	*pResult = 0;
}
