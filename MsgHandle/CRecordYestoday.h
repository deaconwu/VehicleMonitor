#pragma once

#include "InfoType.h"

class CRecordYestoday
{
public:

	~CRecordYestoday()
	{
		//实例常驻内存，存在于进程的生命周期，无需手动释放
// 		if (NULL != m_pInstance)
// 		{
// 			delete m_pInstance;
// 			m_pInstance = NULL;
// 		}
	}

	static CRecordYestoday* GetInstance()
	{
		if (NULL == m_pInstance)
			m_pInstance = new CRecordYestoday;

		return m_pInstance;
	}

	void OnReset(UINT joinNum, UINT chargeNum, UINT chargeTimes, ULONGLONG chargeSeconds)
	{
		m_joinNum = joinNum;
		m_chargeNum = chargeNum;
		m_chargeTimes = chargeTimes;
		m_chargeSeconds = chargeSeconds;

		FILE *pF = fopen("RecYestoday.dat", "wb");
		if (NULL == pF)
		{
			return;
		}

		fprintf(pF, "%u,%u,%u,%llu", m_joinNum, m_chargeNum, m_chargeTimes, m_chargeSeconds);
		fclose(pF);
	}

	void OnFetch(STSTATISTICDATAYESTODAY& stData)
	{
		stData.iJoin = m_joinNum;
		stData.iRechargeNum = m_chargeNum;
		stData.iRechargeTimes = m_chargeTimes;
		stData.iRechargeSeconds = m_chargeSeconds;
	}

private:

	CRecordYestoday() : m_joinNum(0), m_chargeNum(0), m_chargeTimes(0), m_chargeSeconds(0)
	{

	}

	UINT m_joinNum;
	UINT m_chargeNum;
	UINT m_chargeTimes;
	ULONGLONG m_chargeSeconds;

	static CRecordYestoday* m_pInstance;
};