#ifndef _C_HISTORY_RECORD_H_
#define _C_HISTORY_RECORD_H_

#include "InfoType.h"

class CHistoryRecord
{
public:
	~CHistoryRecord()
	{
		//实例常驻内存，存在于进程的生命周期，无需手动释放
// 		if (NULL != m_pInstance)
// 		{
// 			delete m_pInstance;
// 			m_pInstance = NULL;
// 		}
	}

	static CHistoryRecord* GetInstance()
	{
		if (0 == m_pInstance)
			m_pInstance = new CHistoryRecord;

		return m_pInstance;
	}

private:
	CHistoryRecord();

	//上周数据
	long m_vehicleNumLastWeek;

	//本周数据
	long m_vehicleNumThisWeek;

	static CHistoryRecord* m_pInstance;
};

#endif
