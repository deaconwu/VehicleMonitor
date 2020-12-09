#include "pch.h"
#include "TimeStamp.h"

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

ULONG ConvertSecondsTime(USHORT iYear, UCHAR iMon, UCHAR iDay, UCHAR iHour, UCHAR iMin, UCHAR iSec)
{
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

void ConvertDateTime(ULONG lSeconds, SYSTEMTIME& stTime)
{
	USHORT iYear = 1970;
	UCHAR iMonth = 0;
	USHORT iDay = 0;
	ULONG lDays = lSeconds / SECONDS_OF_DAY;	//总天数
	ULONG lSecs = lSeconds % SECONDS_OF_DAY;	//除去前面天数，最后一天的秒数

	while (lDays > 365)
	{
		if (((iYear % 4 == 0) && (iYear % 100 != 0)) || (iYear % 400 == 0))
			lDays -= 366;
		else
			lDays -= 365;
		iYear++;
	}

	if ((lDays == 365) && !(((iYear % 4 == 0) && (iYear % 100 != 0)) || (iYear % 400 == 0)))
	{
		lDays -= 365;
		iYear++;
	}

	stTime.wYear = iYear;

	for (iMonth = 1; iMonth <= 12; iMonth++)
	{
		if ((iMonth == 2) && (((iYear % 4 == 0) && (iYear % 100 != 0)) || (iYear % 400 == 0)))
			iDay = 29;
		else
			iDay = DayOfMon[iMonth - 1];

		if (lDays >= iDay)
			lDays -= iDay;
		else
			break;
	}

	stTime.wMonth = iMonth;
	stTime.wDay = (USHORT)(lDays + 1);
	stTime.wHour = (lSecs / SECONDS_OF_HOUR) % HOURS_OF_DAY;
	stTime.wMinute = (lSecs % SECONDS_OF_HOUR) / SECONDS_OF_MIN;
	stTime.wSecond = (lSecs % SECONDS_OF_HOUR) % SECONDS_OF_MIN;
}