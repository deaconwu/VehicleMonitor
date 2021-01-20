#pragma once

#define BUFFER_SIZE 65535
#define VIN_LENGTH 17
#define MAX_FILENUM 10

#define JSON_HEAD "{\"analyze_time\":\"%u-%02u-%02u %02u:%02u:%02u\",\"data_e_time\":\"%u-%02u-%02u %02u:%02u:%02u\",\"data_s_time\":\"%u-%02u-%02u %02u:%02u:%02u\",\"items\":{"

#define JSON_ITEM "\"%s\":{\"model_code\":\"%s\",\"data_s_time\":\"%u-%02u-%02u %02u:%02u:%02u\",\"data_e_time\":\"%u-%02u-%02u %02u:%02u:%02u\"}"

#define JSON_VIN "},\"vin\":\"%s\"}"

typedef struct FileNameModel
{
	char szName[260];
	char szModel[10];
} STFILENAMEMODEL;

bool OnPost(const char *szPath, const char *szUrl);

bool OnPost(const char *szPath, char chVin[], STFILENAMEMODEL arrFile[], unsigned int iFileNum, const char *szUrl);