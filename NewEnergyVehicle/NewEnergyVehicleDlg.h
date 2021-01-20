
// NewEnergyVehicleDlg.h: 头文件
//

#pragma once

#include "CLoginDlg.h"
#include "CStep1Dlg.h"
#include "CStep2Dlg.h"
#include "CStep3Dlg.h"
#include "CStep4Dlg.h"

// CNewEnergyVehicleDlg 对话框
class CNewEnergyVehicleDlg : public CDialogEx
{
// 构造
public:
	CNewEnergyVehicleDlg(CWnd* pParent = nullptr);	// 标准构造函数

	void KillAllTimer();

	void KillAttachProcess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWENERGYVEHICLE_DIALOG };
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

	BOOL PreTranslateMessage(MSG* pMsg);

	LRESULT OnLoginIn(WPARAM wParam, LPARAM lParam);
	LRESULT OnStep1(WPARAM wParam, LPARAM lParam);
	LRESULT OnStep2(WPARAM wParam, LPARAM lParam);
	LRESULT OnStep3(WPARAM wParam, LPARAM lParam);
	LRESULT OnStep4(WPARAM wParam, LPARAM lParam);

	LRESULT OnLoadStep1End(WPARAM wParam, LPARAM lParam);
	LRESULT OnLoadStep2End(WPARAM wParam, LPARAM lParam);
	LRESULT OnLoadStep3End(WPARAM wParam, LPARAM lParam);

	LRESULT OnHttpPostEnd(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CLoginDlg m_loginDlg;
	CStep1Dlg m_step1Dlg;
	CStep2Dlg m_step2Dlg;
	CStep3Dlg m_step3Dlg;
	CStep4Dlg m_step4Dlg;

	//每周期的数据日期
	SYSTEMTIME m_dataDateStep2;
	SYSTEMTIME m_dataDateStep3;

	//每周期的执行时间
	SYSTEMTIME m_dateTimeStep2;
	SYSTEMTIME m_dateTimeStep3;
	SYSTEMTIME m_dateTimeStep4;

public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedBtnLaunch();
	afx_msg void OnBnClickedBtnQuit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
