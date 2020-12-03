#ifndef __INFOTYPE_H__
#define __INFOTYPE_H__

#define BUFFER_SIZE 2000
#define MAX_FILE_SIZE 5000*79+4
#define VIN_LENGTH	17
#define MAX_VEHICLENUM 200000
#define RECNUM_PER_VEHICEL 64	//ÿ������ౣ��64��RecData
#define SHAREMEM_SIZE MAX_VEHICLENUM * RECNUM_PER_VEHICEL * sizeof(STRECDATA)
#define ALERT_CATEGORY_NUM 20
#define TIME_OFFLINE_DISTANCE 10 * 60	//����೤ʱ��δ����Ϣ��Ϊ���߳���(10����)
#define VOLTAGE_CATEGORY_NUM 7	//�����ѹ���� 0��1-2650��2651-4200��4201-15000��15001-65533��65534��65535
#define WARNING_CATEGORY_NUM 7	//Ƿѹ[1,2827]����ѹ[4250,15000]���쳣65534�����¡��쳣254����ѹ�������¶Ⱦ���
#define NUM_VINLOADED_PERTIME 1000
#define NUM_VINLOADED_FIRST 500

#pragma pack (1)

typedef struct RecData
{
	//��������
	UCHAR F1_0;       // 0__����״̬ 1
	UCHAR F1_1;       // 1__���״̬ 2
	UCHAR F1_2;       // 2__����ģʽ 3
	USHORT F1_3;      // 3__���� 5
	UINT F1_4;      // 5__�ۼ���� 9
	USHORT F1_5;      // 9__�ܵ�ѹ 11
	USHORT F1_6;      // 11__�ܵ��� 13
	UCHAR F1_7;       // 13__SOC 14
	UCHAR F1_8;       // 14__DC/DC״̬ 15
	UCHAR F1_9;       // 15__��λ 16
	USHORT F1_10;     // 16__��Ե���� 18
	UCHAR F1_11;      // 18__����̤���г�ֵ 19
	UCHAR F1_12;      // 19__�ƶ�̤��״̬ 20

	//�����������
	UCHAR F2_0;       // 0__�������״̬ 21
	UCHAR F2_1;       // 1__��������������¶� 22
	USHORT F2_2;      // 2__�������ת�� 24
	USHORT F2_3;      // 4__�������ת�� 26
	UCHAR F2_4;       // 6__��������¶� 27
	USHORT F2_5;      // 7__��������������ѹ 29
	USHORT F2_6;      // 9__ֱ��������ֱ��ĸ�ߵ��� 31

	//λ������
	UINT F5_0;      // 0__���� 35
	UINT F5_1;      // 4__γ�� 39
	UCHAR F5_2;       // 8__���㷽�� 40

	//��ֵ����
	USHORT F6_0;      // 0__��ߵ�ѹ����� 42
	USHORT F6_1;      // 2__��ص����ѹ���ֵ 44
	USHORT F6_2;      // 4__��͵�ѹ����� 46
	USHORT F6_3;      // 6__��ص����ѹ���ֵ 48
	UCHAR F6_4;       // 8__����¶�̽��� 49
	UCHAR F6_5;       // 9__����¶�ֵ 50
	UCHAR F6_6;       // 10__����¶�̽��� 51
	UCHAR F6_7;       // 11__����¶�ֵ 52

	//��������
	UINT F7_0; // 56

	//ʱ������
	UCHAR F8[6];    //������ʱ���� 62
} STRECDATA;

typedef struct StatisticData
{
	UINT iSumToday;
	UINT iOnlineToday;
	UINT iOfflineToday;
	UINT iDefaultToday;
	UINT iRechargeToday;
	ULONGLONG iMileageSum;
} STSTATISTICDATA;

typedef struct AlertData
{
	UINT iAlertTimes[MAX_VEHICLENUM];
	UINT iVoltageException[MAX_VEHICLENUM];
	UINT iTempException[MAX_VEHICLENUM];
} STALERTDATA;

typedef struct VoltageData
{
	UINT iValueTimes0[MAX_VEHICLENUM];	//0
	UINT iValueTimes1[MAX_VEHICLENUM];	//1-2650
	UINT iValueTimes2[MAX_VEHICLENUM];	//2651-4200
	UINT iValueTimes3[MAX_VEHICLENUM];	//4201-15000
	UINT iValueTimes4[MAX_VEHICLENUM];	//15001-65533
	UINT iValueTimes5[MAX_VEHICLENUM];	//65534
	UINT iValueTimes6[MAX_VEHICLENUM];	//65535
} STVOLTAGEDATA;

typedef struct VoltageRankSeq
{
	UCHAR chVin[VIN_LENGTH];
	UINT iValueTimes[VOLTAGE_CATEGORY_NUM];
} STVOLTAGERANKSEQ;

typedef struct MsgVoltageRankSeq
{
	UINT iNum;
	STVOLTAGERANKSEQ stNode[MAX_VEHICLENUM];	//�䳤�����MAX_VEHICLENUM
} STMSGVOLTAGERANKSEQ;

typedef struct VoltageRankLink
{
	UCHAR chVin[VIN_LENGTH];
	UINT iValueTimes[VOLTAGE_CATEGORY_NUM];
	struct VoltageRankLink* pPre;
	struct VoltageRankLink* pNext;
} STVOLTAGERANKLINK, *PSTVOLTAGERANKLINK;

typedef struct WarningData
{
	UINT iValueTimes0[MAX_VEHICLENUM];	//Ƿѹ[1,2827]
	UINT iValueTimes1[MAX_VEHICLENUM];	//��ѹ[4250,15000]
	UINT iValueTimes2[MAX_VEHICLENUM];	//�쳣65534
	UINT iValueTimes3[MAX_VEHICLENUM];	//����
	UINT iValueTimes4[MAX_VEHICLENUM];	//�쳣254
	UINT iValueTimes5[MAX_VEHICLENUM];	//��ѹ����
	UINT iValueTimes6[MAX_VEHICLENUM];	//�¶Ⱦ���
} STWARNINGDATA;

typedef struct WarningRankSeq
{
	UCHAR chVin[VIN_LENGTH+1];
	UINT iValueTimes[WARNING_CATEGORY_NUM];
} STWARNINGRANKSEQ;

typedef struct MsgWarningRankSeq
{
	UINT iNum;
	STWARNINGRANKSEQ stNode[MAX_VEHICLENUM];	//�䳤�����MAX_VEHICLENUM
} STMSGWARNINGRANKSEQ;

typedef struct WarningRankLink
{
	UCHAR chVin[VIN_LENGTH+1];
	UINT iValueTimes[WARNING_CATEGORY_NUM];
	struct WarningRankLink* pPre;
	struct WarningRankLink* pNext;
} STWARNINGRANKLINK, *PSTWARNINGRANKLINK;

typedef struct FileTime
{
	char strDir[100];
	time_t tNow;
} STFILETIME;

typedef struct FileDir
{
	char strDir[200];
} STFILEDIR;

#pragma pack()
#endif