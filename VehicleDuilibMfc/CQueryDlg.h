#pragma once

class QueryDlgUI : public WindowImplBase
{
public:
	QueryDlgUI() {}
	~QueryDlgUI() {}

	void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			CDuiString name = msg.pSender->GetName();
			CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("LabelF1_0")));
			if (name == _T("btnQuery"))
			{
				pLabel->SetText(_T("Í£³µ³äµç"));
			}
		}
	}

protected:
	virtual CDuiString GetSkinFolder() { return _T("skin"); };
	virtual CDuiString GetSkinFile() { return _T("QueryDlg.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T("QueryDlg_Wnd"); };
};