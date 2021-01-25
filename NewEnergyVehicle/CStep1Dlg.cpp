// CStep1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "NewEnergyVehicle.h"
#include "CStep1Dlg.h"
#include "afxdialogex.h"

DWORD WINAPI OnLoadConfig1Thread(LPVOID lparam);

// CStep1Dlg 对话框

IMPLEMENT_DYNAMIC(CStep1Dlg, CDialogEx)

CStep1Dlg::CStep1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STEP1, pParent)
{
	m_bi.hwndOwner = this->m_hWnd;
	memset(&m_stepConfig, 0, sizeof(m_stepConfig));
}

CStep1Dlg::~CStep1Dlg()
{
}

void CStep1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STEP1RECV, m_dateTimeRecv);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STEP1SEND, m_dateTimeSend);
}

BOOL CStep1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_dateTimeRecv.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_dateTimeSend.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));

	DWORD dwThreadId;
	HANDLE hThread = CreateThread(NULL, NULL, OnLoadConfig1Thread, this->m_hWnd, 0, &dwThreadId);

	return TRUE;
}

void CStep1Dlg::DisableCtrl()
{
	((CButton*)GetDlgItem(IDC_BTN_STEP1_APPLY))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP1_PATH))->EnableWindow(false);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP1_PORTRECV))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP1_PORTSEND))->EnableWindow(false);

	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_STEP1RECV))->EnableWindow(false);
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_STEP1SEND))->EnableWindow(false);

	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP1RECV))->EnableWindow(false);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP1SEND))->EnableWindow(false);
}

BEGIN_MESSAGE_MAP(CStep1Dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_STEP1_APPLY, &CStep1Dlg::OnBnClickedBtnStep1Apply)
	ON_BN_CLICKED(IDC_BTN_STEP1_NEXT, &CStep1Dlg::OnBnClickedBtnStep1Next)
	ON_BN_CLICKED(IDC_BTN_STEP1_PATH, &CStep1Dlg::OnBnClickedBtnStep1Path)
	ON_MESSAGE(UM_LOADCONFIG, &CStep1Dlg::OnLoadConfig)
END_MESSAGE_MAP()


// CStep1Dlg 消息处理程序

LRESULT CStep1Dlg::OnLoadConfig(WPARAM wParam, LPARAM lParam)
{
	STSTEP1CONFIG* pMsg = (STSTEP1CONFIG*)wParam;

	memcpy(&m_stepConfig, pMsg, sizeof(STSTEP1CONFIG));

	((CEdit*)GetDlgItem(IDC_EDIT_STEP1_PATH))->SetWindowText(pMsg->ch_0);
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_STEP1RECV))->SetWindowText(pMsg->ch_1);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP1_PORTRECV))->SetWindowText(pMsg->ch_2);
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_STEP1SEND))->SetWindowText(pMsg->ch_3);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP1_PORTSEND))->SetWindowText(pMsg->ch_4);

	SYSTEMTIME stTime = {};
	Str2Time(stTime, pMsg->ch_5);
	m_dateTimeRecv.SetTime(stTime);

	Str2Time(stTime, pMsg->ch_6);
	m_dateTimeSend.SetTime(stTime);

	::PostMessage(m_hPWnd, UM_LOADSTEP1END, NULL, NULL);

	return 0;
}

void CStep1Dlg::OnBnClickedBtnStep1Apply()
{
	// TODO: 在此添加控件通知处理程序代码

	memset(&m_stepConfig, 0, sizeof(m_stepConfig));

	TCHAR tchConfig[256] = {};
	((CEdit*)GetDlgItem(IDC_EDIT_STEP1_PATH))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_0, "%s", tchConfig);

	STIPADDR ipAddr = {};
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_STEP1RECV))->GetAddress(ipAddr.ip1, ipAddr.ip2, ipAddr.ip3, ipAddr.ip4);
	sprintf(m_stepConfig.ch_1, "%u.%u.%u.%u", ipAddr.ip1, ipAddr.ip2, ipAddr.ip3, ipAddr.ip4);

	CString portStr = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT_STEP1_PORTRECV))->GetWindowText(portStr);
	int iPort = _ttoi(portStr);
	sprintf(m_stepConfig.ch_2, "%d", iPort);

	ipAddr = {};
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_STEP1SEND))->GetAddress(ipAddr.ip1, ipAddr.ip2, ipAddr.ip3, ipAddr.ip4);
	sprintf(m_stepConfig.ch_3, "%u.%u.%u.%u", ipAddr.ip1, ipAddr.ip2, ipAddr.ip3, ipAddr.ip4);

	portStr = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT_STEP1_PORTSEND))->GetWindowText(portStr);
	iPort = _ttoi(portStr);
	sprintf(m_stepConfig.ch_4, "%d", iPort);

	SYSTEMTIME stTime;
	ZeroMemory(&stTime, sizeof(SYSTEMTIME));
	m_dateTimeRecv.GetTime(&stTime);
	sprintf(m_stepConfig.ch_5, "%02u-%02u-%02u %02u:%02u:%02u", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	ZeroMemory(&stTime, sizeof(SYSTEMTIME));
	m_dateTimeSend.GetTime(&stTime);
	sprintf(m_stepConfig.ch_6, "%02u-%02u-%02u %02u:%02u:%02u", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	FILE *pF = fopen("conf_step1.txt", "wb");
	fprintf(pF, "原始数据路径: \"%s\"\n", m_stepConfig.ch_0);
	fprintf(pF, "接收端IP: \"%s\"\n", m_stepConfig.ch_1);
	fprintf(pF, "接收端Port: \"%s\"\n", m_stepConfig.ch_2);
	fprintf(pF, "发送端IP: \"%s\"\n", m_stepConfig.ch_3);
	fprintf(pF, "发送端Port: \"%s\"\n", m_stepConfig.ch_4);
	fprintf(pF, "接收数据时间: \"%s\"\n", m_stepConfig.ch_5);
	fprintf(pF, "发送数据时间: \"%s\"", m_stepConfig.ch_6);
	fclose(pF);

	if (!CheckConfigDateTimeValid())
	{
		MessageBox(_T("数据收发配置时间不能早于当前时间"), _T("提示"), MB_OK);
	}

	return;
}

bool CStep1Dlg::CheckConfigComplete()
{
	if (strlen(m_stepConfig.ch_0) == 0)
	{
		return false;
	}

	if (!memcmp(m_stepConfig.ch_1, "0.0.0.0", strlen("0.0.0.0"))
		|| !memcmp(m_stepConfig.ch_1, "0", strlen("0")))
	{
		return false;
	}

	if (!memcmp(m_stepConfig.ch_2, "0", strlen("0")))
	{
		return false;
	}

	if (!memcmp(m_stepConfig.ch_3, "0.0.0.0", strlen("0.0.0.0"))
		|| !memcmp(m_stepConfig.ch_3, "0", strlen("0")))
	{
		return false;
	}

	if (!memcmp(m_stepConfig.ch_4, "0", strlen("0")))
	{
		return false;
	}

	return (strlen(m_stepConfig.ch_5) > 0) && (strlen(m_stepConfig.ch_6) > 0);
}

bool CStep1Dlg::CheckConfigDateTimeValid()
{
	//数据收发时间在当前时间之后
	SYSTEMTIME stRecv = {};
	Str2Time(stRecv, m_stepConfig.ch_5);
	time_t tRecv = ConvertSecondsTime(stRecv);

	SYSTEMTIME stSend = {};
	Str2Time(stSend, m_stepConfig.ch_6);
	time_t tSend = ConvertSecondsTime(stSend);

	SYSTEMTIME st;
	GetLocalTime(&st);
	time_t tCur = ConvertSecondsTime(st);

	if (tCur>= tRecv || tCur>= tSend)
	{
		return false;
	}

	return true;
}

time_t CStep1Dlg::FetchExcuteTimeElapse()
{
	SYSTEMTIME stRecv = {};
	Str2Time(stRecv, m_stepConfig.ch_5);
	time_t tRecv = ConvertSecondsTime(stRecv);

	SYSTEMTIME st;
	GetLocalTime(&st);
	time_t tCur = ConvertSecondsTime(st);

	return tRecv - tCur;
}

void CStep1Dlg::FetchConfigDate(SYSTEMTIME& stDate)
{
	Str2Time(stDate, m_stepConfig.ch_5);
}

void CStep1Dlg::FetchTransParam(char* pStrParam)
{
	strcat(pStrParam, m_stepConfig.ch_0);
	strcat(pStrParam, " ");
	strcat(pStrParam, m_stepConfig.ch_1);
	strcat(pStrParam, " ");
	strcat(pStrParam, m_stepConfig.ch_2);
}

void CStep1Dlg::OnBnClickedBtnStep1Next()
{
	// TODO: 在此添加控件通知处理程序代码

	::PostMessage(m_hPWnd, UM_STEP2, NULL, NULL);
}


void CStep1Dlg::OnBnClickedBtnStep1Path()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP1_PATH))->SetWindowText(tchPath);
}

DWORD WINAPI OnLoadConfig1Thread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	STSTEP1CONFIG stData = {};

	FILE *fpRead = fopen("conf_step1.txt", "rb");
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

	free(pContent);

	SendMessage(hWnd, UM_LOADCONFIG, (WPARAM)&stData, 0);

	return 0;
}