#include "pch.h"
#include "AlertCalc.h"

bool CheckAlertFlag(UINT iData, int iType)
{
	UCHAR iFlag = iData;

	switch (iType)
	{
	case 0:
	{
		//温度差异报警
		iFlag = iData % 2;
		break;
	}
	case 1:
	{
		//电池高温报警
		iFlag = (iData / 2) % 2;
		break;
	}
	case 2:
	{
		iFlag = (iData / 4) % 2;
		break;
	}
	case 3:
	{
		iFlag = (iData / 8) % 2;
		break;
	}
	case 4:
	{
		iFlag = (iData / 16) % 2;
		break;
	}
	case 5:
	{
		iFlag = (iData / 32) % 2;
		break;
	}
	case 6:
	{
		iFlag = (iData / 64) % 2;
		break;
	}
	case 7:
	{
		iFlag = (iData / 128) % 2;
		break;
	}
	case 8:
	{
		iFlag = (iData / 256) % 2;
		break;
	}
	case 9:
	{
		iFlag = (iData / 512) % 2;
		break;
	}
	case 10:
	{
		iFlag = (iData / 1024) % 2;
		break;
	}
	case 11:
	{
		iFlag = (iData / 2048) % 2;
		break;
	}
	case 12:
	{
		iFlag = (iData / 4096) % 2;
		break;
	}
	case 13:
	{
		iFlag = (iData / 8192) % 2;
		break;
	}
	case 14:
	{
		iFlag = (iData / 16384) % 2;
		break;
	}
	case 15:
	{
		iFlag = (iData / 32768) % 2;
		break;
	}
	case 16:
	{
		iFlag = (iData / 65536) % 2;
		break;
	}
	case 17:
	{
		iFlag = (iData / 131072) % 2;
		break;
	}
	case 18:
	{
		iFlag = (iData / 262144) % 2;
		break;
	}
	default:
		break;
	}

	return (iFlag > 0);
}