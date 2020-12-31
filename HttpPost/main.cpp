#include <stdio.h>
#include <stdlib.h>
#include "http_post.h"
#include <windows.h>


//分离文件扩展名
void str_extention(char* str, char szExtent[])
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

void str_vin_model(char* pStr, char* pVin, char* pModel)
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

int main()
{
	//遍历当前目录文件
	char szPath[MAX_PATH] = {};
	GetCurrentDirectory(MAX_PATH, szPath);

	HANDLE hFile;

	WIN32_FIND_DATAA wfd = {};
	char szPathFile[MAX_PATH] = {};
	strcat(szPathFile, szPath);
	strcat(szPathFile, "\\*.txt");

	hFile = FindFirstFile(szPathFile, &wfd);
	if (hFile == INVALID_HANDLE_VALUE)	//当前目录未匹配到这种类型文件
	{
		printf("no file .txt\n");
		system("pause");
		return -1;
	}

	char szFileName[MAX_PATH] = {};
	char szExtent[MAX_PATH] = {};
	strcpy(szFileName, wfd.cFileName);
	str_extention(szFileName, szExtent);

	printf("szFileName: %s\n", szFileName);

	char chVin[VIN_LENGTH + 1] = {};
	char chModel[10] = {};
	str_vin_model(szFileName, chVin, chModel);

	STFILENAMEMODEL arrFile[MAX_FILENUM] = {};
	memcpy(arrFile[0].szName, wfd.cFileName, strlen(wfd.cFileName));
	memcpy(arrFile[0].szModel, chModel, sizeof(chModel));
	UINT iFileNum = 1;

	//	struct curl_forms forms[100] = {};

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
				printf("szFileName: %s\n", szFileName);
				memcpy(arrFile[iFileNum].szName, wfd.cFileName, strlen(wfd.cFileName));
				memcpy(arrFile[iFileNum].szModel, chModelEx, sizeof(chModelEx));

				iFileNum += 1;
			}
			else
			{
				printf("post vin %s\n", chVin);
				//这个vin码相关文件已遍历完，开始上传这辆车所有信息
				OnPost(szPath, chVin, arrFile, iFileNum);

				printf("\n");

				printf("szFileName: %s\n", szFileName);
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
			OnPost(szPath, chVin, arrFile, iFileNum);

			printf("\n");

			break;
		}
	} while (1);

	system("pause");

	return 0;
}
