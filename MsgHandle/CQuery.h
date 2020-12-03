#pragma once


// CQuery 对话框

class CQuery : public CDialogEx
{
	DECLARE_DYNAMIC(CQuery)

public:
	CQuery(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CQuery();

	void OnVinInput(char* pVin, bool bEditSet=true);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnQuery();
};
