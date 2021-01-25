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

	char dbName[128];	//数据库名
	char sqlCmd[512];	//执行的sql语句
	char errMsg[128];	//返回的错误信息
	int(*call_back)(void *data, int argc, char **argv, char **azColName);
	void *data;
}STSQLITELOG;

enum eStepState
{
	eStepState_Excute = 1,	//开始执行
	eStepState_Quit = 2,	//执行完成后退出
	eStepState_Wait = 3,	//等待执行完成
};

typedef struct stStepStatus
{
	eStepState eStep2;
	eStepState eStep3;
} STSTEPSTATUS;

int sqlite3_exec_cmd(STSQLITELOG *obj);