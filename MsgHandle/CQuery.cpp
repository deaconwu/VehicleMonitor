// CQuery.cpp: 实现文件
//

#include "pch.h"
#include "MsgHandle.h"
#include "CQuery.h"
#include "afxdialogex.h"
#include "CInfoRecord.h"

// CQuery 对话框

IMPLEMENT_DYNAMIC(CQuery, CDialogEx)

CQuery::CQuery(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QUERY, pParent)
{

}

CQuery::~CQuery()
{
}

void CQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CQuery::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//整车
	((CEdit*)GetDlgItem(IDC_EDIT_F1_0))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_1))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_2))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_3))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_4))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_5))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_6))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_7))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_8))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_9))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_10))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_11))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_12))->EnableWindow(false);

	//驱动电机
	((CEdit*)GetDlgItem(IDC_EDIT_F2_0))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_1))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_2))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_3))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_4))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_5))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_6))->EnableWindow(false);

	//位置
	((CEdit*)GetDlgItem(IDC_EDIT_F5_0))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F5_1))->EnableWindow(false);

	//极值
	((CEdit*)GetDlgItem(IDC_EDIT_F6_0))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_1))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_2))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_3))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_4))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_5))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_6))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_7))->EnableWindow(false);

	//报警
	((CButton*)GetDlgItem(IDC_EDIT_F7_0))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_1))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_2))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_3))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_4))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_5))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_6))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_7))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_8))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_9))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_10))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_11))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_12))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_13))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_14))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_15))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_16))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_17))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_18))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_EDIT_F7_19))->EnableWindow(false);

	//采集时间
	((CEdit*)GetDlgItem(IDC_EDIT_F8))->EnableWindow(false);

	return TRUE;
}

void CQuery::OnVinInput(char* pVin, bool bEditSet)
{
	STRECDATA stData;
	memset(&stData, 0, sizeof(stData));

	UINT alertTimes[ALERT_CATEGORY_NUM] = {};

	bool bRet = CInfoRecord::GetInstance()->QueryLatestInfo((UCHAR*)pVin, stData, alertTimes);
	if (!bRet)
	{
		MessageBox(_T("该车辆不存在"), _T("提示"), MB_OK | MB_ICONERROR);
		return;
	}

	if (bEditSet)
	{
		TCHAR tchVin[VIN_LENGTH+1] = {};

#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, pVin, -1, tchVin, VIN_LENGTH);
#else
		strcpy(tchVin, pVin);
#endif

		((CEdit*)GetDlgItem(IDC_EDIT_VINCODE))->SetWindowText(tchVin);
	}

	CString csStr;

	//采集时间
	csStr.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"), stData.F8[0], stData.F8[1], stData.F8[2], stData.F8[3], stData.F8[4], stData.F8[5]);
	((CEdit*)GetDlgItem(IDC_EDIT_F8))->SetWindowText(csStr);

	//车辆状态
	if (stData.F1_0 == 1)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_0))->SetWindowText(_T("启动"));
	}
	else if (stData.F1_0 == 2)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_0))->SetWindowText(_T("熄火"));
	}
	else if (stData.F1_0 == 3)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_0))->SetWindowText(_T("其它"));
	}
	else if (stData.F1_0 == 254)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_0))->SetWindowText(_T("异常"));
	}
	else if (stData.F1_0 == 255)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_0))->SetWindowText(_T("无效"));
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_0))->SetWindowText(_T(""));
	}

	//充电状态
	if (stData.F1_1 == 1)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_1))->SetWindowText(_T("停车充电"));
	}
	else if (stData.F1_1 == 2)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_1))->SetWindowText(_T("行驶充电"));
	}
	else if (stData.F1_1 == 3)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_1))->SetWindowText(_T("未充电"));
	}
	else if (stData.F1_1 == 4)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_1))->SetWindowText(_T("充电完成"));
	}
	else if (stData.F1_1 == 254)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_1))->SetWindowText(_T("异常"));
	}
	else if (stData.F1_1 == 255)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_1))->SetWindowText(_T("无效"));
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_1))->SetWindowText(_T(""));
	}

	//运行模式
	if (stData.F1_2 == 1)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_2))->SetWindowText(_T("纯电"));
	}
	else if (stData.F1_2 == 2)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_2))->SetWindowText(_T("混动"));
	}
	else if (stData.F1_2 == 3)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_2))->SetWindowText(_T("燃油"));
	}
	else if (stData.F1_2 == 254)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_2))->SetWindowText(_T("异常"));
	}
	else if (stData.F1_2 == 255)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_2))->SetWindowText(_T("无效"));
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_2))->SetWindowText(_T(""));
	}

	//车速
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F1_3);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_3))->SetWindowText(csStr);

	//累计里程
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F1_4);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_4))->SetWindowText(csStr);

	//总电压
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F1_5);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_5))->SetWindowText(csStr);

	//总电流
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F1_6);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_6))->SetWindowText(csStr);

	//SOC
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F1_7);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_7))->SetWindowText(csStr);

	//DC/DC状态
	if (stData.F1_8 == 1)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_8))->SetWindowText(_T("工作"));
	}
	else if (stData.F1_8 == 2)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_8))->SetWindowText(_T("断开"));
	}
	else if (stData.F1_8 == 254)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_8))->SetWindowText(_T("异常"));
	}
	else if (stData.F1_8 == 255)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_8))->SetWindowText(_T("无效"));
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F1_8))->SetWindowText(_T(""));
	}

	//挡位
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F1_9);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_9))->SetWindowText(csStr);

	//绝缘电阻
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F1_10);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_10))->SetWindowText(csStr);

	//加速踏板行程值
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F1_11);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_11))->SetWindowText(csStr);

	//制动踏板状态
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F1_12);
	((CEdit*)GetDlgItem(IDC_EDIT_F1_12))->SetWindowText(csStr);

	//电机状态
	if (stData.F2_0 == 1)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F2_0))->SetWindowText(_T("耗电"));
	}
	else if (stData.F2_0 == 2)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F2_0))->SetWindowText(_T("发电"));
	}
	else if (stData.F2_0 == 3)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F2_0))->SetWindowText(_T("关闭"));
	}
	else if (stData.F2_0 == 4)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F2_0))->SetWindowText(_T("准备"));
	}
	else if (stData.F2_0 == 254)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F2_0))->SetWindowText(_T("异常"));
	}
	else if (stData.F2_0 == 255)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F2_0))->SetWindowText(_T("无效"));
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_F2_0))->SetWindowText(_T(""));
	}

	//驱动电机控制器温度
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F2_1);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_1))->SetWindowText(csStr);

	//驱动电机转速
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F2_2);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_2))->SetWindowText(csStr);

	//驱动电机转矩
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F2_3);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_3))->SetWindowText(csStr);

	//驱动电机温度
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F2_4);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_4))->SetWindowText(csStr);

	//电机控制器输入电压
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F2_5);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_5))->SetWindowText(csStr);

	//直流控制器直流母线电流
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F2_6);
	((CEdit*)GetDlgItem(IDC_EDIT_F2_6))->SetWindowText(csStr);

	//经度
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F5_0);
	((CEdit*)GetDlgItem(IDC_EDIT_F5_0))->SetWindowText(csStr);

	//纬度
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F5_1);
	((CEdit*)GetDlgItem(IDC_EDIT_F5_1))->SetWindowText(csStr);

	//最高电压单体号
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F6_0);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_0))->SetWindowText(csStr);

	//电池单体电压最高值
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F6_1);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_1))->SetWindowText(csStr);

	//最低电压单体号
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F6_2);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_2))->SetWindowText(csStr);

	//电池单体电压最低值
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F6_3);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_3))->SetWindowText(csStr);

	//最高温度探针号
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F6_4);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_4))->SetWindowText(csStr);

	//最高温度值
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F6_5);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_5))->SetWindowText(csStr);

	//最低温度探针号
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F6_6);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_6))->SetWindowText(csStr);

	//最低温度值
	csStr = _T("");
	csStr.Format(_T("%u"), stData.F6_7);
	((CEdit*)GetDlgItem(IDC_EDIT_F6_7))->SetWindowText(csStr);

	//报警总次数
	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[0]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_0))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_1))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[2]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_2))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[3]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_3))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[4]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_4))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[5]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_5))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[6]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_6))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[7]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_7))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[8]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_8))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[9]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_9))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[10]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_10))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[11]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_11))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[12]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_12))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[13]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_13))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[14]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_14))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[15]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_15))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[16]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_16))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[17]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_17))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[18]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_18))->SetWindowText(csStr);

	csStr = _T("");
	csStr.Format(_T("%u"), alertTimes[19]);
	((CEdit*)GetDlgItem(IDC_EDIT_F7_19))->SetWindowText(csStr);
}

BEGIN_MESSAGE_MAP(CQuery, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_QUERY, &CQuery::OnBnClickedBtnQuery)
END_MESSAGE_MAP()


// CQuery 消息处理程序


void CQuery::OnBnClickedBtnQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR tchVin[VIN_LENGTH+1] = {};
	((CEdit*)GetDlgItem(IDC_EDIT_VINCODE))->GetWindowText(tchVin, VIN_LENGTH+1);

	char chVin[VIN_LENGTH+1] = {};

	int iLength = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)&tchVin, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)&tchVin, -1, chVin, iLength, NULL, NULL);

	OnVinInput(chVin, false);
}
