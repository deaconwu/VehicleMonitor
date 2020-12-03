// CHistoryRecordDlg.cpp: 实现文件
//

#include "pch.h"
#include "MsgHandle.h"
#include "CHistoryRecordDlg.h"
#include "afxdialogex.h"
#include "CInfoRecord.h"
#include "UserMessage.h"

static UCHAR g_chVin[VIN_LENGTH] = {};
// CHistoryRecordDlg 对话框

IMPLEMENT_DYNAMIC(CHistoryRecordDlg, CDialogEx)

CHistoryRecordDlg::CHistoryRecordDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HISTORYREC, pParent)
{
	m_hParentWnd = NULL;
}

CHistoryRecordDlg::~CHistoryRecordDlg()
{
	CInfoRecord::GetInstance()->UnSetRefreshVin();
}

void CHistoryRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listRec);
}

BOOL CHistoryRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(_T("历史记录"));

	((CEdit*)GetDlgItem(IDC_EDITVIN))->EnableWindow(false);

	m_listRec.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listRec.InsertColumn(1, _T("索引"), LVCFMT_CENTER, 60);
	m_listRec.InsertColumn(2, _T("单体电压最高值"), LVCFMT_CENTER, 120);
	m_listRec.InsertColumn(3, _T("单体电压最低值"), LVCFMT_CENTER, 120);
	m_listRec.InsertColumn(4, _T("最高温度值"), LVCFMT_CENTER, 100);
	m_listRec.InsertColumn(5, _T("最低温度值"), LVCFMT_CENTER, 100);
// 	m_listRec.InsertColumn(6, _T("电压警报数"), LVCFMT_CENTER, 100);
// 	m_listRec.InsertColumn(7, _T("温度警报数"), LVCFMT_CENTER, 100);

	CString str = L"";
	for (UINT i = 1; i <= RECNUM_PER_VEHICEL; i++)
	{
		str = L"";
		str.Format(_T("%u"), i);
		m_listRec.InsertItem(i+1, str);
		m_listRec.SetItemText(i+1, 0, str);
	}

	return TRUE;
}

void CHistoryRecordDlg::OnLaunch(CString strVin, char chVin[], HWND hWnd)
{
	((CEdit*)GetDlgItem(IDC_EDITVIN))->SetWindowText(strVin);
	memcpy(g_chVin, chVin, VIN_LENGTH);

	m_hParentWnd = hWnd;
	
	CInfoRecord::GetInstance()->SetRefreshVin((UCHAR*)chVin);

	OnRefresh();
}

void CHistoryRecordDlg::OnRefresh()
{
	STRECDATA arrRec[RECNUM_PER_VEHICEL] = {};
	UCHAR iIndex = 0;
	CInfoRecord::GetInstance()->QueryHistoryInfo(g_chVin, arrRec, iIndex);

	iIndex = (iIndex - 1 + RECNUM_PER_VEHICEL) % RECNUM_PER_VEHICEL;

	m_listRec.SetFocus();
	
	CString str = L"";
	for (UCHAR i = 0; i < RECNUM_PER_VEHICEL; i++)
	{
// 		if (i != iIndex)
// 		{
// 			m_listRec.SetItemState(i, 0, LVIS_SELECTED);
// 		}

		str = L"";
		str.Format(_T("%u"), arrRec[i].F6_1);
		m_listRec.SetItemText(i, 1, str);

		str = L"";
		str.Format(_T("%u"), arrRec[i].F6_3);
		m_listRec.SetItemText(i, 2, str);

		str = L"";
		str.Format(_T("%u"), arrRec[i].F6_5);
		m_listRec.SetItemText(i, 3, str);

		str = L"";
		str.Format(_T("%u"), arrRec[i].F6_7);
		m_listRec.SetItemText(i, 4, str);
	}

	m_listRec.SetItemState(iIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

BEGIN_MESSAGE_MAP(CHistoryRecordDlg, CDialogEx)
	ON_MESSAGE_VOID(WM_CLOSE, OnClose)
END_MESSAGE_MAP()

// CHistoryRecordDlg 消息处理程序
void CHistoryRecordDlg::OnClose()
{
	PostMessageA(m_hParentWnd, UM_DESTROYHISTORYDLG, NULL, 0);
	//DestroyWindow();
}