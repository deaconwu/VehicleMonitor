#pragma once

typedef struct stSqliteLog
{
public:
	stSqliteLog()
	{
		ZeroMemory(dbName, sizeof(dbName));
		ZeroMemory(sqlCmd, sizeof(sqlCmd));
		ZeroMemory(errMsg, sizeof(errMsg));
		data = NULL;
		strcpy(dbName, "Vehicle.db");
	}

	char dbName[128];	//���ݿ���
	char sqlCmd[512];	//ִ�е�sql���
	char errMsg[128];	//���صĴ�����Ϣ
	int(*call_back)(void *data, int argc, char **argv, char **azColName);
	void *data;
}STSQLITELOG;

enum eStepState
{
	eStepState_Excute = 1,	//��ʼִ��
	eStepState_Quit = 2,	//ִ����ɺ��˳�
	eStepState_Wait = 3,	//�ȴ�ִ�����
};

typedef struct stStepStatus
{
	eStepState eStep2;
	eStepState eStep3;
} STSTEPSTATUS;

int sqlite3_exec_cmd(STSQLITELOG *obj);