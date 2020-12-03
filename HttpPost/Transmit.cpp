#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <WinSock.h>
#include <wininet.h>
#include <tchar.h>
#include <curl/curl.h>
#include "http_post.h"
#include <iostream>
#include <string.h>

#pragma comment(lib,"ws2_32.lib") 
#pragma comment(lib, "Wininet.lib")

using namespace std;

#define BUFFER_SIZE 65535

int write_data(void* buffer, int size, int nmemb, void* userp) {
// 	std::string* str = dynamic_cast<std::string*>((std::string*)userp);
// 	str->append((char*)buffer, size * nmemb);

	FILE *fpWrite = fopen("resp.txt", "wb+");
	fwrite(buffer, size, nmemb, fpWrite);
	fclose(fpWrite);

	return nmemb;
}

int main()
{
	char strPostData[BUFFER_SIZE] = {};

	char strTokenData[BUFFER_SIZE] = {"ANALYSIS-CALL-TOKEN\njfu349fh9w8fJDWEEjcmwiefm38fjmcdiowe\r\n"};

	char strJsonData[BUFFER_SIZE] = {};
	int headLen = snprintf(strJsonData, BUFFER_SIZE, JSON_HEAD, "2020-09-01 00:00:00", "2020-09-01 00:00:00", "2020-09-01 15:37:30");
	int itemLen = snprintf(&strJsonData[headLen], BUFFER_SIZE, JSON_ITEM, "LJNDGE2TXJN120419_DtQx_2020_08_31.txt", "DtQx", "2020-09-01 00:00:00", "2020-09-01 15:37:30");
	int vinLen = snprintf(&strJsonData[headLen + itemLen], BUFFER_SIZE - headLen - itemLen, JSON_VIN, "LJNDGE2TXJN120419");
	strcat(strJsonData, "\r\n");

	strcat(strPostData, strJsonData);

	std::string body;
	std::string response;
	CURLcode curlCode = CURLE_OK;
	struct curl_httppost* post = NULL;
	struct curl_httppost* last = NULL;

	curl_version();

	curl_global_init(CURL_GLOBAL_ALL);

	CURL *curl = curl_easy_init();
	int my_param = 1;

	char szField[512] = {};

	curlCode = curl_easy_setopt(curl, CURLOPT_URL, "http://10.130.161.40:8096/analysis/file");
	
	struct curl_slist *list = NULL;

	//list = curl_slist_append(list, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
	list = curl_slist_append(list, "ANALYSIS-CALL-TOKEN:jfu349fh9w8fJDWEEjcmwiefm38fjmcdiowe");
	curlCode = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
	curlCode = curl_easy_setopt(curl, CURLOPT_POST, 1L);

	CURLFORMcode curlFormCode;

	char strJson[BUFFER_SIZE] = {};
	sprintf(strJson, "%s", JSON_STRING);

	curlFormCode = curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "describe",
		CURLFORM_COPYCONTENTS, strJson,
		CURLFORM_END);

	struct curl_forms forms[3] = {};
	forms[0].option = CURLFORM_FILE;
	forms[0].value = "C:/Users/Dell/source/repos/Transmit/LJNDGV1T0JN600342_DtDy_2020_11_10.txt";

	forms[1].option = CURLFORM_FILE;
	forms[1].value = "C:/Users/Dell/source/repos/Transmit/LJNDGV1T0JN600342_DtQx_2020_11_10.txt";

	forms[2].option = CURLFORM_END;

	curlFormCode = curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "files",
		CURLFORM_ARRAY, forms,
		CURLFORM_END);

	curlCode = curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
	curlCode = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, write_data);
	//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	//curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&response);
	curlCode = curl_easy_perform(curl);

	curl_slist_free_all(list);
	curl_formfree(post);
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	system("pause");

	return 0;
}
