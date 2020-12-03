#ifndef __INFOTYPE_H__
#define __INFOTYPE_H__

#define BUFFER_SIZE 2000
#define MAX_FILE_SIZE 5000*79+4
#define VIN_LENGTH	17
#define MAX_VEHICLENUM 200000
#define RECNUM_PER_VEHICEL 64	//每辆车最多保留64条RecData
#define SHAREMEM_SIZE MAX_VEHICLENUM * RECNUM_PER_VEHICEL * sizeof(STRECDATA)
#define ALERT_CATEGORY_NUM 20
#define TIME_OFFLINE_DISTANCE 10 * 60	//距离多长时间未发信息视为离线车辆(10分钟)
#define VOLTAGE_CATEGORY_NUM 7	//单体电压分类 0、1-2650、2651-4200、4201-15000、15001-65533、65534、65535
#define WARNING_CATEGORY_NUM 7	//欠压[1,2827]、过压[4250,15000]、异常65534、过温、异常254、电压警报、温度警报
#define NUM_VINLOADED_PERTIME 1000
#define NUM_VINLOADED_FIRST 500

#pragma pack (1)

typedef struct RecData
{
	//整车数据
	UCHAR F1_0;       // 0__车辆状态 1
	UCHAR F1_1;       // 1__充电状态 2
	UCHAR F1_2;       // 2__运行模式 3
	USHORT F1_3;      // 3__车速 5
	UINT F1_4;      // 5__累计里程 9
	USHORT F1_5;      // 9__总电压 11
	USHORT F1_6;      // 11__总电流 13
	UCHAR F1_7;       // 13__SOC 14
	UCHAR F1_8;       // 14__DC/DC状态 15
	UCHAR F1_9;       // 15__挡位 16
	USHORT F1_10;     // 16__绝缘电阻 18
	UCHAR F1_11;      // 18__加速踏板行程值 19
	UCHAR F1_12;      // 19__制动踏板状态 20

	//驱动电机数据
	UCHAR F2_0;       // 0__驱动电机状态 21
	UCHAR F2_1;       // 1__驱动电机控制器温度 22
	USHORT F2_2;      // 2__驱动电机转速 24
	USHORT F2_3;      // 4__驱动电机转矩 26
	UCHAR F2_4;       // 6__驱动电机温度 27
	USHORT F2_5;      // 7__电机控制器输入电压 29
	USHORT F2_6;      // 9__直流控制器直流母线电流 31

	//位置数据
	UINT F5_0;      // 0__经度 35
	UINT F5_1;      // 4__纬度 39
	UCHAR F5_2;       // 8__推算方向 40

	//极值数据
	USHORT F6_0;      // 0__最高电压单体号 42
	USHORT F6_1;      // 2__电池单体电压最高值 44
	USHORT F6_2;      // 4__最低电压单体号 46
	USHORT F6_3;      // 6__电池单体电压最低值 48
	UCHAR F6_4;       // 8__最高温度探针号 49
	UCHAR F6_5;       // 9__最高温度值 50
	UCHAR F6_6;       // 10__最低温度探针号 51
	UCHAR F6_7;       // 11__最低温度值 52

	//报警数据
	UINT F7_0; // 56

	//时间数据
	UCHAR F8[6];    //年月日时分秒 62
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
	STVOLTAGERANKSEQ stNode[MAX_VEHICLENUM];	//变长，最多MAX_VEHICLENUM
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
	UINT iValueTimes0[MAX_VEHICLENUM];	//欠压[1,2827]
	UINT iValueTimes1[MAX_VEHICLENUM];	//过压[4250,15000]
	UINT iValueTimes2[MAX_VEHICLENUM];	//异常65534
	UINT iValueTimes3[MAX_VEHICLENUM];	//过温
	UINT iValueTimes4[MAX_VEHICLENUM];	//异常254
	UINT iValueTimes5[MAX_VEHICLENUM];	//电压警报
	UINT iValueTimes6[MAX_VEHICLENUM];	//温度警报
} STWARNINGDATA;

typedef struct WarningRankSeq
{
	UCHAR chVin[VIN_LENGTH+1];
	UINT iValueTimes[WARNING_CATEGORY_NUM];
} STWARNINGRANKSEQ;

typedef struct MsgWarningRankSeq
{
	UINT iNum;
	STWARNINGRANKSEQ stNode[MAX_VEHICLENUM];	//变长，最多MAX_VEHICLENUM
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