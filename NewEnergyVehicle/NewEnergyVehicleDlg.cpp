
// NewEnergyVehicleDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "NewEnergyVehicle.h"
#include "NewEnergyVehicleDlg.h"
#include "afxdialogex.h"
#include "UserMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static SHELLEXECUTEINFO shellExe[4] = {};

// CNewEnergyVehicleDlg 对话框

CNewEnergyVehicleDlg::CNewEnergyVehicleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NEWENERGYVEHICLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNewEnergyVehicleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNewEnergyVehicleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_LOGININ, &CNewEnergyVehicleDlg::OnLoginIn)
	ON_MESSAGE(UM_STEP1, &CNewEnergyVehicleDlg::OnStep1)
	ON_MESSAGE(UM_STEP2, &CNewEnergyVehicleDlg::OnStep2)
	ON_MESSAGE(UM_STEP3, &CNewEnergyVehicleDlg::OnStep3)
	ON_MESSAGE(UM_STEP4, &CNewEnergyVehicleDlg::OnStep4)
	ON_MESSAGE(UM_LOADSTEP1END, &CNewEnergyVehicleDlg::OnLoadStep1End)
	ON_MESSAGE(UM_LOADSTEP2END, &CNewEnergyVehicleDlg::OnLoadStep2End)
	ON_MESSAGE(UM_LOADSTEP3END, &CNewEnergyVehicleDlg::OnLoadStep3End)
	ON_MESSAGE(UM_HTTPPOSTEND, &CNewEnergyVehicleDlg::OnHttpPostEnd)
	ON_BN_CLICKED(IDC_RADIO1, &CNewEnergyVehicleDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CNewEnergyVehicleDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CNewEnergyVehicleDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CNewEnergyVehicleDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BTN_LAUNCH, &CNewEnergyVehicleDlg::OnBnClickedBtnLaunch)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CNewEnergyVehicleDlg::OnBnClickedBtnQuit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNewEnergyVehicleDlg 消息处理程序
LRESULT CNewEnergyVehicleDlg::OnLoginIn(WPARAM wParam, LPARAM lParam)
{
	m_loginDlg.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);

	((CButton*)GetDlgItem(IDC_RADIO1))->ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_RADIO2))->ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_RADIO3))->ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_RADIO4))->ShowWindow(SW_SHOW);

	((CButton*)GetDlgItem(IDC_BTN_LAUNCH))->ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_BTN_QUIT))->ShowWindow(SW_SHOW);

	m_step1Dlg.ShowWindow(SW_SHOW);
	m_step2Dlg.ShowWindow(SW_HIDE);
	m_step3Dlg.ShowWindow(SW_HIDE);
	m_step4Dlg.ShowWindow(SW_HIDE);

	return 0;
}

LRESULT CNewEnergyVehicleDlg::OnStep1(WPARAM wParam, LPARAM lParam)
{
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(0);

	m_step1Dlg.ShowWindow(SW_SHOW);
	m_step2Dlg.ShowWindow(SW_HIDE);
	m_step3Dlg.ShowWindow(SW_HIDE);
	m_step4Dlg.ShowWindow(SW_HIDE);

	return 0;
}

LRESULT CNewEnergyVehicleDlg::OnStep2(WPARAM wParam, LPARAM lParam)
{
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(0);

	m_step1Dlg.ShowWindow(SW_HIDE);
	m_step2Dlg.ShowWindow(SW_SHOW);
	m_step3Dlg.ShowWindow(SW_HIDE);
	m_step4Dlg.ShowWindow(SW_HIDE);

	SYSTEMTIME stDate = {};
	m_step1Dlg.FetchConfigDate(stDate);
	m_step2Dlg.InputStep1Date(stDate);
	m_step3Dlg.InputStep1Date(stDate);
	m_step4Dlg.InputStep1Date(stDate);

	return 0;
}

LRESULT CNewEnergyVehicleDlg::OnStep3(WPARAM wParam, LPARAM lParam)
{
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(0);

	m_step1Dlg.ShowWindow(SW_HIDE);
	m_step2Dlg.ShowWindow(SW_HIDE);
	m_step3Dlg.ShowWindow(SW_SHOW);
	m_step4Dlg.ShowWindow(SW_HIDE);

	SYSTEMTIME stDate = {};
	m_step2Dlg.FetchConfigExcuteDateTime(stDate);
	m_step3Dlg.InputStep2DateTime(stDate);

	return 0;
}

LRESULT CNewEnergyVehicleDlg::OnStep4(WPARAM wParam, LPARAM lParam)
{
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(1);

	m_step1Dlg.ShowWindow(SW_HIDE);
	m_step2Dlg.ShowWindow(SW_HIDE);
	m_step3Dlg.ShowWindow(SW_HIDE);
	m_step4Dlg.ShowWindow(SW_SHOW);

	SYSTEMTIME stDate = {};
	m_step3Dlg.FetchConfigExcuteDateTime(stDate);
	m_step4Dlg.InputStep3DateTime(stDate);

	return 0;
}

LRESULT CNewEnergyVehicleDlg::OnLoadStep1End(WPARAM wParam, LPARAM lParam)
{
	SYSTEMTIME stDate = {};
	m_step1Dlg.FetchConfigDate(stDate);
	m_step2Dlg.InputStep1Date(stDate);
	m_step3Dlg.InputStep1Date(stDate);
	m_step4Dlg.InputStep1Date(stDate);

	return 0;
}

LRESULT CNewEnergyVehicleDlg::OnLoadStep2End(WPARAM wParam, LPARAM lParam)
{
	SYSTEMTIME stDate = {};
	m_step2Dlg.FetchConfigExcuteDateTime(stDate);
	m_step3Dlg.InputStep2DateTime(stDate);

	return 0;
}

LRESULT CNewEnergyVehicleDlg::OnLoadStep3End(WPARAM wParam, LPARAM lParam)
{
	SYSTEMTIME stDate = {};
	m_step3Dlg.FetchConfigExcuteDateTime(stDate);
	m_step4Dlg.InputStep3DateTime(stDate);

	return 0;
}

LRESULT CNewEnergyVehicleDlg::OnHttpPostEnd(WPARAM wParam, LPARAM lParam)
{
	//计算下一周期执行时间
	CalcNextDateTime(m_dateTimeStep2);
	time_t tStep2 = ConvertSecondsTime(m_dateTimeStep2);

	CalcNextDateTime(m_dateTimeStep3);
	time_t tStep3 = ConvertSecondsTime(m_dateTimeStep3);

	CalcNextDateTime(m_dateTimeStep4);
	time_t tStep4 = ConvertSecondsTime(m_dateTimeStep4);

	SYSTEMTIME st;
	GetLocalTime(&st);
	time_t tCur = ConvertSecondsTime(st);

	//间隔下一周期触发
	SetTimer(TIMER_ID_STEP2_START, (tStep2 - tCur) * 1000, NULL);

	SetTimer(TIMER_ID_STEP3_START, (tStep3 - tCur) * 1000, NULL);

	SetTimer(TIMER_ID_STEP4_START, (tStep4 - tCur) * 1000, NULL);

	return 0;
}

BOOL CNewEnergyVehicleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CRect rect;
	GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_step1Dlg.Create(IDD_STEP1, this);
	m_step1Dlg.MoveWindow(rect);
	m_step1Dlg.SetHwnd(this->m_hWnd);
	m_step1Dlg.ShowWindow(SW_HIDE);

	m_step2Dlg.Create(IDD_STEP2, this);
	m_step2Dlg.MoveWindow(rect);
	m_step2Dlg.SetHwnd(this->m_hWnd);
	m_step2Dlg.ShowWindow(SW_HIDE);

	m_step3Dlg.Create(IDD_STEP3, this);
	m_step3Dlg.MoveWindow(rect);
	m_step3Dlg.SetHwnd(this->m_hWnd);
	m_step3Dlg.ShowWindow(SW_HIDE);

	m_step4Dlg.Create(IDD_STEP4, this);
	m_step4Dlg.MoveWindow(rect);
	m_step4Dlg.SetHwnd(this->m_hWnd);
	m_step4Dlg.ShowWindow(SW_HIDE);

// 	rect.top += 15;
// 	rect.bottom += 15;
// 	rect.left += 45;
// 	rect.right += 45;
	m_loginDlg.Create(IDD_LOGIN, this);
	m_loginDlg.MoveWindow(rect);
	m_loginDlg.SetHwnd(this->m_hWnd);
	m_loginDlg.ShowWindow(SW_SHOW);

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO1))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO2))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO3))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO4))->ShowWindow(SW_HIDE);

	((CButton*)GetDlgItem(IDC_BTN_LAUNCH))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BTN_QUIT))->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);

	//如果数据库和表不存在就创建：步骤、执行状态(0开始执行、1已退出、2等待完成)、执行时间
	strcpy(g_sqliteLog.sqlCmd, "create table step_log(stepIndex INTEGER NOT NULL, state INTEGER NOT NULL, operateTime BLOB NOT NULL);");
	sqlite3_exec_cmd(&g_sqliteLog);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNewEnergyVehicleDlg::OnPaint()
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
HCURSOR CNewEnergyVehicleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNewEnergyVehicleDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_step1Dlg.ShowWindow(SW_SHOW);
	m_step2Dlg.ShowWindow(SW_HIDE);
	m_step3Dlg.ShowWindow(SW_HIDE);
	m_step4Dlg.ShowWindow(SW_HIDE);
}

void CNewEnergyVehicleDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_step1Dlg.ShowWindow(SW_HIDE);
	m_step2Dlg.ShowWindow(SW_SHOW);
	m_step3Dlg.ShowWindow(SW_HIDE);
	m_step4Dlg.ShowWindow(SW_HIDE);

	SYSTEMTIME stDate = {};
	m_step1Dlg.FetchConfigDate(stDate);
	m_step2Dlg.InputStep1Date(stDate);
	m_step3Dlg.InputStep1Date(stDate);
	m_step4Dlg.InputStep1Date(stDate);
}

void CNewEnergyVehicleDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_step1Dlg.ShowWindow(SW_HIDE);
	m_step2Dlg.ShowWindow(SW_HIDE);
	m_step3Dlg.ShowWindow(SW_SHOW);
	m_step4Dlg.ShowWindow(SW_HIDE);

	SYSTEMTIME stDate = {};
	m_step1Dlg.FetchConfigDate(stDate);
	m_step2Dlg.InputStep1Date(stDate);
	m_step3Dlg.InputStep1Date(stDate);
	m_step4Dlg.InputStep1Date(stDate);

	stDate = {};
	m_step2Dlg.FetchConfigExcuteDateTime(stDate);
	m_step3Dlg.InputStep2DateTime(stDate);
}

void CNewEnergyVehicleDlg::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_step1Dlg.ShowWindow(SW_HIDE);
	m_step2Dlg.ShowWindow(SW_HIDE);
	m_step3Dlg.ShowWindow(SW_HIDE);
	m_step4Dlg.ShowWindow(SW_SHOW);

	SYSTEMTIME stDate = {};
	m_step1Dlg.FetchConfigDate(stDate);
	m_step2Dlg.InputStep1Date(stDate);
	m_step3Dlg.InputStep1Date(stDate);
	m_step4Dlg.InputStep1Date(stDate);

	stDate = {};
	m_step3Dlg.FetchConfigExcuteDateTime(stDate);
	m_step4Dlg.InputStep3DateTime(stDate);
}

void CNewEnergyVehicleDlg::OnBnClickedBtnLaunch()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_step1Dlg.CheckConfigComplete())
	{
		MessageBox(_T("数据收发配置未完成"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step1Dlg.CheckConfigDateTimeValid())
	{
		MessageBox(_T("数据收发配置时间不能早于当前时间"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step2Dlg.CheckConfigComplete())
	{
		MessageBox(_T("数据入库配置未完成"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step3Dlg.CheckConfigComplete())
	{
		MessageBox(_T("常态化点名配置未完成"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step4Dlg.CheckConfigComplete())
	{
		MessageBox(_T("数据上传配置未完成"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step2Dlg.CheckConfigEnterDateValid())
	{
		MessageBox(_T("入库数据日期错误！"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step2Dlg.CheckConfigExcuteDateTimeValid())
	{
		MessageBox(_T("入库执行时间错误！"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step3Dlg.CheckConfigEnterDateValid())
	{
		MessageBox(_T("常态化点名数据日期错误！"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step3Dlg.CheckConfigExcuteDateTimeValid())
	{
		MessageBox(_T("常态化点名执行时间错误！"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step4Dlg.CheckConfigExcuteDateTimeValid())
	{
		MessageBox(_T("上传执行时间错误！"), _T("提示"), MB_OK);
		return;
	}

	time_t tElapse1 = m_step1Dlg.FetchExcuteTimeElapse();	//获得间隔多久触发定时器
	if (tElapse1 <= 0)
	{
		MessageBox(_T("数据收发配置时间不能早于当前时间"), _T("提示"), MB_OK);
		return;
	}

	time_t tElapse2 = m_step2Dlg.FetchExcuteTimeElapse();
	if (tElapse2 <= 0)
	{
		MessageBox(_T("入库执行时间不能早于当前时间"), _T("提示"), MB_OK);
		return;
	}

	time_t tElapse3 = m_step3Dlg.FetchExcuteTimeElapse();
	if (tElapse3 <= 0)
	{
		MessageBox(_T("常态化点名执行时间不能早于当前时间"), _T("提示"), MB_OK);
		return;
	}

	time_t tElapse4 = m_step4Dlg.FetchExcuteTimeElapse();
	if (tElapse3 <= 0)
	{
		MessageBox(_T("上传执行时间不能早于当前时间"), _T("提示"), MB_OK);
		return;
	}

	((CButton*)GetDlgItem(IDC_BTN_LAUNCH))->EnableWindow(false);
	m_step1Dlg.DisableCtrl();
	m_step2Dlg.DisableCtrl();
	m_step3Dlg.DisableCtrl();
	m_step4Dlg.DisableCtrl();

	m_step2Dlg.FetchConfigExcuteDateTime(m_dateTimeStep2);
	m_step3Dlg.FetchConfigExcuteDateTime(m_dateTimeStep3);
	m_step4Dlg.FetchConfigExcuteDateTime(m_dateTimeStep4);

	SetTimer(TIMER_ID_STEP1_START, tElapse1 * 1000, NULL);	//最后一个填NULL，默认回调OnTimer

	//SetTimer(TIMER_ID_STEP2_START, tElapse2 * 1000, NULL);

	SetTimer(TIMER_ID_STEP2_START, (tElapse1 + 10) * 1000, NULL);

	//SetTimer(TIMER_ID_STEP3_START, tElapse3 * 1000, NULL);

	SetTimer(TIMER_ID_STEP3_START, (tElapse1+20) * 1000, NULL);

	//SetTimer(TIMER_ID_STEP4_START, tElapse4 * 1000, NULL);

	SetTimer(TIMER_ID_STEP4_START, (tElapse1 + 30) * 1000, NULL);
}

void CNewEnergyVehicleDlg::OnBnClickedBtnQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_QUIT, 0, 0);
}

void CNewEnergyVehicleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnTimer(nIDEvent);

	KillTimer(nIDEvent);

	SYSTEMTIME st;
	GetLocalTime(&st);

	if (nIDEvent == TIMER_ID_STEP1_START)
	{
		//sqlite日志记录数据收发运行
		ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
		sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(1, %u, '%02u-%02u-%02u %02u:%02u:%02u');",
			eStepState_Excute, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		sqlite3_exec_cmd(&g_sqliteLog);

		//数据收发一直运行
		ShellExecute(0, "open", "F:\\VSProject\\VehicleInfo\\x64\\Debug\\VehicleInfo.exe", "", "", SW_SHOWNORMAL);
	}
	else if (nIDEvent == TIMER_ID_STEP2_START)
	{
		//sqlite日志记录执行数据入库
		ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
		sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(2, %u, '%02u-%02u-%02u %02u:%02u:%02u');",
			eStepState_Excute, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		sqlite3_exec_cmd(&g_sqliteLog);

		//执行数据入库
		ShellExecute(0, "open", "F:\\duilib\\VehicleDuilibMfc\\x64\\Release\\VehicleDuilibMfc.exe", "", "", SW_SHOWNORMAL);
	}
	else if (nIDEvent == TIMER_ID_STEP3_START || nIDEvent == TIMER_ID_STEP2_ENDCHECK)
	{
		//检测数据入库是否完成
		DWORD dwProcess = GetProcessidFromName("VehicleDuilibMfc.exe");
		if (dwProcess == 0)
		{
			//sqlite日志记录数据入库完成
			ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
			sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(2, %u, '%02u-%02u-%02u %02u:%02u:%02u');",
				eStepState_Quit, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sqlite3_exec_cmd(&g_sqliteLog);

			//sqlite日志记录执行常态化点名
			ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
			sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(3, %u, '%02u-%02u-%02u %02u:%02u:%02u');",
				eStepState_Excute, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sqlite3_exec_cmd(&g_sqliteLog);

			//已完成入库，执行常态化点名
			ShellExecute(0, "open", "F:\\sqlite\\sqlite3.exe", "", "", SW_SHOWNORMAL);
		}
		else
		{
			//sqlite日志记录等待数据入库完成
			ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
			sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(2, %u, '%02u-%02u-%02u %02u:%02u:%02u');",
				eStepState_Wait, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sqlite3_exec_cmd(&g_sqliteLog);

			//未完成入库，间隔1秒检查入库是否完成
			SetTimer(TIMER_ID_STEP2_ENDCHECK, 1000, NULL);
		}
	}
	else if (nIDEvent == TIMER_ID_STEP4_START || nIDEvent == TIMER_ID_STEP3_ENDCHECK)
	{
		//检测常态化点名是否完成
		DWORD dwProcess2 = GetProcessidFromName("VehicleDuilibMfc.exe");
		DWORD dwProcess3 = GetProcessidFromName("sqlite3.exe");
		if (dwProcess2 == 0 && dwProcess3 == 0)
		{
			//sqlite日志记录常态化点名完成
			ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
			sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(3, %u, '%02u-%02u-%02u %02u:%02u:%02u');",
				eStepState_Quit, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sqlite3_exec_cmd(&g_sqliteLog);

			//sqlite日志记录执行数据上传
			ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
			sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(4, %u, '%02u-%02u-%02u %02u:%02u:%02u');",
				eStepState_Excute, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sqlite3_exec_cmd(&g_sqliteLog);

			//已完成常态化点名，执行数据上传
			m_step4Dlg.ExcutePost();
		}
		else
		{
			//sqlite日志记录等待常态化点名完成
			ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
			sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(3, %u, '%02u-%02u-%02u %02u:%02u:%02u');",
				eStepState_Wait, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sqlite3_exec_cmd(&g_sqliteLog);

			//未完成常态化点名，间隔1秒检查常态化点名是否完成
			SetTimer(TIMER_ID_STEP3_ENDCHECK, 1000, NULL);
		}
	}
}
