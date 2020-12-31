#pragma once

typedef struct stStep1Config
{
	char ch_0[256];	//ԭʼ����·��
	char ch_1[17];	//���ն�IP
	char ch_2[12];	//���ն�Port
	char ch_3[17];	//���Ͷ�IP
	char ch_4[12];	//���Ͷ�Port
	char ch_5[22];	//��������ʱ��
	char ch_6[22];	//��������ʱ��
} STSTEP1CONFIG;

typedef struct stStep2Config
{
	char ch_0[22];	//�����������
	char ch_1[256];	//ԭʼ�����ļ�·��
	char ch_2[256];	//FK�ļ����·��
	char ch_3[256];	//�������ݴ��·��
	char ch_4[256];	//RVT���ݴ��·��
	char ch_5[256];	//DtVFb���ݴ��·��
	char ch_6[256];	//TzTFb���ݴ��·
	char ch_7[22];	//���ִ��ʱ��
} STSTEP2CONFIG;

typedef struct stStep3Config
{
	char ch_0[22];	//������������
	char ch_1[256];	//RVT·��
	char ch_2[256];	//DtVTb·��
	char ch_3[256];	//TzTFb·��
	char ch_4[256];	//dVdQ·��
	char ch_5[256];	//�������·��
	char ch_6[256];	//GB����·��
	char ch_7[22];	//����ִ��ʱ��
	char ch_8;		//��������
	unsigned int ch_9;	//�������ͱ�־λ
} STSTEP3CONFIG;

typedef struct stStep4Config
{
	char ch_0[256];	//�Զ�URL��ַ
	char ch_1[256];	//�ϴ�����·��
	char ch_2[22];	//�ϴ�ִ��ʱ��
	char ch_3;		//�ϴ�����
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