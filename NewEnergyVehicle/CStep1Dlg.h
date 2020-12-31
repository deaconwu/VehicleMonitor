#pragma once

#include "CStepDlgBase.h"

// CStep1Dlg 对话框

class CStep1Dlg : public CDialogEx , public CStepDlgBase
{
	DECLARE_DYNAMIC(CStep1Dlg)

public:
	CStep1Dlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStep1Dlg();

	bool CheckConfigComplete();

	bool CheckConfigDateTimeValid();

	time_t FetchExcuteTimeElapse();

	void FetchConfigDate(SYSTEMTIME& stDate);

	void DisableCtrl();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STEP1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	LRESULT OnLoadConfig(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnStep1Apply();

public:
	afx_msg void OnBnClickedBtnStep1Next();
	afx_msg void OnBnClickedBtnStep1Path();

private:
	CDateTimeCtrl m_dateTimeRecv;
	CDateTimeCtrl m_dateTimeSend;

	STSTEP1CONFIG m_stepConfig;
};
