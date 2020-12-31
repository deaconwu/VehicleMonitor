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

int write_data(void* buffer, int size, int nmemb, void* userp) {
	// 	std::string* str = dynamic_cast<std::string*>((std::string*)userp);
	// 	str->append((char*)buffer, size * nmemb);

	FILE *fpWrite = fopen("resp.log", "wb");
	fwrite(buffer, size, nmemb, fpWrite);
	fclose(fpWrite);

	return nmemb;
}

bool OnPost(const char *szPath, char chVin[], STFILENAMEMODEL arrFile[], UINT iFileNum)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	struct curl_formsSelf forms[100] = {};

	//Éú³Éjson´®
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

	curlCode = curl_easy_setopt(curl, CURLOPT_URL, "http://10.130.161.40:8096/analysis/file");

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

	return true;
}