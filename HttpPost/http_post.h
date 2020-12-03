#ifndef __HTTP_POST__
#define __HTTP_POST__
			
#define JSON_HEAD "Json:{\"analyze_time\":\"%s\",\"data_e_time\":\"%s\",\"data_s_time\":\"%s\",\"items\":{"

#define JSON_ITEM "\"%s\":{\"model_code\":\"%s\",\"data_s_time\":\"%s\",\"data_e_time\":\"%s\"}"

#define JSON_VIN "},\"vin\":\"%s\"}"

#define JSON_STRING "{\"analyze_time\":\"2020-11-11 00:00:00\",\"data_e_time\":\"2020-11-11 00:00:00\",\"data_s_time\":\"2020-11-11 00:00:00\",\"items\":{\"LJNDGV1T0JN600342_DtDy_2020_11_10.txt\":{\"data_s_time\":\"2020-11-10 00:00:00\",\"model_code\":\"DtDy\",\"data_e_time\":\"2020-11-10 00:00:00\"},\"LJNDGV1T0JN600342_DtQx_2020_11_10.txt\":{\"data_s_time\":\"2020-11-10 00:00:00\",\"model_code\":\"DtQx\",\"data_e_time\":\"2020-11-10 00:00:00\"}},\"vin\":\"LJNDGV1T0JN600342\"}"

 
#endif