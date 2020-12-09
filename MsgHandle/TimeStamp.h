#pragma once

#define SECONDS_OF_MIN	60
#define MINUTES_OF_HOUR	60
#define SECONDS_OF_HOUR	(MINUTES_OF_HOUR * SECONDS_OF_MIN)
#define HOURS_OF_DAY	24
#define SECONDS_OF_DAY	(HOURS_OF_DAY * SECONDS_OF_HOUR)

ULONG ConvertSecondsTime(SYSTEMTIME& stTime);

ULONG ConvertSecondsTime(USHORT iYear, UCHAR iMon, UCHAR iDay, UCHAR iHour, UCHAR iMin, UCHAR iSec);

void ConvertDateTime(ULONG iSeconds, SYSTEMTIME& stTime);
