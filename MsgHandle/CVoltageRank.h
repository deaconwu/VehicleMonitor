#pragma once

class CVoltageRank
{
public:
	~CVoltageRank() {}

	static CVoltageRank* GetInstance()
	{
		if (0 == m_pInstance)
			m_pInstance = new CVoltageRank;

		return m_pInstance;
	}

	void NotifyType(int iType);

	void OnStopRank();

	void OnLaunchRank(HWND hWnd, int iType=0);

	void OnSaveList(WORD wYear, WORD wMonth, WORD wDay);

private:
	CVoltageRank() {}

	HANDLE m_hThread;
	static CVoltageRank* m_pInstance;
};