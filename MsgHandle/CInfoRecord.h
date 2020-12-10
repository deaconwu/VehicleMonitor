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

	void SaveTxt();

	bool ReadVin();

	void WriteVin();

	INT FindVinPos(UCHAR pVin[]);

	INT FindVinPosEx(UCHAR pVin[]);

	void InsertRec(INT pos, char* pBuf);

	void OnParse(char buf[], int len);

	void OnStatisticToday(STSTATISTICDATATODAY &stData);

	void OnMileageRank(STMSGMILEAGERANKSEQ &msgSeq);

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

	void MoveToYestoday();

	void MoveToLastWeek();

	void InsertVinAndSort(UCHAR pVin[]);

private:
	UCHAR m_chVinEx[MAX_VEHICLENUM][VIN_LENGTH];
	UCHAR m_chVin[MAX_VEHICLENUM][VIN_LENGTH]; //ÿ����vin��
	STRECDATA* m_pVehicleRec[MAX_VEHICLENUM];
	UCHAR m_iCurRecIndex[MAX_VEHICLENUM];
	UINT m_vehicleNum;
	UINT m_vehicleNumEx;
	UINT m_alertTimes[MAX_VEHICLENUM][ALERT_CATEGORY_NUM];	//ÿ�������౨��������0:�ܱ�������

	UINT m_voltageException[MAX_VEHICLENUM];
	UINT m_tempException[MAX_VEHICLENUM];

	UINT m_voltageValueTimes[VOLTAGE_CATEGORY_NUM][MAX_VEHICLENUM];

	UINT m_warningValueTimes[WARNING_CATEGORY_NUM][MAX_VEHICLENUM];

	UCHAR m_bTodayJoin[MAX_VEHICLENUM];	//���ս���ĳ�
	SYSTEMTIME m_sTime[MAX_VEHICLENUM];	//���½���ʱ��

	UINT m_chargeTimes;	//�ۼƳ�����
	ULONGLONG m_chargeSeconds;	//�ۼƳ��ʱ��(����������¼Ϊ���״̬������ɼ�ʱ����)

	UCHAR m_chRefreshVin[VIN_LENGTH];

	HWND m_hWndMain;

	static CInfoRecord* m_pInstance;
};

#endif
