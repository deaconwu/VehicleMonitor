
// MsgHandleDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MsgHandle.h"
#include "MsgHandleDlg.h"
#include "afxdialogex.h"
#include "CInfoRecord.h"
#include "CHistoryRecord.h"
#include "UserMessage.h"

#pragma comment(lib,"ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMsgHandleDlg 对话框
CMsgHandleDlg::CMsgHandleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MSGHANDLE_DIALOG, pParent),
	m_pSock(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMsgHandleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CMsgHandleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CMsgHandleDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CMsgHandleDlg::OnBnClickedBtnDisconnect)
	ON_MESSAGE(NETWORK_EVENT, OnNetEvent)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CMsgHandleDlg::OnTcnSelchangeTab)
	ON_MESSAGE(UM_QUERYVINS, OnQueryVin)
	ON_MESSAGE(UM_REFRESHREC, OnRefreshVin)
END_MESSAGE_MAP()

bool CMsgHandleDlg::OnConnect()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return false;
	}

	//创建套接字
	m_pSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_pSock == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	CIPAddressCtrl* pIPAddr = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS);
	pIPAddr->GetAddress(serAddr.sin_addr.S_un.S_un_b.s_b1, serAddr.sin_addr.S_un.S_un_b.s_b2, serAddr.sin_addr.S_un.S_un_b.s_b3, serAddr.sin_addr.S_un.S_un_b.s_b4);

	CString portStr;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PORT);
	pEdit->GetWindowText(portStr);
	int iPort = _ttoi(portStr);
	serAddr.sin_port = htons(iPort);

	if (connect(m_pSock, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		DWORD dwErr = GetLastError();
		closesocket(m_pSock);
		WSACleanup();
		m_pSock = INVALID_SOCKET;
		return false;
	}

// 	char buf[BUFFER_SIZE] = {};
// 	int size = 0;
// 	INT optVal = 0;
// 	INT optLen = sizeof(optVal);
// 	getsockopt(m_pSock, SOL_SOCKET, SO_RCVBUF, (char*)&optVal, &optLen);
// 	size = recv(m_pSock, buf, optVal, 0);
// 	DWORD dw = GetLastError();
	if (WSAAsyncSelect(m_pSock, this->m_hWnd, NETWORK_EVENT, FD_READ | FD_CLOSE))
	{
		DWORD dwErr = GetLastError();
		closesocket(m_pSock);
		WSACleanup();
		m_pSock = INVALID_SOCKET;
		return false;
	}

	return true;
}

// CMsgHandleDlg 消息处理程序
LRESULT CMsgHandleDlg::OnNetEvent(WPARAM wParam, LPARAM lParam)
{
	SOCKET sock = (SOCKET)wParam;
	int iEvent = WSAGETSELECTEVENT(lParam);

	char buf[BUFFER_SIZE] = {};
	int size = 0;
// 	INT optVal = 0;
// 	INT optLen = sizeof(optVal);

	CString csStr = _T("");

	switch (iEvent)
	{
	case FD_READ:
	{
		//getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&optVal, &optLen);
		int size = recv(sock, buf, BUFFER_SIZE, 0);
		if (size > 0)
		{
			CInfoRecord::GetInstance()->OnParse(buf, size);
		}
		else
		{
			closesocket(m_pSock);
			WSACleanup();
			m_pSock = NULL;
			OnConnect();
		}

		break;
	}
	case FD_CLOSE:
	{
		closesocket(m_pSock);
		WSACleanup();
		m_pSock = NULL;
		while (!OnConnect())
		{
			closesocket(m_pSock);
			WSACleanup();
			m_pSock = NULL;
		}

		break;
	}
	default:
		break;
	}

	return 1;
}

LRESULT CMsgHandleDlg::OnQueryVin(WPARAM wParam, LPARAM lParam)
{
	char* pVin = (char*)wParam;

	m_tab.SetCurSel(0);

	m_query.OnVinInput(pVin);
	m_query.ShowWindow(SW_SHOW);
	m_statistics.ShowWindow(SW_HIDE);
	m_statistics.OnDestroy();
// 	m_alert.OnDestroy();
// 	m_alert.ShowWindow(SW_HIDE);
// 	m_voltage.OnDestroy();
// 	m_voltage.ShowWindow(SW_HIDE);
	m_warning.OnDestroy();
	m_warning.ShowWindow(SW_HIDE);

	return 0;
}

LRESULT CMsgHandleDlg::OnRefreshVin(WPARAM wParam, LPARAM lParam)
{
	m_warning.NotifyRefreshVin();
	return 0;
}

BOOL CMsgHandleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	((CButton*)GetDlgItem(IDC_BTN_CONNECT))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BTN_DISCONNECT))->EnableWindow(false);

	SetWindowText(_T("车辆监控"));

	m_query.Create(IDD_QUERY, &m_tab);
	m_statistics.Create(IDD_STATISTICS, &m_tab);
// 	m_alert.Create(IDD_ALERTDATA, &m_tab);
// 	m_voltage.Create(IDD_VOLTAGE, &m_tab);
	m_warning.Create(IDD_WARNING, &m_tab);

	CRect tabRect;
	m_tab.GetClientRect(tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 22;
	tabRect.bottom -= 1;
	m_query.MoveWindow(&tabRect);
	m_statistics.MoveWindow(&tabRect);
// 	m_alert.MoveWindow(&tabRect);
// 	m_voltage.MoveWindow(&tabRect);
	m_warning.MoveWindow(&tabRect);

	m_tab.InsertItem(0, _T("查询"));
	m_tab.InsertItem(1, _T("统计"));
// 	m_tab.InsertItem(2, _T("报警"));
// 	m_tab.InsertItem(3, _T("最低单体电压"));
	m_tab.InsertItem(2, _T("警报"));
	m_query.ShowWindow(SW_SHOW);
	m_statistics.ShowWindow(SW_HIDE);
// 	m_alert.ShowWindow(SW_HIDE);
// 	m_voltage.ShowWindow(SW_HIDE);
	m_warning.ShowWindow(SW_HIDE);

	m_tab.SetCurSel(0);

#ifdef NDEBUG
	CInfoRecord::GetInstance()->ReadVin();

// 	m_alert.LoadList();
// 	m_voltage.LoadList(this->m_hWnd);
	m_warning.LoadList(this->m_hWnd);
#else
	CInfoRecord::GetInstance()->SaveBin();
#endif

	CInfoRecord::GetInstance()->SetHwnd(this->m_hWnd);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMsgHandleDlg::OnPaint()
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
HCURSOR CMsgHandleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMsgHandleDlg::OnBnClickedBtnConnect()
{
	if (!OnConnect())
	{
		return;
	}

	((CEdit*)GetDlgItem(IDC_IPADDRESS))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_PORT))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_CONNECT))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_DISCONNECT))->EnableWindow(true);
}

void CMsgHandleDlg::OnBnClickedBtnDisconnect()
{
	closesocket(m_pSock);
	WSACleanup();
	m_pSock = INVALID_SOCKET;

	((CEdit*)GetDlgItem(IDC_IPADDRESS))->EnableWindow(true);
	((CEdit*)GetDlgItem(IDC_EDIT_PORT))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BTN_CONNECT))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BTN_DISCONNECT))->EnableWindow(false);
}

void CMsgHandleDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	switch (m_tab.GetCurSel())
	{
	case 0:
	{
		m_query.ShowWindow(SW_SHOW);
		m_statistics.OnDestroy();
		m_statistics.ShowWindow(SW_HIDE);
// 		m_alert.OnDestroy();
// 		m_alert.ShowWindow(SW_HIDE);
// 		m_voltage.OnDestroy();
// 		m_voltage.ShowWindow(SW_HIDE);
		m_warning.OnDestroy();
		m_warning.ShowWindow(SW_HIDE);
		break;
	}
	case 1:
	{
		m_query.ShowWindow(SW_HIDE);
		m_statistics.OnLauch();
		m_statistics.ShowWindow(SW_SHOW);
// 		m_alert.OnDestroy();
// 		m_alert.ShowWindow(SW_HIDE);
// 		m_voltage.OnDestroy();
// 		m_voltage.ShowWindow(SW_HIDE);
		m_warning.OnDestroy();
		m_warning.ShowWindow(SW_HIDE);
		break;
	}
	case 2:
	{
		m_query.ShowWindow(SW_HIDE);
		m_statistics.OnDestroy();
		m_statistics.ShowWindow(SW_HIDE);
// 		m_alert.OnLauch();
// 		m_alert.ShowWindow(SW_SHOW);
// 		m_voltage.OnDestroy();
// 		m_voltage.ShowWindow(SW_HIDE);
		m_warning.OnLauch();
		m_warning.ShowWindow(SW_SHOW);
		break;
	}
	default:
		break;
	}
}
