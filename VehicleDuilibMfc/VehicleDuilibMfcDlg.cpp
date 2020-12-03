
// VehicleDuilibMfcDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "VehicleDuilibMfc.h"
#include "VehicleDuilibMfcDlg.h"
#include "afxdialogex.h"
#include "CHelloDuilibWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVehicleDuilibMfcDlg 对话框



CVehicleDuilibMfcDlg::CVehicleDuilibMfcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VEHICLEDUILIBMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVehicleDuilibMfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVehicleDuilibMfcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_QUERY, OnTabQuery)
	ON_MESSAGE(UM_STATISTIC, OnTabStatis)
END_MESSAGE_MAP()


// CVehicleDuilibMfcDlg 消息处理程序

LRESULT CVehicleDuilibMfcDlg::OnTabQuery(WPARAM wParam, LPARAM lParam)
{
	m_query.ShowWindow(TRUE);
	m_statis.ShowWindow(FALSE);

	return 0;
}

LRESULT CVehicleDuilibMfcDlg::OnTabStatis(WPARAM wParam, LPARAM lParam)
{
	m_query.ShowWindow(FALSE);
	m_statis.ShowWindow(TRUE);

	return 0;
}


BOOL CVehicleDuilibMfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowText(_T("车辆监控"));

	CPaintManagerUI::SetInstance(AfxGetInstanceHandle());

	m_duiwnd.Create(*this, _T("DUIWnd"), UI_WNDSTYLE_CHILD, 0, -10, -31);
	m_duiwnd.Init(this->m_hWnd);
	m_duiwnd.ShowWindow(TRUE);

	m_query.Create(*this, _T("DUIQuery"), UI_WNDSTYLE_CHILD, 0, -10, 31);
	m_query.ShowWindow(TRUE);

	m_statis.Create(*this, _T("DUIStatis"), UI_WNDSTYLE_CHILD, 0, -10, 31);
	m_statis.ShowWindow(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVehicleDuilibMfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVehicleDuilibMfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

