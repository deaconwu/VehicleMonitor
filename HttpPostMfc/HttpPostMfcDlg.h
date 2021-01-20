
// HttpPostMfcDlg.h: 头文件
//

#pragma once


// CHttpPostMfcDlg 对话框
class CHttpPostMfcDlg : public CDialogEx
{
// 构造
public:
	CHttpPostMfcDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HTTPPOSTMFC_DIALOG };
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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLaunch();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnPathupload();
	afx_msg void OnBnClickedBtnPathrecord();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	char m_chUrl[256];
	char m_chPathPost[256];
	char m_chPathRecord[256];

	SYSTEMTIME m_dateTimePost;	//每周期的执行时间
	BROWSEINFO m_bi;
};
