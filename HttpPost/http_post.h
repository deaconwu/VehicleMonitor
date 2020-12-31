#ifndef __HTTP_POST__
#define __HTTP_POST__

#define JSON_HEAD "{\"analyze_time\":\"%u-%02u-%02u %02u:%02u:%02u\",\"data_e_time\":\"%u-%02u-%02u %02u:%02u:%02u\",\"data_s_time\":\"%u-%02u-%02u %02u:%02u:%02u\",\"items\":{"

#define JSON_ITEM "\"%s\":{\"model_code\":\"%s\",\"data_s_time\":\"%u-%02u-%02u %02u:%02u:%02u\",\"data_e_time\":\"%u-%02u-%02u %02u:%02u:%02u\"}"

#define JSON_VIN "},\"vin\":\"%s\"}"

#define JSON_STRING "{\"analyze_time\":\"2020-11-11 00:00:00\",\"data_e_time\":\"2020-11-11 00:00:00\",\"data_s_time\":\"2020-11-11 00:00:00\",\"items\":{\"LJNDGV1T0JN600342_DtDy_2020_11_10.txt\":{\"data_s_time\":\"2020-11-10 00:00:00\",\"model_code\":\"DtDy\",\"data_e_time\":\"2020-11-10 00:00:00\"},\"LJNDGV1T0JN600342_DtQx_2020_11_10.txt\":{\"data_s_time\":\"2020-11-10 00:00:00\",\"model_code\":\"DtQx\",\"data_e_time\":\"2020-11-10 00:00:00\"}},\"vin\":\"LJNDGV1T0JN600342\"}"

#define BUFFER_SIZE 65535
#define VIN_LENGTH 17
#define MAX_FILENUM 10

typedef struct FileNameModel
{
	char szName[260];
	char szModel[10];
} STFILENAMEMODEL;

bool OnPost(const char *szPath, char chVin[], STFILENAMEMODEL arrFile[], unsigned int iFileNum);

#endif