// CStep2Dlg.cpp: 实现文件
//

#include "pch.h"
#include "NewEnergyVehicle.h"
#include "CStep2Dlg.h"
#include "afxdialogex.h"

DWORD WINAPI OnLoadConfig2Thread(LPVOID lparam);

// CStep2Dlg 对话框

IMPLEMENT_DYNAMIC(CStep2Dlg, CDialogEx)

CStep2Dlg::CStep2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STEP2, pParent)
{
	m_bi.hwndOwner = this->m_hWnd;
	memset(&m_stepConfig, 0, sizeof(m_stepConfig));
}

CStep2Dlg::~CStep2Dlg()
{
}

void CStep2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CStep2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP2EXCUTE))->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PERIOD))->SetWindowText(_T("1"));

	DWORD dwThreadId;
	HANDLE hThread = CreateThread(NULL, NULL, OnLoadConfig2Thread, this->m_hWnd, 0, &dwThreadId);

	return TRUE;
}

void CStep2Dlg::DisableCtrl()
{
	((CButton*)GetDlgItem(IDC_BTN_STEP2_APPLY))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP2_PATHORINGIN))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP2_PATHFK))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP2_PATHNATION))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP2_PATHRVT))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP2_PATHDTVFB))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP2_PATHTZTFB))->EnableWindow(false);

	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP2ENTER))->EnableWindow(false);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP2EXCUTE))->EnableWindow(false);
}

BEGIN_MESSAGE_MAP(CStep2Dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_STEP2_PRE, &CStep2Dlg::OnBnClickedBtnStep2Pre)
	ON_BN_CLICKED(IDC_BTN_STEP2_NEXT, &CStep2Dlg::OnBnClickedBtnStep2Next)
	ON_BN_CLICKED(IDC_BTN_STEP2_PATHORINGIN, &CStep2Dlg::OnBnClickedBtnStep2Pathoringin)
	ON_BN_CLICKED(IDC_BTN_STEP2_PATHFK, &CStep2Dlg::OnBnClickedBtnStep2Pathfk)
	ON_BN_CLICKED(IDC_BTN_STEP2_PATHNATION, &CStep2Dlg::OnBnClickedBtnStep2Pathnation)
	ON_BN_CLICKED(IDC_BTN_STEP2_PATHRVT, &CStep2Dlg::OnBnClickedBtnStep2Pathrvt)
	ON_BN_CLICKED(IDC_BTN_STEP2_PATHDTVFB, &CStep2Dlg::OnBnClickedBtnStep2Pathdtvfb)
	ON_BN_CLICKED(IDC_BTN_STEP2_PATHTZTFB, &CStep2Dlg::OnBnClickedBtnStep2Pathtztfb)
	ON_BN_CLICKED(IDC_BTN_STEP2_APPLY, &CStep2Dlg::OnBnClickedBtnStep2Apply)
	ON_MESSAGE(UM_LOADCONFIG, &CStep2Dlg::OnLoadConfig)
END_MESSAGE_MAP()


// CStep2Dlg 消息处理程序

LRESULT CStep2Dlg::OnLoadConfig(WPARAM wParam, LPARAM lParam)
{
	STSTEP2CONFIG* pMsg = (STSTEP2CONFIG*)wParam;

	memcpy(&m_stepConfig, pMsg, sizeof(STSTEP2CONFIG));

	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHORINGIN))->SetWindowText(pMsg->ch_1);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHFK))->SetWindowText(pMsg->ch_2);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHNATION))->SetWindowText(pMsg->ch_3);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHRVT))->SetWindowText(pMsg->ch_4);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHDTVFB))->SetWindowText(pMsg->ch_5);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHTZTFB))->SetWindowText(pMsg->ch_6);

	SYSTEMTIME stTime = {};
	Str2Time(stTime, pMsg->ch_0, true);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP2ENTER))->SetTime(stTime);
	Str2Time(stTime, pMsg->ch_7);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP2EXCUTE))->SetTime(stTime);

	::PostMessage(m_hPWnd, UM_LOADSTEP2END, NULL, NULL);

	return 0;
}

void CStep2Dlg::FetchConfigDataDate(SYSTEMTIME& stDate)
{
	Str2Time(stDate, m_stepConfig.ch_0, true);
}

void CStep2Dlg::FetchConfigExcuteDateTime(SYSTEMTIME& stDate)
{
	Str2Time(stDate, m_stepConfig.ch_7);
}

void CStep2Dlg::FetchTransParam(char* pStrParam)
{
	strcat(pStrParam, m_stepConfig.ch_1);
	strcat(pStrParam, " ");
	strcat(pStrParam, m_stepConfig.ch_2);
	strcat(pStrParam, " ");
	strcat(pStrParam, m_stepConfig.ch_3);
	strcat(pStrParam, " ");
	strcat(pStrParam, m_stepConfig.ch_4);
	strcat(pStrParam, " ");
	strcat(pStrParam, m_stepConfig.ch_5);
	strcat(pStrParam, " ");
	strcat(pStrParam, m_stepConfig.ch_6);
}

time_t CStep2Dlg::FetchExcuteTimeElapse()
{
	SYSTEMTIME stExcute = {};
	Str2Time(stExcute, m_stepConfig.ch_7);
	time_t tExcute = ConvertSecondsTime(stExcute);

	SYSTEMTIME st;
	GetLocalTime(&st);
	time_t tCur = ConvertSecondsTime(st);

	return tExcute - tCur;
}

void CStep2Dlg::OnBnClickedBtnStep2Pre()
{
	// TODO: 在此添加控件通知处理程序代码
	::PostMessage(m_hPWnd, UM_STEP1, NULL, NULL);
}

void CStep2Dlg::OnBnClickedBtnStep2Next()
{
	// TODO: 在此添加控件通知处理程序代码
	::PostMessage(m_hPWnd, UM_STEP3, NULL, NULL);
}

void CStep2Dlg::OnBnClickedBtnStep2Pathoringin()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHORINGIN))->SetWindowText(tchPath);
}

void CStep2Dlg::OnBnClickedBtnStep2Pathfk()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHFK))->SetWindowText(tchPath);
}

void CStep2Dlg::OnBnClickedBtnStep2Pathnation()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHNATION))->SetWindowText(tchPath);
}

void CStep2Dlg::OnBnClickedBtnStep2Pathrvt()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHRVT))->SetWindowText(tchPath);
}

void CStep2Dlg::OnBnClickedBtnStep2Pathdtvfb()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHDTVFB))->SetWindowText(tchPath);
}


void CStep2Dlg::OnBnClickedBtnStep2Pathtztfb()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHTZTFB))->SetWindowText(tchPath);
}

DWORD WINAPI OnLoadConfig2Thread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	STSTEP2CONFIG stData = {};

	FILE *fpRead = fopen("conf_step2.txt", "rb");
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

	pConfig = (char*)strtok(NULL, "\n");
	FilterConfig(stData.ch_3, pConfig);

	pConfig = (char*)strtok(NULL, "\n");
	FilterConfig(stData.ch_4, pConfig);

	pConfig = (char*)strtok(NULL, "\n");
	FilterConfig(stData.ch_5, pConfig);

	pConfig = (char*)strtok(NULL, "\n");
	FilterConfig(stData.ch_6, pConfig);

	pConfig = (char*)strtok(NULL, "\n");
	FilterConfig(stData.ch_7, pConfig);

	free(pContent);

	SendMessage(hWnd, UM_LOADCONFIG, (WPARAM)&stData, 0);

	return 0;
}

void CStep2Dlg::OnBnClickedBtnStep2Apply()
{
	// TODO: 在此添加控件通知处理程序代码

	memset(&m_stepConfig, 0, sizeof(m_stepConfig));

	SYSTEMTIME stTime;
	ZeroMemory(&stTime, sizeof(SYSTEMTIME));
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP2ENTER))->GetTime(&stTime);
	sprintf(m_stepConfig.ch_0, "%02u-%02u-%02u", stTime.wYear, stTime.wMonth, stTime.wDay);


	ZeroMemory(&stTime, sizeof(SYSTEMTIME));
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP2EXCUTE))->GetTime(&stTime);
	sprintf(m_stepConfig.ch_7, "%02u-%02u-%02u %02u:%02u:%02u", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	TCHAR tchConfig[256] = {};
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHORINGIN))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_1, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHFK))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_2, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHNATION))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_3, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHRVT))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_4, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHDTVFB))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_5, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP2_PATHTZTFB))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_6, "%s", tchConfig);

	FILE *pF = fopen("conf_step2.txt", "wb");
	fprintf(pF, "数据入库时间: \"%s\"\n", m_stepConfig.ch_0);
	fprintf(pF, "原始数据文件路径: \"%s\"\n", m_stepConfig.ch_1);
	fprintf(pF, "FK文件存放路径: \"%s\"\n", m_stepConfig.ch_2);
	fprintf(pF, "国标数据存放路径: \"%s\"\n", m_stepConfig.ch_3);
	fprintf(pF, "RVT数据存放路径: \"%s\"\n", m_stepConfig.ch_4);
	fprintf(pF, "DtVFb数据存放路径: \"%s\"\n", m_stepConfig.ch_5);
	fprintf(pF, "TzTFb数据存放路: \"%s\"\n", m_stepConfig.ch_6);
	fprintf(pF, "入库执行时间: \"%s\"", m_stepConfig.ch_7);
	fclose(pF);

	if (!CheckConfigEnterDateValid())
	{
		MessageBox(_T("入库数据日期错误！"), _T("提示"), MB_OK);
		return;
	}

	if (!CheckConfigExcuteDateTimeValid())
	{
		MessageBox(_T("入库执行时间错误！"), _T("提示"), MB_OK);
		return;
	}
}

bool CStep2Dlg::CheckConfigComplete()
{
	return (strlen(m_stepConfig.ch_1) > 0) && (strlen(m_stepConfig.ch_2) > 0) && (strlen(m_stepConfig.ch_3) > 0) && (strlen(m_stepConfig.ch_4) > 0) && (strlen(m_stepConfig.ch_5) > 0) && (strlen(m_stepConfig.ch_6) > 0);
}

bool CStep2Dlg::CheckConfigEnterDateValid()
{
	//配置的入库数据日期
	SYSTEMTIME stDate = {};
	Str2Time(stDate, m_stepConfig.ch_0, true);

	//今天
	SYSTEMTIME stToday;
	GetLocalTime(&stToday);

	//入库数据时间判断，入库的数据日期不能早于接收日期
	if (stDate.wYear < m_step1Date.wYear)
	{
		return false;
	}

	if (stDate.wYear == m_step1Date.wYear)
	{
		if (stDate.wMonth < m_step1Date.wMonth)
		{
			return false;
		}

		if (stDate.wMonth == m_step1Date.wMonth && stDate.wDay < m_step1Date.wDay)
		{
			return false;
		}
	}

	return true;
}

bool CStep2Dlg::CheckConfigExcuteDateTimeValid()
{
	//配置的入库数据日期
	SYSTEMTIME stDate = {};
	Str2Time(stDate, m_stepConfig.ch_0, true);

	//配置的入库执行时间
	SYSTEMTIME stDateTime = {};
	Str2Time(stDateTime, m_stepConfig.ch_7);

	//当前时间
	SYSTEMTIME stCur;
	GetLocalTime(&stCur);

	//入库执行时间判断，在接收日期之后
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

	//入库执行时间判断，在入库数据日期之后
	if (stDateTime.wYear < stDate.wYear)
	{
		return false;
	}

	if (stDateTime.wYear == stDate.wYear)
	{
		if (stDateTime.wMonth < stDate.wMonth)
		{
			return false;
		}

		if (stDateTime.wMonth == stDate.wMonth && stDateTime.wDay <= stDate.wDay)
		{
			return false;
		}
	}

	//入库执行时间判断，在当前时间之后
	time_t tDateTime = ConvertSecondsTime(stDateTime);
	time_t tCur = ConvertSecondsTime(stCur);
	if (tDateTime <= tCur)
	{
		return false;
	}

	return true;
}
