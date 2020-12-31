#pragma once

#include "CStepDlgBase.h"

// CStep2Dlg 对话框

class CStep2Dlg : public CDialogEx, public CStepDlgBase
{
	DECLARE_DYNAMIC(CStep2Dlg)

public:
	CStep2Dlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStep2Dlg();

	bool CheckConfigComplete();

	bool CheckConfigEnterDateValid();	//入库数据日期判断

	bool CheckConfigExcuteDateTimeValid();	//入库执行时间判断

	void FetchConfigExcuteDateTime(SYSTEMTIME& stDate);

	time_t FetchExcuteTimeElapse();

	void DisableCtrl();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STEP2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	LRESULT OnLoadConfig(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStep2Pre();
	afx_msg void OnBnClickedBtnStep2Next();
	afx_msg void OnBnClickedBtnStep2Pathoringin();
	afx_msg void OnBnClickedBtnStep2Pathfk();
	afx_msg void OnBnClickedBtnStep2Pathnation();
	afx_msg void OnBnClickedBtnStep2Pathrvt();
	afx_msg void OnBnClickedBtnStep2Pathdtvfb();
	afx_msg void OnBnClickedBtnStep2Pathtztfb();

private:
	STSTEP2CONFIG m_stepConfig;
public:
	afx_msg void OnBnClickedBtnStep2Apply();
};
