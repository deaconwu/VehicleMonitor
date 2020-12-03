#pragma once


// CVoltageData 对话框

class CVoltageData : public CDialogEx
{
	DECLARE_DYNAMIC(CVoltageData)

public:
	CVoltageData(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CVoltageData();

 	void LoadList(HWND hWnd);

 	LRESULT OnGetData(WPARAM wParam, LPARAM lParam);

	LRESULT OnLoadVehicle(WPARAM wParam, LPARAM lParam);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VOLTAGE };
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
	HWND m_hWndMain;

public:
	afx_msg void OnLvnColumnclickListVins(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListVins(NMHDR *pNMHDR, LRESULT *pResult);
};
