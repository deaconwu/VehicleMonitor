// DataCompress.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <cstring>
#include <shlobj.h>
#include <tlhelp32.h>
#include <atlimage.h>
#include <bcrypt.h>
//#include "Compress.h"

//#pragma comment(lib,"COMPRESS.lib")

int main()
{
 	//HMODULE hModule = LoadLibrary(L"ntdll.dll");
	HMODULE hModule = LoadLibraryA("Compress.dll");
	if (NULL == hModule)
	{
		printf("Load dll error!\n");
		return 1;
	}

	char szBuffer[50] = "i am a gooooooooood student";
	size_t dwBufferLength = strlen(szBuffer);
	unsigned char* pCompressData = NULL;
	unsigned long dwCompressDataLength = 0;
	unsigned char* pUncompressData = NULL;
	unsigned long dwUncompressDataLength = 0;

	bool(*pCompress)(unsigned char*, unsigned long, unsigned char**, unsigned long*) = NULL;
	pCompress = (bool(*)(unsigned char* , unsigned long , unsigned char**, unsigned long*))GetProcAddress(hModule, "DataCompress");

	bool(*pUnCompress)(unsigned char*, unsigned long, unsigned char**, unsigned long*) = NULL;
	pUnCompress = (bool(*)(unsigned char*, unsigned long, unsigned char**, unsigned long*))GetProcAddress(hModule, "UncompressData");

	if (pCompress!=NULL)
	{
		pCompress((unsigned char *)szBuffer, dwBufferLength, &pCompressData, &dwCompressDataLength);
		for (unsigned long i = 0; i < dwBufferLength; ++i)
		{
			printf("%X ", szBuffer[i]);
		}
		printf("\n");

		for (unsigned long i = 0; i < dwCompressDataLength; ++i)
		{
			printf("%X ", pCompressData[i]);
		}
		printf("\n");
		
		if (pUnCompress != NULL)
		{
			pUnCompress(pCompressData, dwCompressDataLength, &pUncompressData, &dwUncompressDataLength);
			for (unsigned long i = 0; i < dwUncompressDataLength; ++i)
			{
				printf("%X ", pUncompressData[i]);
			}
			printf("\n");

			delete[] pUncompressData;
			pUncompressData = NULL;
		}

		delete [] pCompressData;
		pCompressData = NULL;
	}

	FreeLibrary(hModule);
	system("pause");

	return 0;
}
