#pragma once

//#include <libloaderapi.h>

//HMODULE hModule = ::LoadLibraryA("DuiLib.dll");

class CHelloDuilibWnd : public WindowImplBase
{
public:
	CHelloDuilibWnd(void) {};
	~CHelloDuilibWnd(void) {};

	void Init(HWND wnd)
	{
		m_wnd = wnd;
	}

	void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			CDuiString name = msg.pSender->GetName();

			if (name == _T("optQuery"))
			{
				SendMessageA(m_wnd, UM_QUERY, 0, 0);
			}
			else if (name == _T("optStatistics"))
			{
				SendMessageA(m_wnd, UM_STATISTIC, 0, 0);
			}
		}
		else if (msg.sType == _T("textchanged"))
		{
			CDuiString name = msg.pSender->GetName();
			CEditUI* pEditIp = NULL;
			if (name == _T("editIp1") || name == _T("editIp2") || name == _T("editIp3") || name == _T("editIp4"))
			{
				pEditIp = static_cast<CEditUI*>(m_PaintManager.FindControl(name));
				CDuiString strContent = pEditIp->GetText();
				CDuiString strCorrect = _T("");

				int len = strContent.GetLength();

				for (int i = 0; i < len; i++)
				{
					TCHAR tch = strContent.GetAt(i);
					if (i != len - 1)
					{
						if (tch<_T('0') || tch>_T('9'))
						{

						}
						else
						{
							strCorrect.Append(&tch);
						}
					}
					else
					{
						if ((tch<_T('0') || tch>_T('9')) && (tch != _T('.')))
						{

						}
						else if ((len == 1 && tch != _T('.')) || len > 1)
						{
							strCorrect.Append(&tch);
						}
					}
				}

				len = strCorrect.GetLength();
				TCHAR chLast = strCorrect.GetAt(len - 1);

				if (strCorrect.GetLength() != strContent.GetLength())
				{
					pEditIp->SetText(strCorrect);
					pEditIp->SetSel(len, -1);
				}

				CDuiString nameNext = _T("editIp2");
				if (name == _T("editIp2"))
					nameNext = _T("editIp3");

				if (name == _T("editIp3"))
					nameNext = _T("editIp4");

				if (name == _T("editIp4"))
					nameNext = _T("editPort");


				if (len > 1 && chLast == _T('.'))
				{
					int pot = strCorrect.Find(_T("."));
					pEditIp->SetText(strCorrect.Left(pot));
					m_PaintManager.SetFocus(pEditIp, false);
					pEditIp = static_cast<CEditUI*>(m_PaintManager.FindControl(nameNext));
					pEditIp->SetFocus();
					pEditIp->SetSelAll();
				}
				else if (strCorrect.GetLength() == pEditIp->GetMaxChar())
				{
					pEditIp = static_cast<CEditUI*>(m_PaintManager.FindControl(nameNext));
					pEditIp->SetFocus();
					pEditIp->SetSelAll();
				}
			}
		}
	}

	//LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// 	{
	// 		LRESULT lRes = 0;
	// 		BOOL bHandled = TRUE;
	// 		switch (uMsg)
	// 		{
	// 		case WM_CREATE:
	// 		{
	// 			lRes = OnCreate(uMsg, wParam, lParam, bHandled);
	// 			break;
	// 		}
	// 		default:
	// 			bHandled = FALSE;
	// 		}
	// 
	// 		if (bHandled)
	// 			return lRes;
	// 
	// 		if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
	// 			return lRes;
	// 
	// 		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	// 	}
	// 
	// 	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	// 	{
	// 		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	// 		styleValue &= ~WS_CAPTION;
	// 		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	// 
	// 		m_PaintManager.Init(m_hWnd);
	// 		CDialogBuilder builder;
	// 		CDialogBuilderCallbackEx cb;
	// 		CControlUI* pRoot = builder.Create(_T("HelloDuilib.xml"), (UINT)0, &cb, &m_PaintManager);
	// 		ASSERT(pRoot && "Failed to parse XML");
	// 		m_PaintManager.AttachDialog(pRoot);
	// 		m_PaintManager.AddNotifier(this);
	// 
	// 		return 0;
	// 	}

protected:
	virtual CDuiString GetSkinFolder() { return _T("skin"); };
	virtual CDuiString GetSkinFile() { return _T("HelloDuilib.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T("HelloDuilib_Wnd"); };

private:
	HWND m_wnd;
	//CIpAddressUI m_ip;
};