#include "pch.h"
#include <sqlite3.h>
#include "CSqliteLog.h"

stSqliteLog g_sqliteLog;

int sqlite3_exec_cmd(STSQLITELOG *obj)
{
	sqlite3* db;
	memset(obj->errMsg, 0, sizeof(obj->errMsg));

	int ret = sqlite3_open(obj->dbName, &db);
	if (ret != SQLITE_OK)
	{
		const char* errConst = sqlite3_errmsg(db);
		strcpy(obj->errMsg, errConst);
		return ret;
	}

	char *zErrMsg = NULL;
	ret = sqlite3_exec(db, obj->sqlCmd, obj->call_back, (void*)obj->data, &zErrMsg);
	if (ret != SQLITE_OK)
	{
		strcpy(obj->errMsg, zErrMsg);
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(db);

	return ret;
}
