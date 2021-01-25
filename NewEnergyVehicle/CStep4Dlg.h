#pragma once

#include "CStepDlgBase.h"

// CStep4Dlg 对话框

class CStep4Dlg : public CDialogEx, public CStepDlgBase
{
	DECLARE_DYNAMIC(CStep4Dlg)

public:
	CStep4Dlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStep4Dlg();

	bool CheckConfigComplete();

	void InputStep3DateTime(SYSTEMTIME& stDate) { memcpy(&m_step3DateTime, &stDate, sizeof(SYSTEMTIME)); }

	bool CheckConfigExcuteDateTimeValid();	//上传数据执行时间判断

	void FetchConfigExcuteDateTime(SYSTEMTIME& stDate);

	time_t FetchExcuteTimeElapse();

	void DisableCtrl();

	void ExcutePost();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STEP4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	LRESULT OnLoadConfig(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStep4Pre();
	afx_msg void OnBnClickedBtnStep4Pathupload();
	afx_msg void OnBnClickedBtnStep4Pathhistory();
	afx_msg void OnBnClickedBtnStep4Apply();

private:
	STSTEP4CONFIG m_stepConfig;
	SYSTEMTIME m_step3DateTime;	//配置的常态化点名执行时间
};
