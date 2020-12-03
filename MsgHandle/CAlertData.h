#pragma once


// CAlertData 对话框

class CAlertData : public CDialogEx
{
	DECLARE_DYNAMIC(CAlertData)

public:
	CAlertData(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAlertData();

	void LoadList();

	LRESULT OnLoadList(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetData(WPARAM wParam, LPARAM lParam);
	LRESULT OnLoadVin(WPARAM wParam, LPARAM lParam);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALERTDATA };
#endif

	void OnLauch();
	void OnDestroy();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listVins;

private:
	HANDLE m_hThread;
};
