// CStep3Dlg.cpp: 实现文件
//

#include "pch.h"
#include "NewEnergyVehicle.h"
#include "CStep3Dlg.h"
#include "afxdialogex.h"

DWORD WINAPI OnLoadConfig3Thread(LPVOID lparam);

// CStep3Dlg 对话框

IMPLEMENT_DYNAMIC(CStep3Dlg, CDialogEx)

CStep3Dlg::CStep3Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STEP3, pParent)
{
	m_bi.hwndOwner = this->m_hWnd;
	memset(&m_stepConfig, 0, sizeof(m_stepConfig));
}

CStep3Dlg::~CStep3Dlg()
{
}

void CStep3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CStep3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP3EXECUTE))->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PERIOD))->SetWindowText(_T("1"));

	DWORD dwThreadId;
	HANDLE hThread = CreateThread(NULL, NULL, OnLoadConfig3Thread, this->m_hWnd, 0, &dwThreadId);

	return TRUE;
}

void CStep3Dlg::DisableCtrl()
{
	((CButton*)GetDlgItem(IDC_BTN_STEP3_APPLY))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP3_PATHRVT))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP3_PATHDTVTB))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP3_PATHTZTFB))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP3_PATHDVDQ))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP3_PATHRESULT))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_STEP3_PATHRGB))->EnableWindow(false);

	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP3CALL))->EnableWindow(false);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP3EXECUTE))->EnableWindow(false);

	((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK2))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK5))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK6))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK7))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK8))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK9))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK10))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK11))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK12))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK13))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK14))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK15))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK16))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK17))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK18))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK19))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK20))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(false);
}

BEGIN_MESSAGE_MAP(CStep3Dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_STEP3_PRE, &CStep3Dlg::OnBnClickedBtnStep3Pre)
	ON_BN_CLICKED(IDC_BTN_STEP3_NEXT, &CStep3Dlg::OnBnClickedBtnStep3Next)
	ON_BN_CLICKED(IDC_BTN_STEP3_PATHRVT, &CStep3Dlg::OnBnClickedBtnStep3Pathrvt)
	ON_BN_CLICKED(IDC_BTN_STEP3_PATHDTVFB, &CStep3Dlg::OnBnClickedBtnStep3Pathdtvfb)
	ON_BN_CLICKED(IDC_BTN_STEP3_PATHDVDQ, &CStep3Dlg::OnBnClickedBtnStep3Pathdvdq)
	ON_BN_CLICKED(IDC_BTN_STEP3_PATHRESULT, &CStep3Dlg::OnBnClickedBtnStep3Pathresult)
	ON_BN_CLICKED(IDC_BTN_STEP3_APPLY, &CStep3Dlg::OnBnClickedBtnStep3Apply)
	ON_MESSAGE(UM_LOADCONFIG, &CStep3Dlg::OnLoadConfig)
	ON_BN_CLICKED(IDC_BTN_STEP3_PATHDTVTB, &CStep3Dlg::OnBnClickedBtnStep3Pathdtvtb)
	ON_BN_CLICKED(IDC_BTN_STEP3_PATHRGB, &CStep3Dlg::OnBnClickedBtnStep3Pathrgb)
	ON_BN_CLICKED(IDC_BTN_STEP3_PATHTZTFB, &CStep3Dlg::OnBnClickedBtnStep3Pathtztfb)
END_MESSAGE_MAP()


// CStep3Dlg 消息处理程序

LRESULT CStep3Dlg::OnLoadConfig(WPARAM wParam, LPARAM lParam)
{
	STSTEP3CONFIG* pMsg = (STSTEP3CONFIG*)wParam;

	memcpy(&m_stepConfig, pMsg, sizeof(STSTEP3CONFIG));

	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHRVT))->SetWindowText(pMsg->ch_1);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHDTVTB))->SetWindowText(pMsg->ch_2);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHTZTFB))->SetWindowText(pMsg->ch_3);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHDVDQ))->SetWindowText(pMsg->ch_4);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHRESULT))->SetWindowText(pMsg->ch_5);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHGB))->SetWindowText(pMsg->ch_6);

	CString csStr = _T("");
	csStr.Format(_T("%u"), pMsg->ch_8);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PERIOD))->SetWindowText(csStr);

	SYSTEMTIME stTime = {};
	Str2Time(stTime, pMsg->ch_0, true);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP3CALL))->SetTime(stTime);
	Str2Time(stTime, pMsg->ch_7);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP3EXECUTE))->SetTime(stTime);

	int iFlag = pMsg->ch_9 % 2;
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 2) % 2;
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 4) % 2;
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 8) % 2;
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 16) % 2;
	((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 32) % 2;
	((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 64) % 2;
	((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 128) % 2;
	((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 256) % 2;
	((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 512) % 2;
	((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 1024) % 2;
	((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 2048) % 2;
	((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 4096) % 2;
	((CButton*)GetDlgItem(IDC_CHECK13))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 8192) % 2;
	((CButton*)GetDlgItem(IDC_CHECK14))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 16384) % 2;
	((CButton*)GetDlgItem(IDC_CHECK15))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 32768) % 2;
	((CButton*)GetDlgItem(IDC_CHECK16))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 65536) % 2;
	((CButton*)GetDlgItem(IDC_CHECK17))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 131072) % 2;
	((CButton*)GetDlgItem(IDC_CHECK18))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 262144) % 2;
	((CButton*)GetDlgItem(IDC_CHECK19))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 524288) % 2;
	((CButton*)GetDlgItem(IDC_CHECK20))->SetCheck(iFlag);

	iFlag = (pMsg->ch_9 / 1048576) % 2;
	((CButton*)GetDlgItem(IDC_CHECK21))->SetCheck(iFlag);

	::PostMessage(m_hPWnd, UM_LOADSTEP3END, NULL, NULL);

	return 0;
}

void CStep3Dlg::OnBnClickedBtnStep3Pre()
{
	// TODO: 在此添加控件通知处理程序代码
	::PostMessage(m_hPWnd, UM_STEP2, NULL, NULL);
}


void CStep3Dlg::OnBnClickedBtnStep3Next()
{
	// TODO: 在此添加控件通知处理程序代码
	::PostMessage(m_hPWnd, UM_STEP4, NULL, NULL);
}


void CStep3Dlg::OnBnClickedBtnStep3Pathrvt()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHRVT))->SetWindowText(tchPath);
}


void CStep3Dlg::OnBnClickedBtnStep3Pathdtvtb()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHDTVTB))->SetWindowText(tchPath);
}

void CStep3Dlg::OnBnClickedBtnStep3Pathdtvfb()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHDTVFB))->SetWindowText(tchPath);
}

void CStep3Dlg::OnBnClickedBtnStep3Pathtztfb()
{
	// TODO: 在此添加控件通知处理程序代码

	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHTZTFB))->SetWindowText(tchPath);
}

void CStep3Dlg::OnBnClickedBtnStep3Pathdvdq()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHDVDQ))->SetWindowText(tchPath);
}


void CStep3Dlg::OnBnClickedBtnStep3Pathresult()
{
	// TODO: 在此添加控件通知处理程序代码
	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHRESULT))->SetWindowText(tchPath);
}


void CStep3Dlg::OnBnClickedBtnStep3Pathrgb()
{
	// TODO: 在此添加控件通知处理程序代码

	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHGB))->SetWindowText(tchPath);
}


void CStep3Dlg::OnBnClickedBtnStep3Apply()
{
	// TODO: 在此添加控件通知处理程序代码

	memset(&m_stepConfig, 0, sizeof(m_stepConfig));

	SYSTEMTIME stTime;
	ZeroMemory(&stTime, sizeof(SYSTEMTIME));
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP3CALL))->GetTime(&stTime);
	sprintf(m_stepConfig.ch_0, "%02u-%02u-%02u", stTime.wYear, stTime.wMonth, stTime.wDay);

	TCHAR tchConfig[256] = {};
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHRVT))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_1, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHDTVTB))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_2, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHTZTFB))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_3, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHDVDQ))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_4, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHRESULT))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_5, "%s", tchConfig);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PATHGB))->GetWindowText(tchConfig, 256);
	sprintf(m_stepConfig.ch_6, "%s", tchConfig);

	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STEP3EXECUTE))->GetTime(&stTime);
	sprintf(m_stepConfig.ch_7, "%02u-%02u-%02u %02u:%02u:%02u", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

	ZeroMemory(tchConfig, 256);
	((CEdit*)GetDlgItem(IDC_EDIT_STEP3_PERIOD))->GetWindowText(tchConfig, 256);
	m_stepConfig.ch_8 = atoi(tchConfig);

	int iFlag = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	m_stepConfig.ch_9 |= iFlag;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 2;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 4;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 8;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 16;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 32;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 64;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 128;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 256;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 512;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 1024;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 2048;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK13))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 4096;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 8192;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK15))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 16384;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK16))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 32768;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK17))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 65536;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK18))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 131072;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK19))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 262144;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK20))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 524288;

	iFlag = ((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck();
	m_stepConfig.ch_9 |= iFlag * 1048576;

	FILE *pF = fopen("conf_step3.txt", "wb");
	fprintf(pF, "点名时间: \"%s\"\n", m_stepConfig.ch_0);
	fprintf(pF, "RVT路径: \"%s\"\n", m_stepConfig.ch_1);
	fprintf(pF, "DtVTb路径: \"%s\"\n", m_stepConfig.ch_2);
	fprintf(pF, "TzTFb路径: \"%s\"\n", m_stepConfig.ch_3);
	fprintf(pF, "dVdQ路径: \"%s\"\n", m_stepConfig.ch_4);
	fprintf(pF, "结果保存路径: \"%s\"\n", m_stepConfig.ch_5);
	fprintf(pF, "GB数据路径: \"%s\"\n", m_stepConfig.ch_6);
	fprintf(pF, "执行时间: \"%s\"\n", m_stepConfig.ch_7);
	fprintf(pF, "点名周期: \"%u\"\n", m_stepConfig.ch_8);
	fprintf(pF, "点名类型标志: \"%u\"", m_stepConfig.ch_9);

	fclose(pF);

	if (!CheckConfigEnterDateValid())
	{
		MessageBox(_T("常态化点名数据日期错误！"), _T("提示"), MB_OK);
		return;
	}

	if (!CheckConfigExcuteDateTimeValid())
	{
		MessageBox(_T("常态化点名执行时间错误！"), _T("提示"), MB_OK);
		return;
	}
}

bool CStep3Dlg::CheckConfigComplete()
{
	return (strlen(m_stepConfig.ch_1) > 0) && (strlen(m_stepConfig.ch_2) > 0) && (strlen(m_stepConfig.ch_3) > 0) && (strlen(m_stepConfig.ch_4) > 0) && (strlen(m_stepConfig.ch_5) > 0) && (strlen(m_stepConfig.ch_6) > 0) && (m_stepConfig.ch_8 > 0);
}

void CStep3Dlg::FetchConfigDataDate(SYSTEMTIME& stDate)
{
	Str2Time(stDate, m_stepConfig.ch_0, true);
}

void CStep3Dlg::FetchConfigExcuteDateTime(SYSTEMTIME& stDate)
{
	Str2Time(stDate, m_stepConfig.ch_7);
}

bool CStep3Dlg::CheckConfigEnterDateValid()
{
	//配置的点名数据日期
	SYSTEMTIME stDate = {};
	Str2Time(stDate, m_stepConfig.ch_0, true);

	//点名数据日期判断，点名的数据日期不能早于接收日期
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

bool CStep3Dlg::CheckConfigExcuteDateTimeValid()
{
	//配置的点名数据日期
	SYSTEMTIME stDate = {};
	Str2Time(stDate, m_stepConfig.ch_0, true);

	//配置的点名执行时间
	SYSTEMTIME stDateTime = {};
	Str2Time(stDateTime, m_stepConfig.ch_7);

	//当前时间
	SYSTEMTIME stCur;
	GetLocalTime(&stCur);

	//点名执行时间判断，在数据接收日期之后
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

	//点名执行时间判断，在点名数据日期之后
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

	//点名执行时间判断，在当前时间之后
	time_t tDateTime = ConvertSecondsTime(stDateTime);
	time_t tCur = ConvertSecondsTime(stCur);
	if (tDateTime <= tCur)
	{
		return false;
	}

	//点名执行时间判断，在入库执行时间之后
	time_t tStep2 = ConvertSecondsTime(m_step2DateTime);
	if (tDateTime <= tStep2)
	{
		return false;
	}

	return true;
}

void CStep3Dlg::FetchTransParam(char* pStrParam)
{
	strcat(pStrParam, m_stepConfig.ch_0);
	strcat(pStrParam, " ");
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

time_t CStep3Dlg::FetchExcuteTimeElapse()
{
	SYSTEMTIME stExcute = {};
	Str2Time(stExcute, m_stepConfig.ch_7);
	time_t tExcute = ConvertSecondsTime(stExcute);

	SYSTEMTIME st;
	GetLocalTime(&st);
	time_t tCur = ConvertSecondsTime(st);

	return tExcute - tCur;
}

DWORD WINAPI OnLoadConfig3Thread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	STSTEP3CONFIG stData = {};

	FILE *fpRead = fopen("conf_step3.txt", "rb");
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

	char chInt[32] = {};

	pConfig = (char*)strtok(NULL, "\n");
	FilterConfig(chInt, pConfig);
	stData.ch_8 = atoi(chInt);

	ZeroMemory(chInt, 32);

	pConfig = (char*)strtok(NULL, "\n");
	FilterConfig(chInt, pConfig);
	stData.ch_9 = atoi(chInt);

	free(pContent);

	SendMessage(hWnd, UM_LOADCONFIG, (WPARAM)&stData, 0);

	return 0;
}