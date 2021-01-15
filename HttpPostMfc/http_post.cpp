#include "pch.h"
#include "http_post.h"
#include <WinSock.h>
#include <wininet.h>
#include <curl/curl.h>
#include <iostream>
#include <string.h>
#include <windows.h>

#pragma comment(lib,"ws2_32.lib") 
#pragma comment(lib, "Wininet.lib")

using namespace std;

struct curl_formsSelf {
	CURLformoption option;
	char     *value;
};

//�����ļ���չ��
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

	pTmp += VIN_LENGTH + 1;	//ƫ��vin�볤�Ⱥ͵�һ��'_'

	UCHAR iModLen = 0;
	char* pM = pTmp;

	while (*pTmp != '_')
	{
		iModLen += 1;
		pTmp += 1;
	}

	memcpy(pModel, pM, iModLen);
}

int write_data(void* buffer, int size, int nmemb, void* userp) {
	// 	std::string* str = dynamic_cast<std::string*>((std::string*)userp);
	// 	str->append((char*)buffer, size * nmemb);

	FILE *fpWrite = fopen("resp.log", "wb");
	fwrite(buffer, size, nmemb, fpWrite);
	fclose(fpWrite);

	return nmemb;
}

bool OnPost(const char *szPath, const char *szUrl)
{
	HANDLE hFile;

	WIN32_FIND_DATAA wfd = {};
	char szPathFile[MAX_PATH] = {};
	strcat(szPathFile, szPath);
	strcat(szPathFile, "\\*.txt");

	hFile = FindFirstFile(szPathFile, &wfd);
	if (hFile == INVALID_HANDLE_VALUE)	//��ǰĿ¼δƥ�䵽���������ļ�
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

			if (!memcmp(chVin, chVinEx, VIN_LENGTH))	//ͬһvin���йص��ļ���һ���ϴ�
			{
				memcpy(arrFile[iFileNum].szName, wfd.cFileName, strlen(wfd.cFileName));
				memcpy(arrFile[iFileNum].szModel, chModelEx, sizeof(chModelEx));

				iFileNum += 1;
			}
			else
			{
				printf("post vin %s\n", chVin);
				//���vin������ļ��ѱ����꣬��ʼ�ϴ�������������Ϣ
				ret = OnPost(szPath, chVin, arrFile, iFileNum, szUrl);

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
			ret = OnPost(szPath, chVin, arrFile, iFileNum, szUrl);

			break;
		}
	} while (1);

	return ret;
}

bool OnPost(const char *szPath, char chVin[], STFILENAMEMODEL arrFile[], UINT iFileNum, const char *szUrl)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	struct curl_formsSelf forms[100] = {};

	//����json��
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

	return curlCode == CURLE_OK;
}