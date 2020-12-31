#pragma once

typedef struct stStep1Config
{
	char ch_0[256];	//原始数据路径
	char ch_1[17];	//接收端IP
	char ch_2[12];	//接收端Port
	char ch_3[17];	//发送端IP
	char ch_4[12];	//发送端Port
	char ch_5[22];	//接收数据时间
	char ch_6[22];	//发送数据时间
} STSTEP1CONFIG;

typedef struct stStep2Config
{
	char ch_0[22];	//入库数据日期
	char ch_1[256];	//原始数据文件路径
	char ch_2[256];	//FK文件存放路径
	char ch_3[256];	//国标数据存放路径
	char ch_4[256];	//RVT数据存放路径
	char ch_5[256];	//DtVFb数据存放路径
	char ch_6[256];	//TzTFb数据存放路
	char ch_7[22];	//入库执行时间
} STSTEP2CONFIG;

typedef struct stStep3Config
{
	char ch_0[22];	//点名数据日期
	char ch_1[256];	//RVT路径
	char ch_2[256];	//DtVTb路径
	char ch_3[256];	//TzTFb路径
	char ch_4[256];	//dVdQ路径
	char ch_5[256];	//结果保存路径
	char ch_6[256];	//GB数据路径
	char ch_7[22];	//点名执行时间
	char ch_8;		//点名周期
	unsigned int ch_9;	//点名类型标志位
} STSTEP3CONFIG;

typedef struct stStep4Config
{
	char ch_0[256];	//对端URL地址
	char ch_1[256];	//上传数据路径
	char ch_2[22];	//上传执行时间
	char ch_3;		//上传周期
} STSTEP4CONFIG;

typedef struct stIpAddr
{
	UCHAR ip1;
	UCHAR ip2;
	UCHAR ip3;
	UCHAR ip4;
} STIPADDR;

void FilterConfig(char* pDst, const char* pSrc);

void Str2Time(SYSTEMTIME& stTime, const char* pTimeStr, bool bDateOnly=false);

DWORD GetProcessidFromName(LPCTSTR name);