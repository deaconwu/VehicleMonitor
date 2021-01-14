#pragma once

class CWarningRank
{
public:

	~CWarningRank() {}

	static CWarningRank* GetInstance()
	{
		if (0 == m_pInstance)
			m_pInstance = new CWarningRank;

		return m_pInstance;
	}

	void NotifyType(int iType);

	void OnStopRank();

	void OnLaunchRank(HWND hWnd, int iType = 0);

	void OnSaveList(WORD wYear, WORD wMonth, WORD wDay);

private:
	CWarningRank() {}

	HANDLE m_hThread;
	static CWarningRank* m_pInstance;
};