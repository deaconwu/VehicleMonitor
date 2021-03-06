#pragma once

#define UM_LOGININ WM_USER + 1
#define UM_STEP1 WM_USER + 2
#define UM_STEP2 WM_USER + 3
#define UM_STEP3 WM_USER + 4
#define UM_STEP4 WM_USER + 5

#define UM_LOADCONFIG WM_USER + 6

#define UM_LOADSTEP1END WM_USER + 7
#define UM_LOADSTEP2END WM_USER + 8
#define UM_LOADSTEP3END WM_USER + 9

#define UM_HTTPPOSTEND WM_USER + 10	//第四步上传完成，然后根据配置的周期，依次触发数据入库、常态化点名、数据上传

#define TIMER_ID_STEP1_START 1
#define TIMER_ID_STEP2_START 2
#define TIMER_ID_STEP2_ENDCHECK   3
#define TIMER_ID_STEP3_START 4
#define TIMER_ID_STEP3_ENDCHECK   5
#define TIMER_ID_STEP4_START 6