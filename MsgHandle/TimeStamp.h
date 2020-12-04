#pragma once

#define SECONDS_OF_MIN	60
#define MINUTES_OF_HOUR	60
#define SECONDS_OF_HOUR	MINUTES_OF_HOUR * SECONDS_OF_MIN
#define HOURS_OF_DAY	24
#define SECONDS_OF_DAY	HOURS_OF_DAY * SECONDS_OF_HOUR

static UCHAR DayOfMon[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

ULONG ConvertSecondsTime(SYSTEMTIME& stTime)
{
	USHORT iYear = stTime.wYear;
	USHORT iMon = stTime.wMonth;
	USHORT iDay = stTime.wDay;
	USHORT iHour = stTime.wHour;
	USHORT iMin = stTime.wMinute;
	USHORT iSec = stTime.wSecond;

	USHORT iLeapY = 0;
	ULONG iDays = 0;
	ULONG iSeconds = 0;

	for (USHORT i = 1970; i < iYear; i++)
	{
		if (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))
			iLeapY++;
	}

	iDays = iLeapY * 366 + (iYear - 1970 - iLeapY) * 365;

	for (USHORT i = 1; i < iMon; i++)
	{
		if ((i == 2) && (((iYear % 4 == 0) && (iYear % 100 != 0)) || (iYear % 400 == 0)))
			iDays += 29;
		else
			iDays += DayOfMon[i - 1];
	}

	iDays += iDay - 1;	//当天未满24小时，需减1

	iSeconds = iDays * SECONDS_OF_DAY + iHour * SECONDS_OF_HOUR + iMin * SECONDS_OF_MIN + iSec;

	return iSeconds;
}