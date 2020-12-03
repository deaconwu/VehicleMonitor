
// VehicleDuilibMfcDlg.h: 头文件
//

#pragma once

#include "UserMessage.h"
#include "CHelloDuilibWnd.h"
#include "CQueryDlg.h"
#include "CStatisDlg.h"

// CVehicleDuilibMfcDlg 对话框
class CVehicleDuilibMfcDlg : public CDialogEx
{
// 构造
public:
	CVehicleDuilibMfcDlg(CWnd* pParent = nullptr);	// 标准构造函数

	afx_msg LRESULT OnTabQuery(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTabStatis(WPARAM wParam, LPARAM lParam);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VEHICLEDUILIBMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CHelloDuilibWnd m_duiwnd;
	QueryDlgUI m_query;
	StatisDlgUI m_statis;
};
