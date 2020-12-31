// CStep4Dlg.cpp: 实现文件
//

#include "pch.h"
#include "NewEnergyVehicle.h"
#include "CStep4Dlg.h"
#include "afxdialogex.h"
#include "http_post.h"

DWORD WINAPI OnLoadConfig4Thread(LPVOID lparam);

// CStep4Dlg 对话框

IMPLEMENT_DYNAMIC(CStep4Dlg, CDialogEx)

CStep4Dlg::CStep4Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STEP4, pParent)
{
	m_bi.hwndOwner = this->m_hWnd;
	memset(&m_stepConfig, 0, sizeof(m_stepConfig));
}

CStep4Dlg::~CStep4Dlg()
{
}

void CStep4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CStep4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP4EXECUTE))->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	((CEdit*)GetDlgItem(IDC_EDIT_STEP4_PERIOD))->SetWindowText(_T("1"));

	DWORD dwThreadId;
	HANDLE hThread = CreateThread(NULL, NULL, OnLoadConfig4Thread, this->m_hWnd, 0, &dwThreadId);

	return TRUE;
}

void CStep4Dlg::DisableCtrl()
{
	((CButton*)GetDlgItem(IDC_BTN_STEP4_APPLY))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP4_PATHUPLOAD))->EnableWindow(false);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP4_URL))->EnableWindow(false);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP4EXECUTE))->EnableWindow(false);
}

bool CStep4Dlg::CheckConfigComplete()
{
	return (strlen(m_stepConfig.ch_0) > 0) && (strlen(m_stepConfig.ch_1) > 0) && (m_stepConfig.ch_3 > 0);
}

bool CStep4Dlg::CheckConfigExcuteDateTimeValid()
{
	//配置的上传执行时间
	SYSTEMTIME stDateTime = {};
	Str2Time(stDateTime, m_stepConfig.ch_2);

	//当前时间
	SYSTEMTIME stCur;
	GetLocalTime(&stCur);

	//上传执行时间判断，在数据接收日期之后
	if (stDateTime.wYear < m_step1Date.wYear)
	{
		return false;
	}

	if (stDateTime.wYear == m_step1Date.wYear)
	{
		if (stDateTime.wMonth < m_step1Date.wMonth)
		{
			return false;
		}

		if (stDateTime.wMonth == m_step1Date.wMonth && stDateTime.wDay <= m_step1Date.wDay)
		{
			return false;
		}
	}

	//上传执行时间判断，在当前时间之后
	time_t tDateTime = ConvertSecondsTime(stDateTime);
	time_t tCur = ConvertSecondsTime(stCur);
	if (tDateTime <= tCur)
	{
		return false;
	}

	//上传执行时间判断，在常态化点名执行时间之后
	time_t tStep3 = ConvertSecondsTime(m_step3DateTime);
	if (tDateTime <= tStep3)
	{
		return false;
	}

	return true;
}

void CStep4Dlg::FetchConfigExcuteDateTime(SYSTEMTIME& stDate)
{
	Str2Time(stDate, m_stepConfig.ch_2);
}

time_t CStep4Dlg::FetchExcuteTimeElapse()
{
	SYSTEMTIME stExcute = {};
	Str2Time(stExcute, m_stepConfig.ch_2);
	time_t tExcute = ConvertSecondsTime(stExcute);

	SYSTEMTIME st;
	GetLocalTime(&st);
	time_t tCur = ConvertSecondsTime(st);

	return tExcute - tCur;
}

void CStep4Dlg::ExcutePost()
{
	bool ret = OnPost(m_stepConfig.ch_1);

	SYSTEMTIME st;
	GetLocalTime(&st);

	if (!ret)
	{
		//sqlite日志记录数据上传失败
		ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
		sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(4, %u, '%02u-%02u-%02u %02u:%02u:%02u');",
			eStepState_Error, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		sqlite3_exec_cmd(&g_sqliteLog);
	}

	//sqlite日志记录数据上传结束
	ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
	sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO step_log VALUES(4, %u, '%02u-%02u-%02u %02u:%02u:%02u');",
		eStepState_Quit, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	sqlite3_exec_cmd(&g_sqliteLog);

	//上传完成，再周期触发数据入库、常态化点名、数据上传
	::PostMessage(m_hPWnd, UM_HTTPPOSTEND, NULL, NULL);
}

BEGIN_MESSAGE_MAP(CStep4Dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_STEP4_PRE, &CStep4Dlg::OnBnClickedBtnStep4Pre)
	ON_BN_CLICKED(IDC_BTN_STEP4_PATHUPLOAD, &CStep4Dlg::OnBnClickedBtnStep4Pathupload)
	ON_BN_CLICKED(IDC_BTN_STEP4_APPLY, &CStep4Dlg::OnBnClickedBtnStep4Apply)
	ON_MESSAGE(UM_LOADCONFIG, &CStep4Dlg::OnLoadConfig)
END_MESSAGE_MAP()


// CStep4Dlg 消息处理程序
LRESULT CStep4Dlg::OnLoadConfig(WPARAM wParam, LPARAM lParam)
{
	STSTEP4CONFIG* pMsg = (STSTEP4CONFIG*)wParam;

	memcpy(&m_stepConfig, pMsg, sizeof(STSTEP4CONFIG));

	((CEdit*)GetDlgItem(IDC_EDIT_STEP4_URL))->SetWindowText(pMsg->ch_0);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP4_PATHUPLOAD))->SetWindowText(pMsg->ch_1);

	SYSTEMTIME stTime = {};
	Str2Time(stTime, pMsg->ch_2);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP4EXECUTE))->SetTime(stTime);

	CString csStr = _T("");
	csStr.Format(_T("%u"), pMsg->ch_3);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP4_PERIOD))->SetWindowText(csStr);

	return 0;
}

void CStep4Dlg::OnBnClickedBtnStep4Pre()
{
	// TODO: 在此添加控件通知处理程序代码
	::PostMessage(m_hPWnd, UM_STEP3, NULL, NULL);
}

void CStep4Dlg::OnBnClickedBtnStep4Pathupload()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP4_PATHUPLOAD))->SetWindowText(tchPath);
}


void CStep4Dlg::OnBnClickedBtnStep4Apply()
{
	// TODO: 在此添加控件通知处理程序代码

	memset(&m_stepConfig, 0, sizeof(m_stepConfig));

	TCHAR tchConfig[256] = {};
	((CEdit*)GetDlgItem(IDC_EDIT_STEP4_URL))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_0, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP4_PATHUPLOAD))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_1, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP4_PERIOD))->GetWindowText(tchConfig, 256);
	m_stepConfig.ch_3 = atoi(tchConfig);

	SYSTEMTIME stTime = {};
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP4EXECUTE))->GetTime(&stTime);
	sprintf(m_stepConfig.ch_2, "%02u-%02u-%02u %02u:%02u:%02u", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	FILE *pF = fopen("conf_step4.txt", "wb");
	fprintf(pF, "对端URL地址: \"%s\"\n", m_stepConfig.ch_0);
	fprintf(pF, "上传数据路径: \"%s\"\n", m_stepConfig.ch_1);
	fprintf(pF, "上传执行时间: \"%s\"\n", m_stepConfig.ch_2);
	fprintf(pF, "上传周期: \"%u\"", m_stepConfig.ch_3);

	fclose(pF);

	if (!CheckConfigExcuteDateTimeValid())
	{
		MessageBox(_T("上传执行时间错误！"), _T("提示"), MB_OK);
		return;
	}
}

DWORD WINAPI OnLoadConfig4Thread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	STSTEP4CONFIG stData = {};

	FILE *fpRead = fopen("conf_step4.txt", "rb");
	if (NULL == fpRead)
	{
		return 0;
	}

	fseek(fpRead, 0, SEEK_END);
	long len = ftell(fpRead);
	char* pContent = (char*)malloc(len);
	fseek(fpRead, 0, SEEK_SET);
	fread((char*)pContent, 1, len, fpRead);
	fclose(fpRead);

	const char* pConfig = (char*)strtok(pContent, "\n");
	FilterConfig(stData.ch_0, pConfig);

	pConfig = (char*)strtok(NULL, "\n");
	FilterConfig(stData.ch_1, pConfig);

	pConfig = (char*)strtok(NULL, "\n");
	FilterConfig(stData.ch_2, pConfig);

	char chInt[32] = {};

	pConfig = (char*)strtok(NULL, "\n");
	FilterConfig(chInt, pConfig);
	stData.ch_3 = atoi(chInt);

	free(pContent);

	SendMessage(hWnd, UM_LOADCONFIG, (WPARAM)&stData, 0);

	return 0;
}