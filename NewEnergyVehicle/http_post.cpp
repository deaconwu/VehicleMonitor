#include "pch.h"
#include "http_post.h"
#include <WinSock.h>
#include <wininet.h>
#include <curl/curl.h>
#include <iostream>
#include <string.h>
#include <windows.h>
#include "cJSON.h"

#pragma comment(lib,"ws2_32.lib") 
#pragma comment(lib, "Wininet.lib")

using namespace std;

struct curl_formsSelf {
	CURLformoption option;
	char     *value;
};

static unsigned int CheckRespCode()
{
	FILE *fpRead = fopen("resp.log", "rb");

	cJSON *json;
	char line[1024] = { 0 };
	fgets(line, sizeof(line), fpRead);

	fclose(fpRead);

	json = cJSON_Parse(line);
	cJSON_Print(json);
	cJSON *arrayItem = cJSON_GetObjectItem(json, "code");

	return arrayItem->valueint;
}

//分离文件扩展名
static void str_extention(char* str, char szExtent[])
{
	memset(szExtent, 0, sizeof(szExtent));
	char* p = NULL;
	p = strtok(str, ".");
	while (p != NULL)
	{
		memset(szExtent, 0, sizeof(szExtent));
		strcpy(szExtent, p);
		p = strtok(NULL, ".");
	}
}

static void str_vin_model(char* pStr, char* pVin, char* pModel)
{
	char* pTmp = pStr;

	memcpy(pVin, pTmp, VIN_LENGTH);

	pTmp += VIN_LENGTH + 1;	//偏移vin码长度和第一个'_'

	UCHAR iModLen = 0;
	char* pM = pTmp;

	while (*pTmp != '_')
	{
		iModLen += 1;
		pTmp += 1;
	}

	memcpy(pModel, pM, iModLen);
}

int write_data(void* buffer, int size, int nmemb, void* resp)
{
	// 	std::string* str = dynamic_cast<std::string*>((std::string*)userp);
	// 	str->append((char*)buffer, size * nmemb);

	FILE *fpWrite = fopen("resp.log", "wb");
	fwrite(buffer, size, nmemb, fpWrite);
	fclose(fpWrite);

	return nmemb;
}

bool OnPost(const char *szPath, const char *szUrl, const char *szHis)
{
	HANDLE hFile;

	WIN32_FIND_DATAA wfd = {};
	char szPathFile[MAX_PATH] = {};
	strcat(szPathFile, szPath);
	strcat(szPathFile, "\\*.txt");

	hFile = FindFirstFile(szPathFile, &wfd);
	if (hFile == INVALID_HANDLE_VALUE)	//当前目录未匹配到这种类型文件
	{
		return false;
	}

	char szFileName[MAX_PATH] = {};
	char szExtent[MAX_PATH] = {};
	strcpy(szFileName, wfd.cFileName);
	str_extention(szFileName, szExtent);

	char chVin[VIN_LENGTH + 1] = {};
	char chModel[10] = {};
	str_vin_model(szFileName, chVin, chModel);

	STFILENAMEMODEL arrFile[MAX_FILENUM] = {};
	memcpy(arrFile[0].szName, wfd.cFileName, strlen(wfd.cFileName));
	memcpy(arrFile[0].szModel, chModel, sizeof(chModel));
	UINT iFileNum = 1;
	bool ret = false;

	do
	{
		if (FindNextFile(hFile, &wfd))
		{
			memset(szFileName, 0, sizeof(szFileName));
			strcpy(szFileName, wfd.cFileName);
			str_extention(szFileName, szExtent);

			if (memcmp(szExtent, "txt", strlen("txt")))
			{
				continue;
			}

			char chVinEx[VIN_LENGTH + 1] = {};
			char chModelEx[10] = {};
			str_vin_model(szFileName, chVinEx, chModelEx);

			if (!memcmp(chVin, chVinEx, VIN_LENGTH))	//同一vin码有关的文件放一块上传
			{
				memcpy(arrFile[iFileNum].szName, wfd.cFileName, strlen(wfd.cFileName));
				memcpy(arrFile[iFileNum].szModel, chModelEx, sizeof(chModelEx));

				iFileNum += 1;
			}
			else
			{
				printf("post vin %s\n", chVin);
				//这个vin码相关文件已遍历完，开始上传这辆车所有信息
				if (strlen(chVin) == VIN_LENGTH)
				{
					ret = OnPost(szPath, chVin, arrFile, iFileNum, szUrl, szHis);
				}

				//itemLen += snprintf(&strJsonData[itemLen], BUFFER_SIZE, JSON_VIN, chVin);

				memcpy(chVin, chVinEx, VIN_LENGTH);
				memcpy(chModel, chModelEx, sizeof(chModelEx));

				memset(&arrFile, 0, sizeof(arrFile));
				memcpy(arrFile[0].szName, wfd.cFileName, strlen(wfd.cFileName));
				memcpy(arrFile[0].szModel, chModelEx, sizeof(chModelEx));
				iFileNum = 1;
			}
		}
		else
		{
			ret = OnPost(szPath, chVin, arrFile, iFileNum, szUrl, szHis);

			break;
		}
	} while (1);

	return ret;
}

bool OnPost(const char *szPath, char chVin[], STFILENAMEMODEL arrFile[], UINT iFileNum, const char *szUrl, const char *szHis)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	struct curl_formsSelf forms[100] = {};

	//生成json串
	char strJson[BUFFER_SIZE] = {};
	int headLen = snprintf(strJson, BUFFER_SIZE, JSON_HEAD,
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,	//analyze_time
		st.wYear, st.wMonth, st.wDay, 0, 0, 0,	//data_e_time
		st.wYear, st.wMonth, st.wDay, 0, 0, 0	//data_s_time
	);

	int itemLen = headLen;

	for (UINT i = 0; i < iFileNum; i++)
	{
		itemLen += snprintf(&strJson[itemLen], BUFFER_SIZE, JSON_ITEM,
			arrFile[i].szName,
			arrFile[i].szModel,
			st.wYear, st.wMonth, st.wDay, 0, 0, 0,	//data_s_time
			st.wYear, st.wMonth, st.wDay, 0, 0, 0	//data_e_time
		);

		if (i != iFileNum - 1)
		{
			strcat(&strJson[itemLen], ",");
			itemLen += 1;
		}

		forms[i].value = (char *)malloc(MAX_PATH);

		forms[i].option = CURLFORM_FILE;
		strcpy(forms[i].value, szPath);
		strcat(forms[i].value, "\\");
		strcat(forms[i].value, arrFile[i].szName);
	}

	itemLen += snprintf(&strJson[itemLen], BUFFER_SIZE, JSON_VIN, chVin);
	strcat(strJson, "\r\n");

	forms[iFileNum].option = CURLFORM_END;


	std::string body;
	std::string response;
	CURLcode curlCode = CURLE_OK;
	struct curl_httppost* post = NULL;
	struct curl_httppost* last = NULL;

	curl_version();

	curl_global_init(CURL_GLOBAL_ALL);

	CURL *curl = curl_easy_init();

	curlCode = curl_easy_setopt(curl, CURLOPT_URL, szUrl);

	struct curl_slist *list = NULL;
	list = curl_slist_append(list, "ANALYSIS-CALL-TOKEN:jfu349fh9w8fJDWEEjcmwiefm38fjmcdiowe");
	curlCode = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
	curlCode = curl_easy_setopt(curl, CURLOPT_POST, 1L);

	CURLFORMcode curlFormCode;

	curlFormCode = curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "describe",
		CURLFORM_COPYCONTENTS, strJson,
		CURLFORM_END);

	curlFormCode = curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "files",
		CURLFORM_ARRAY, forms,
		CURLFORM_END);

	curlCode = curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
	curlCode = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
	//curl_easy_setopt(curl, CURLOPT_READFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&response);
	curlCode = curl_easy_perform(curl);

	curl_slist_free_all(list);
	curl_formfree(post);
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	for (UINT i = 0; i < iFileNum; i++)
	{
		free(forms[i].value);
		forms[i].value = NULL;
	}

	unsigned int statusCode = 0;

	if (curlCode == CURLE_OK)
	{
		//读resp.log里的状态码
		statusCode = CheckRespCode();
		if (200 == statusCode)
		{
			//上传成功的文件才移动
			OnMove(szPath, szHis, arrFile, iFileNum);
		}
	}

	for (UINT i = 0; i < iFileNum; i++)
	{
		ZeroMemory(g_sqliteLog.sqlCmd, sizeof(g_sqliteLog.sqlCmd));
		sprintf_s(g_sqliteLog.sqlCmd, "INSERT INTO post_record VALUES('%s', '%s', '%s', %u, %u, '%02u-%02u-%02u %02u:%02u:%02u');",
			chVin, arrFile[i].szModel, arrFile[i].szName, curlCode, statusCode, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		sqlite3_exec_cmd(&g_sqliteLog);
	}

	return (curlCode == CURLE_OK) && (statusCode == 200);
}

void OnMove(const char *szPath, const char *szHis, STFILENAMEMODEL arrFile[], unsigned int iFileNum)
{
	//移动文件
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION | FOF_FILESONLY;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;

	char *pPathSrc = new char[MAX_PATH*iFileNum];

	for (UINT i=0; i<iFileNum; i++)
	{
		memset(pPathSrc, 0, MAX_PATH*iFileNum);
		strcat(pPathSrc, szPath);
		strcat(pPathSrc, "\\");
		strcat(pPathSrc, arrFile[i].szName);
		strcat(pPathSrc, "\0\0");

		FileOp.pFrom = pPathSrc;
		FileOp.pTo = szHis;
		FileOp.wFunc = FO_MOVE;
		int ret = SHFileOperation(&FileOp);
		if (ret)
		{
			printf("move error");
		}
	}

	free(pPathSrc);
	pPathSrc = NULL;
}