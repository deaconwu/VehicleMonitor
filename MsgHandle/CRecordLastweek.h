#pragma once

#include "InfoType.h"

class CRecordLastweek
{
public:
	CRecordLastweek() : m_iOnline(0), m_iOffline(0), m_iFault(0), m_iRecharge(0), m_iUnlocated(0)
	{

	}

	~CRecordLastweek()
	{
		if (NULL != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	static CRecordLastweek* GetInstance()
	{
		if (NULL == m_pInstance)
			m_pInstance = new CRecordLastweek;

		return m_pInstance;
	}

	void OnReset(UINT iOnline, UINT iOffline, UINT iFault, UINT iRecharge, UINT iUnlocated)
	{
		m_iOnline = iOnline;
		m_iOffline = iOffline;
		m_iFault = iFault;
		m_iRecharge = iRecharge;
		m_iUnlocated = iUnlocated;

		FILE *pF = fopen("RecLastWeek.dat", "wb");
		if (NULL == pF)
		{
			return;
		}

		fprintf(pF, "%u,%u,%u,%u,%u", m_iOnline, m_iOffline, m_iFault, m_iRecharge, m_iUnlocated);
		fclose(pF);
	}

	void OnFetch(STSTATISTICDATALASTWEEK& stData)
	{
		stData.iOnline = m_iOnline;
		stData.iOffline = m_iOffline;
		stData.iFault = m_iFault;
		stData.iRecharge = m_iRecharge;
		stData.iUnlocated = m_iUnlocated;
	}

private:
	UINT m_iOnline;
	UINT m_iOffline;
	UINT m_iFault;
	UINT m_iRecharge;
	UINT m_iUnlocated;

	static CRecordLastweek* m_pInstance;
};