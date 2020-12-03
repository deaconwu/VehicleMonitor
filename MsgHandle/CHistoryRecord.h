#ifndef _C_HISTORY_RECORD_H_
#define _C_HISTORY_RECORD_H_

#include "InfoType.h"

class CHistoryRecord
{
public:
	CHistoryRecord();
	~CHistoryRecord()
	{
		if (NULL != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	static CHistoryRecord* GetInstance()
	{
		if (0 == m_pInstance)
			m_pInstance = new CHistoryRecord;

		return m_pInstance;
	}

private:

	//上周数据
	long m_vehicleNumLastWeek;

	//本周数据
	long m_vehicleNumThisWeek;

	static CHistoryRecord* m_pInstance;
};

#endif
