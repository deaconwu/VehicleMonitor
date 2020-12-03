#include "pch.h"
#include "CHistoryRecord.h"
#include "CInfoRecord.h"

extern UCHAR* g_recMem;

CHistoryRecord* CHistoryRecord::m_pInstance = NULL;

CHistoryRecord::CHistoryRecord() : m_vehicleNumLastWeek(0), m_vehicleNumThisWeek(0)
{
}