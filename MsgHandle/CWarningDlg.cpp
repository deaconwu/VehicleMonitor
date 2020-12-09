// CWarningDlg.cpp: 实现文件
//

#include "pch.h"
#include "MsgHandle.h"
#include "CWarningDlg.h"
#include "afxdialogex.h"
#include "CInfoRecord.h"
#include "UserMessage.h"
#include "CWarningRank.h"

static UINT g_nNumLoaded = 0;
static UINT g_nVehicleNum = 0;
static bool g_bStopFlag = true;

DWORD WINAPI OnLoadCarThread(LPVOID lparam);

// CWarningDlg 对话框

IMPLEMENT_DYNAMIC(CWarningDlg, CDialogEx)

CWarningDlg::CWarningDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WARNING, pParent)
{
	m_pDlgHisRec = NULL;
}

CWarningDlg::~CWarningDlg()
{
}

BOOL CWarningDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listVins.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listVins.InsertColumn(1, _T("Vin码"), LVCFMT_CENTER, 130);
	m_listVins.InsertColumn(2, _T("欠压"), LVCFMT_CENTER, 60);
	m_listVins.InsertColumn(3, _T("过压"), LVCFMT_CENTER, 60);
	m_listVins.InsertColumn(4, _T("65534"), LVCFMT_CENTER, 60);
	m_listVins.InsertColumn(5, _T("过温"), LVCFMT_CENTER, 60);
	m_listVins.InsertColumn(6, _T("254"), LVCFMT_CENTER, 60);
	m_listVins.InsertColumn(7, _T("电压国警"), LVCFMT_CENTER, 60);
	m_listVins.InsertColumn(8, _T("温度国警"), LVCFMT_CENTER, 60);

	return TRUE;
}

void CWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VINS, m_listVins);
}

void CWarningDlg::LoadList(HWND hWnd)
{
	m_hWndMain = hWnd;
	//UCHAR chVin[MAX_VEHICLENUM][VIN_LENGTH] = {};

	/*
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
			HANDLE hThread = CreateThread(NULL, NULL, OnLoadCarThread, this->m_hWnd, 0, &dwThreadId);
		}
	}*/
}

void CWarningDlg::OnLauch()
{
	g_bStopFlag = false;

	CWarningRank::GetInstance()->OnLaunchRank(this->m_hWnd);
}

void CWarningDlg::OnDestroy()
{
	g_bStopFlag = true;
	CWarningRank::GetInstance()->OnStopRank();

	if (NULL != m_pDlgHisRec)
	{
		delete m_pDlgHisRec;
		m_pDlgHisRec = NULL;
	}
}

void CWarningDlg::NotifyRefreshVin()
{
	if (NULL != m_pDlgHisRec)
	{
		m_pDlgHisRec->OnRefresh();
	}
}

DWORD WINAPI OnLoadCarThread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	PostMessage(hWnd, UM_LOADVINS, NULL, 0);

	return 0;
}

BEGIN_MESSAGE_MAP(CWarningDlg, CDialogEx)
	ON_MESSAGE(UM_WARNINGRANK, &CWarningDlg::OnGetData)
	ON_MESSAGE(UM_LOADVINS, &CWarningDlg::OnLoadVehicle)
	ON_MESSAGE(UM_DESTROYHISTORYDLG, &CWarningDlg::OnDestroyHistoryDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_VINS, &CWarningDlg::OnLvnItemchangedListVins)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_VINS, &CWarningDlg::OnLvnColumnclickListVins)
END_MESSAGE_MAP()


// CWarningDlg 消息处理程序

LRESULT CWarningDlg::OnGetData(WPARAM wParam, LPARAM lParam)
{
	STMSGWARNINGRANKSEQ* pMsg = (STMSGWARNINGRANKSEQ*)wParam;

	int count = m_listVins.GetItemCount();
	if (pMsg->iNum > count)
	{
		for (UINT i = count; i < pMsg->iNum; i++)
		{
			m_listVins.InsertItem(i, _T(""));
			//m_listVins.SetItemText(i, 0, csStr);
			m_listVins.SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
		}
	}

	m_listVins.SetRedraw(FALSE);

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

	m_listVins.SetRedraw(TRUE);

	return 0;
}

LRESULT CWarningDlg::OnLoadVehicle(WPARAM wParam, LPARAM lParam)
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

LRESULT CWarningDlg::OnDestroyHistoryDlg(WPARAM wParam, LPARAM lParam)
{
	if (NULL != m_pDlgHisRec)
	{
		delete m_pDlgHisRec;
		m_pDlgHisRec = NULL;
	}

	return 0;
}

void CWarningDlg::OnLvnColumnclickListVins(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CWarningRank::GetInstance()->NotifyType(pNMLV->iSubItem - 1);
}


void CWarningDlg::OnLvnItemchangedListVins(NMHDR *pNMHDR, LRESULT *pResult)
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

			char chVin[VIN_LENGTH+1] = {};

			int iLength = WideCharToMultiByte(CP_ACP, 0, strVin, -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, strVin, -1, chVin, iLength, NULL, NULL);

			//::SendMessage(m_hWndMain, UM_QUERYVINS, (WPARAM)chVin, 0);
			if (NULL != m_pDlgHisRec)
			{
				delete m_pDlgHisRec;
				m_pDlgHisRec = NULL;
			}

			m_pDlgHisRec = new CHistoryRecordDlg;
			m_pDlgHisRec->Create(IDD_HISTORYREC, this);
			m_pDlgHisRec->ShowWindow(SW_SHOW);
			m_pDlgHisRec->OnLaunch(strVin, chVin, this->m_hWnd);

			break;
		}
	}

	*pResult = 0;
}
