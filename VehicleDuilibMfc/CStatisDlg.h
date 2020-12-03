#pragma once

class StatisDlgUI : public WindowImplBase
{
public:
	StatisDlgUI() {}
	~StatisDlgUI() {}

	void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			CDuiString name = msg.pSender->GetName();
		}
	}

protected:
	virtual CDuiString GetSkinFolder() { return _T("skin"); };
	virtual CDuiString GetSkinFile() { return _T("StatisticsDlg.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T("Statistics_Wnd"); };
};