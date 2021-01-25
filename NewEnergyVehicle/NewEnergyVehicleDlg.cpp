
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

//数据入库相关的几个执行程序
static char g_coreExe[5][30] = {"CoreExe.exe", "S1_ThRaw2GB.exe", "S2_ThGB2RVT.exe", "S3_ThRVT2DtVFb2_0.exe", "S4_ThRVT2TzTFb2_0.exe"};


// CNewEnergyVehicleDlg 对话框

CNewEnergyVehicleDlg::CNewEnergyVehicleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NEWENERGYVEHICLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(&m_stepStatus, 0, sizeof(m_stepStatus));
}

void CNewEnergyVehicleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CNewEnergyVehicleDlg::KillAllTimer()
{
	KillTimer(TIMER_ID_STEP1_START);
	KillTimer(TIMER_ID_STEP2_START);
	KillTimer(TIMER_ID_STEP3_START);
	KillTimer(TIMER_ID_STEP4_START);
	KillTimer(TIMER_ID_STEP2_ENDCHECK);
	KillTimer(TIMER_ID_STEP3_ENDCHECK);
}

void CNewEnergyVehicleDlg::KillAttachProcess()
{
	HANDLE hProcess = NULL;
	DWORD dwProcess = GetProcessidFromName("Port_2GB.exe");
	if (dwProcess > 0)
	{
		hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcess);
		if (NULL != hProcess)
		{
			TerminateProcess(hProcess, 0);
		}
	}

	for (int i=0; i<5; i++)
	{
		dwProcess = GetProcessidFromName(g_coreExe[i]);
		hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcess);
		if (NULL != hProcess)
		{
			TerminateProcess(hProcess, 0);
		}
	}

	char chCall[20] = {};
	for (int i = 0; i < 21; i++)
	{
		sprintf(chCall, "Call%u.exe", i);
		dwProcess = GetProcessidFromName(chCall);
		hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcess);
		if (NULL != hProcess)
		{
			TerminateProcess(hProcess, 0);
		}
	}
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
	memset(&m_stepStatus, 0, sizeof(m_stepStatus));

	//计算下一周期数据日期
	CalcNextDateTime(m_dataDateStep2);
	CalcNextDateTime(m_dataDateStep3);

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

	//如果数据库和表不存在就创建：步骤、执行程序名、执行状态(0开始执行、1已退出、2等待完成)、执行时间
	strcpy(g_sqliteLog.sqlCmd, "create table step_log(stepIndex INTEGER NOT NULL, programName TEXT NOT NULL, state INTEGER NOT NULL, operateTime BLOB NOT NULL);");
	sqlite3_exec_cmd(&g_sqliteLog);

	//文件上传记录，resp_code-函数返回值，status_code-服务器返回状态码，200上传成功，其它上传失败；若resp_code返回失败，status_code允许为NULL
	ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
	strcpy(g_sqliteLog.sqlCmd, "create table post_record(vin TEXT NOT NULL, model TEXT NOT NULL, file BLOB NOT NULL, resp_code INTEGER NOT NULL, status_code INTEGER NULL, time BLOB NOT NULL);");
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

BOOL CNewEnergyVehicleDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
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
		MessageBox(_T("入库数据日期不能早于接收日期！"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step2Dlg.CheckConfigExcuteDateTimeValid())
	{
		MessageBox(_T("入库执行时间不能早于入库数据日期！"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step3Dlg.CheckConfigEnterDateValid())
	{
		MessageBox(_T("常态化点名数据日期不能早于接收日期！"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step3Dlg.CheckConfigExcuteDateTimeValid())
	{
		MessageBox(_T("常态化点名执行时间不能早于点名数据日期！"), _T("提示"), MB_OK);
		return;
	}

	if (!m_step4Dlg.CheckConfigExcuteDateTimeValid())
	{
		MessageBox(_T("上传执行时间不能早于常态化点名执行时间！"), _T("提示"), MB_OK);
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

	m_step2Dlg.FetchConfigDataDate(m_dataDateStep2);
	m_step3Dlg.FetchConfigDataDate(m_dataDateStep3);

	SetTimer(TIMER_ID_STEP1_START, tElapse1 * 1000, NULL);	//最后一个填NULL，默认回调OnTimer

	SetTimer(TIMER_ID_STEP2_START, tElapse2 * 1000, NULL);

	//SetTimer(TIMER_ID_STEP2_START, (tElapse1 + 5) * 1000, NULL);

	SetTimer(TIMER_ID_STEP3_START, tElapse3 * 1000, NULL);

	//SetTimer(TIMER_ID_STEP3_START, (tElapse1 + 60) * 1000, NULL);

	SetTimer(TIMER_ID_STEP4_START, tElapse4 * 1000, NULL);

	//SetTimer(TIMER_ID_STEP4_START, (tElapse1 + 70) * 1000, NULL);
}

void CNewEnergyVehicleDlg::OnBnClickedBtnQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	KillAttachProcess();

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
		sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(1, 'Port_2GB.exe', %u, '%02u-%02u-%02u %02u:%02u:%02u');",
			eStepState_Excute, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		sqlite3_exec_cmd(&g_sqliteLog);

		char param[512] = {};
		m_step1Dlg.FetchTransParam(param);

		//数据收发一直运行
		ShellExecute(0, "open", "Port_2GB.exe", param, "", SW_SHOWNORMAL);
	}
	else if (nIDEvent == TIMER_ID_STEP2_START)
	{
		//sqlite日志记录执行数据入库
		ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
		sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(2, 'CoreExe.exe', %u, '%02u-%02u-%02u %02u:%02u:%02u');",
			eStepState_Excute, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		sqlite3_exec_cmd(&g_sqliteLog);

		char param[512] = {};
		sprintf(param, "%02u %02u %02u ", m_dataDateStep2.wYear, m_dataDateStep2.wMonth, m_dataDateStep2.wDay);
		m_step2Dlg.FetchTransParam(param);

		//执行数据入库
		ShellExecute(0, "open", g_coreExe[0], param, "", SW_SHOWNORMAL);
		m_stepStatus.eStep2 = eStepState_Excute;
	}
	else if (nIDEvent == TIMER_ID_STEP3_START || nIDEvent == TIMER_ID_STEP2_ENDCHECK)
	{
		char chCore[30] = {};
		DWORD dwProcess = 0;

		//检测数据入库是否完成
		for (int i=4; i>=0; i--)
		{
			dwProcess = GetProcessidFromName(g_coreExe[i]);
			if (dwProcess > 0)
			{
				//数据入库未完成
				memcpy(chCore, g_coreExe[i], 30);
				break;
			}
		}

		if (dwProcess == 0)
		{
			//sqlite日志记录数据入库完成
			ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
			sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(3, 'CoreExe', %u, '%02u-%02u-%02u %02u:%02u:%02u');",
				eStepState_Quit, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sqlite3_exec_cmd(&g_sqliteLog);

			m_stepStatus.eStep2 = eStepState_Quit;

			char param[512] = {};
			sprintf(param, "%02u_%02u_%02u ", m_dataDateStep3.wYear, m_dataDateStep3.wMonth, m_dataDateStep3.wDay);
			m_step3Dlg.FetchTransParam(param);

			//已完成入库，开始执行常态化点名
			for (UINT i=0; i<21; i++)
			{
				//顺序执行选中的点名类型
				if (m_step3Dlg.CheckSelect(i))
				{
					char chCall[20] = {};
					sprintf(chCall, "Call%u.exe", i);

					//sqlite日志记录常态化点名执行
					ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
					sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(3, '%s', %u, '%02u-%02u-%02u %02u:%02u:%02u');",
						chCall, eStepState_Excute, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
					sqlite3_exec_cmd(&g_sqliteLog);

					ShellExecute(0, "open", chCall, param, "", SW_SHOWNORMAL);
					m_stepStatus.eStep3 = eStepState_Excute;
					dwProcess = GetProcessidFromName(chCall);
					while (dwProcess > 0)
					{
						dwProcess = GetProcessidFromName(chCall);
					}
				}
			}
		}
		else
		{
			//sqlite日志记录数据入库等待
			ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
			sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(3, '%s', %u, '%02u-%02u-%02u %02u:%02u:%02u');",
				chCore, eStepState_Wait, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sqlite3_exec_cmd(&g_sqliteLog);

			//未完成入库，间隔1秒检查入库是否完成
			SetTimer(TIMER_ID_STEP2_ENDCHECK, 1000, NULL);
		}
	}
	else if (nIDEvent == TIMER_ID_STEP4_START || nIDEvent == TIMER_ID_STEP3_ENDCHECK)
	{
		DWORD dwProcess = 0;
		char chExe[30] = {};

		//检测数据入库是否完成
		for (int i = 4; i >= 0; i--)
		{
			dwProcess = GetProcessidFromName(g_coreExe[i]);
			if (dwProcess > 0)
			{
				//数据入库未完成
				memcpy(chExe, g_coreExe[i], 30);
				break;
			}
		}

		if (dwProcess == 0)
		{
			//检测常态化点名是否完成
			for (UINT i = 0; i < 21; i++)
			{
				sprintf(chExe, "Call%u.exe", i);
				dwProcess = GetProcessidFromName(chExe);
				if (dwProcess > 0)
				{
					//常态化点名未完成
					break;
				}
			}
		}

		if (dwProcess == 0)
		{
			memset(chExe, 0, sizeof(chExe));
			strcpy(chExe, "Call");
		}

		if (dwProcess > 0 || m_stepStatus.eStep2==0 || m_stepStatus.eStep3==0)
		{
			//sqlite日志记录常态化点名等待
			ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
			sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(4, '%s', %u, '%02u-%02u-%02u %02u:%02u:%02u');",
				chExe, eStepState_Wait, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sqlite3_exec_cmd(&g_sqliteLog);

			//未完成常态化点名，间隔1秒检查常态化点名是否完成
			SetTimer(TIMER_ID_STEP3_ENDCHECK, 1000, NULL);
		}
		else
		{
			//sqlite日志记录常态化点名完成
			ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
			sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(4, 'Call', %u, '%02u-%02u-%02u %02u:%02u:%02u');",
				eStepState_Quit, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sqlite3_exec_cmd(&g_sqliteLog);

			m_stepStatus.eStep3 = eStepState_Quit;

			//sqlite日志记录数据上传执行
			ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
			sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(4, 'Post', %u, '%02u-%02u-%02u %02u:%02u:%02u');",
				eStepState_Excute, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sqlite3_exec_cmd(&g_sqliteLog);

			//已完成常态化点名，开始执行数据上传
			//m_step4Dlg.ExcutePost();
		}
	}
}
