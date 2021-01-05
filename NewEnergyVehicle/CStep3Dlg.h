#pragma once

#include "CStepDlgBase.h"

// CStep3Dlg 对话框

class CStep3Dlg : public CDialogEx, public CStepDlgBase
{
	DECLARE_DYNAMIC(CStep3Dlg)

public:
	CStep3Dlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStep3Dlg();

	bool CheckConfigComplete();

	void FetchConfigDataDate(SYSTEMTIME& stDate);

	void FetchConfigExcuteDateTime(SYSTEMTIME& stDate);

	void InputStep2DateTime(SYSTEMTIME& stDate) { memcpy(&m_step2DateTime, &stDate, sizeof(SYSTEMTIME)); }

	bool CheckConfigEnterDateValid();	//常态化点名数据日期判断

	bool CheckConfigExcuteDateTimeValid();	//常态化点名执行时间判断

	time_t FetchExcuteTimeElapse();

	void FetchTransParam(char* pStrParam);

	void DisableCtrl();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STEP3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	LRESULT OnLoadConfig(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStep3Pre();
	afx_msg void OnBnClickedBtnStep3Next();
	afx_msg void OnBnClickedBtnStep3Pathrvt();
	afx_msg void OnBnClickedBtnStep3Pathdtvfb();
	afx_msg void OnBnClickedBtnStep3Pathdvdq();
	afx_msg void OnBnClickedBtnStep3Pathresult();
	afx_msg void OnBnClickedBtnStep3Apply();
	afx_msg void OnBnClickedBtnStep3Pathdtvtb();
	afx_msg void OnBnClickedBtnStep3Pathrgb();
	afx_msg void OnBnClickedBtnStep3Pathtztfb();

private:
	STSTEP3CONFIG m_stepConfig;
	SYSTEMTIME m_step2DateTime;	//配置的入库执行时间
};
