#ifndef __CINFORECORD_H__
#define __CINFORECORD_H__

#include "InfoType.h"

class CInfoRecord
{
public:
	CInfoRecord();
	~CInfoRecord()
	{
		if (NULL != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	static CInfoRecord* GetInstance()
	{
		if (NULL == m_pInstance)
			m_pInstance = new CInfoRecord;

		return m_pInstance;
	}

	void OnReset();

	void SaveBin();

	bool ReadVin();

	INT FindVinPos(UCHAR pVin[]);

	void InsertRec(INT pos, char* pBuf);

	void OnParse(char buf[], int len);

	void OnStatistic(STSTATISTICDATA &stData);

	void OnAlert(STALERTDATA &stData);

	void OnVoltage(STVOLTAGEDATA &stData);

	void OnWarning(STWARNINGDATA &stData);

	bool QueryLatestInfo(UCHAR pVin[], STRECDATA &stData, UINT m_alertTimes[]);

	void QueryHistoryInfo(UCHAR pVin[], STRECDATA arrRec[], UCHAR& iIndex);

	UINT GetVins(UCHAR chVin[][VIN_LENGTH]);

	UINT GetVehicleNum() { return m_vehicleNum; }

	void SetHwnd(HWND hWnd) { m_hWndMain = hWnd; }

	void SetRefreshVin(UCHAR* pVin) { memcpy(m_chRefreshVin, pVin, VIN_LENGTH); }

	void UnSetRefreshVin() { memset(m_chRefreshVin, 0, VIN_LENGTH); }

	void OnTerminate();

private:

	UCHAR m_chVin[MAX_VEHICLENUM][VIN_LENGTH]; //每辆车vin码
	STRECDATA* m_pVehicleRec[MAX_VEHICLENUM];
	UCHAR m_iCurRecIndex[MAX_VEHICLENUM];
	UINT m_vehicleNum;
	UINT m_alertTimes[MAX_VEHICLENUM][ALERT_CATEGORY_NUM];	//每辆车各类报警次数，0:总报警次数

	UINT m_voltageException[MAX_VEHICLENUM];
	UINT m_tempException[MAX_VEHICLENUM];

	UINT m_voltageValueTimes[VOLTAGE_CATEGORY_NUM][MAX_VEHICLENUM];

	UINT m_warningValueTimes[WARNING_CATEGORY_NUM][MAX_VEHICLENUM];

	UCHAR m_bTodayJoin[MAX_VEHICLENUM];	//今日接入的车
	SYSTEMTIME m_sTime[MAX_VEHICLENUM];	//最新接收时间

	UCHAR m_chRefreshVin[VIN_LENGTH];

	HWND m_hWndMain;

	static CInfoRecord* m_pInstance;
};

#endif
