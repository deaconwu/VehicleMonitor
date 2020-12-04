
// MsgHandleDlg.h: 头文件
//

#pragma once

#include "CQuery.h"
#include "CStatistics.h"
// #include "CAlertData.h"
// #include "CVoltageData.h"
#include "CWarningDlg.h"
#include "CInfoSocket.h"

// CMsgHandleDlg 对话框
class CMsgHandleDlg : public CDialogEx
{
// 构造
public:
	CMsgHandleDlg(CWnd* pParent = nullptr);	// 标准构造函数

	afx_msg LRESULT OnNetEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQueryVin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRefreshVin(WPARAM wParam, LPARAM lParam);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MSGHANDLE_DIALOG };
#endif

	void OnTerminate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	virtual BOOL OnInitDialog();


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnDisconnect();

private:
	bool OnConnect();

	CTabCtrl m_tab;
	CQuery m_query;
	CStatistics m_statistics;
// 	CAlertData m_alert;
// 	CVoltageData m_voltage;
	CWarningDlg m_warning;

	CInfoSocket* m_pInfoSocket;
public:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
};
