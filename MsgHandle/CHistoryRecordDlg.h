#pragma once


// CHistoryRecordDlg 对话框

class CHistoryRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryRecordDlg)

public:
	CHistoryRecordDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHistoryRecordDlg();

	void OnLaunch(CString strVin, char chVin[], HWND hWnd);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTORYREC };
#endif

	void OnRefresh();

	afx_msg void OnClose();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listRec;

private:
	HWND m_hParentWnd;
};
