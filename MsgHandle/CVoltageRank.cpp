#include "pch.h"
#include "CVoltageRank.h"
#include "CInfoRecord.h"
#include "UserMessage.h"

static bool g_bStopRank = false;
static int g_iType = 1;
static int g_iTypePre = 1;

CVoltageRank* CVoltageRank::m_pInstance = NULL;

char* g_pRankNodeMem = (char*)malloc(sizeof(STVOLTAGERANKLINK)*MAX_VEHICLENUM);

DWORD WINAPI OnRankThread(LPVOID lparam);

static UINT GetValueTimes(UINT iPos, STVOLTAGEDATA& stData)
{
	UINT iValue = 0;
	switch (g_iType)
	{
	case 0:
	{
		iValue = stData.iValueTimes0[iPos];
		break;
	}
	case 1:
	{
		iValue = stData.iValueTimes1[iPos];
		break;
	}
	case 2:
	{
		iValue = stData.iValueTimes2[iPos];
		break;
	}
	case 3:
	{
		iValue = stData.iValueTimes3[iPos];
		break;
	}
	case 4:
	{
		iValue = stData.iValueTimes4[iPos];
		break;
	}
	case 5:
	{
		iValue = stData.iValueTimes5[iPos];
		break;
	}
	case 6:
	{
		iValue = stData.iValueTimes6[iPos];
		break;
	}
	default:
		break;
	}

	return iValue;
}

static void RankSort(STVOLTAGEDATA& stData, STMSGVOLTAGERANKSEQ& msgSeq, int iType=-1)
{
	if (iType < 0)
	{
		iType = g_iType;
	}

	memset(g_pRankNodeMem, 0, sizeof(STVOLTAGERANKLINK)*MAX_VEHICLENUM);

	UCHAR chVin[MAX_VEHICLENUM][VIN_LENGTH] = {};
	msgSeq.iNum = CInfoRecord::GetInstance()->GetVins(chVin);

	UINT iValueTimes = 0;
	UINT iRankNum = 0;
	PSTVOLTAGERANKLINK pNode = NULL;
	PSTVOLTAGERANKLINK pLast = NULL;

	for (UINT i = 0; i < msgSeq.iNum; i++)
	{
		if (g_iType != g_iTypePre)
		{
			return;
		}

		iValueTimes = GetValueTimes(i, stData);
		if (pNode == NULL)
		{
			//链表为空
			pNode = (PSTVOLTAGERANKLINK)(g_pRankNodeMem + iRankNum * sizeof(STVOLTAGERANKLINK));
			if (NULL == pNode)
				return;

			pNode->pPre = NULL;
			pNode->pNext = NULL;
			pLast = pNode;

			pLast->iValueTimes[0] = stData.iValueTimes0[i];
			pLast->iValueTimes[1] = stData.iValueTimes1[i];
			pLast->iValueTimes[2] = stData.iValueTimes2[i];
			pLast->iValueTimes[3] = stData.iValueTimes3[i];
			pLast->iValueTimes[4] = stData.iValueTimes4[i];
			pLast->iValueTimes[5] = stData.iValueTimes5[i];
			pLast->iValueTimes[6] = stData.iValueTimes6[i];

			memcpy(pLast->chVin, chVin[i], (VIN_LENGTH));
			iRankNum += 1;
		}
		else
		{
			if (iValueTimes > 0)	//与链表每个节点比较
			{
				PSTVOLTAGERANKLINK pPrev = pLast;
				while (pPrev != NULL)
				{
					if (iValueTimes <= pPrev->iValueTimes[iType])
					{
						PSTVOLTAGERANKLINK pNew = (PSTVOLTAGERANKLINK)(g_pRankNodeMem + iRankNum * sizeof(STVOLTAGERANKLINK));
						pNew->iValueTimes[0] = stData.iValueTimes0[i];
						pNew->iValueTimes[1] = stData.iValueTimes1[i];
						pNew->iValueTimes[2] = stData.iValueTimes2[i];
						pNew->iValueTimes[3] = stData.iValueTimes3[i];
						pNew->iValueTimes[4] = stData.iValueTimes4[i];
						pNew->iValueTimes[5] = stData.iValueTimes5[i];
						pNew->iValueTimes[6] = stData.iValueTimes6[i];

						memcpy(pNew->chVin, chVin[i], (VIN_LENGTH));
						pNew->pNext = NULL;

						if (pPrev->pNext != NULL)
						{
							pPrev->pNext->pPre = pNew;
							pNew->pNext = pPrev->pNext;
						}
						else
						{
							//新结点插入链尾
							pLast = pNew;
						}

						pNew->pPre = pPrev;
						pPrev->pNext = pNew;

						iRankNum += 1;
						break;
					}

					pPrev = pPrev->pPre;
				}

				if (pPrev == NULL)
				{
					//排名最大
					PSTVOLTAGERANKLINK pNew = (PSTVOLTAGERANKLINK)(g_pRankNodeMem + iRankNum * sizeof(STVOLTAGERANKLINK));
					pNew->iValueTimes[0] = stData.iValueTimes0[i];
					pNew->iValueTimes[1] = stData.iValueTimes1[i];
					pNew->iValueTimes[2] = stData.iValueTimes2[i];
					pNew->iValueTimes[3] = stData.iValueTimes3[i];
					pNew->iValueTimes[4] = stData.iValueTimes4[i];
					pNew->iValueTimes[5] = stData.iValueTimes5[i];
					pNew->iValueTimes[6] = stData.iValueTimes6[i];

					memcpy(pNew->chVin, chVin[i], (VIN_LENGTH));
					pNew->pPre = NULL;
					pNew->pNext = pNode;
					pNode->pPre = pNew;
					pNode = pNew;
					iRankNum += 1;
				}
			}
			else
			{
				//插入链尾
				if (pNode == NULL)
				{
					PSTVOLTAGERANKLINK pNew = (PSTVOLTAGERANKLINK)(g_pRankNodeMem + iRankNum * sizeof(STVOLTAGERANKLINK));
				}

				PSTVOLTAGERANKLINK pPrev = pLast;
				PSTVOLTAGERANKLINK pNew = (PSTVOLTAGERANKLINK)(g_pRankNodeMem + iRankNum * sizeof(STVOLTAGERANKLINK));
				pNew->iValueTimes[0] = stData.iValueTimes0[i];
				pNew->iValueTimes[1] = stData.iValueTimes1[i];
				pNew->iValueTimes[2] = stData.iValueTimes2[i];
				pNew->iValueTimes[3] = stData.iValueTimes3[i];
				pNew->iValueTimes[4] = stData.iValueTimes4[i];
				pNew->iValueTimes[5] = stData.iValueTimes5[i];
				pNew->iValueTimes[6] = stData.iValueTimes6[i];
				memcpy(pNew->chVin, chVin[i], (VIN_LENGTH));
				pNew->pNext = NULL;
				pLast = pNew;
				pNew->pPre = pPrev;
				pPrev->pNext = pNew;

				iRankNum += 1;
			}
		}
	}

	UINT iInertNum = 0;
	while (pNode != NULL)
	{
		if (iType != g_iTypePre)
		{
			return;
		}

		memcpy(msgSeq.stNode[iInertNum].chVin, pNode->chVin, VIN_LENGTH);
		memcpy(msgSeq.stNode[iInertNum].iValueTimes, pNode->iValueTimes, VOLTAGE_CATEGORY_NUM*sizeof(UINT));
		pNode = pNode->pNext;

		iInertNum++;
	}
}

void CVoltageRank::OnStopRank()
{
	g_bStopRank = true;
}

void CVoltageRank::NotifyType(int iType)
{
	g_iType = iType;
}

void CVoltageRank::OnSaveList(WORD wYear, WORD wMonth, WORD wDay)
{
	char strTitle[] = "Vin码,0,1-2650,2651-4200,4201-15000,15001-65533,65534,65535";
	char strFileName[100] = {};
	sprintf(strFileName, "Voltage_%d-%d-%d.csv", wYear, wMonth, wDay);

	FILE *fpWrite = fopen(strFileName, "wb+");
	if (NULL == fpWrite)
	{
		return;
	}

	fprintf(fpWrite, "%s\n", strTitle);
	
	STVOLTAGEDATA stData;
	memset(&stData, 0, sizeof(stData));

	CInfoRecord::GetInstance()->OnVoltage(stData);

	STMSGVOLTAGERANKSEQ msgSeq;
	memset(&msgSeq, 0, sizeof(msgSeq));

	RankSort(stData, msgSeq);

	for (UINT i=0; i< msgSeq.iNum; i++)
	{
		fprintf(fpWrite, "%s,%u,%u,%u,%u,%u,%u,%u",
			msgSeq.stNode[i].chVin,
			msgSeq.stNode[i].iValueTimes[0],
			msgSeq.stNode[i].iValueTimes[1],
			msgSeq.stNode[i].iValueTimes[2],
			msgSeq.stNode[i].iValueTimes[3],
			msgSeq.stNode[i].iValueTimes[4],
			msgSeq.stNode[i].iValueTimes[5],
			msgSeq.stNode[i].iValueTimes[6]);

		if (i < msgSeq.iNum -1)
		{
			fprintf(fpWrite, "\n");
		}
	}

	fclose(fpWrite);
}

void CVoltageRank::OnLaunchRank(HWND hWnd, int iType)
{
	memset(g_pRankNodeMem, 0, sizeof(STVOLTAGERANKLINK)*MAX_VEHICLENUM);
	g_bStopRank = false;
	//g_iType = iType;
	//g_iTypePre = iType;

	DWORD exitCode;
	BOOL ret = ::GetExitCodeThread(m_hThread, &exitCode);
	if (exitCode != STILL_ACTIVE)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;

		DWORD dwThreadId;
		m_hThread = CreateThread(NULL, NULL, OnRankThread, hWnd, 0, &dwThreadId);
	}
}

DWORD WINAPI OnRankThread(LPVOID lparam)
{
	HWND hWnd = (HWND)lparam;

	while (1)
	{
		if (g_bStopRank)
		{
			break;
		}

		STVOLTAGEDATA stData;
		memset(&stData, 0, sizeof(stData));

		CInfoRecord::GetInstance()->OnVoltage(stData);

		STMSGVOLTAGERANKSEQ msgSeq;
		memset(&msgSeq, 0, sizeof(msgSeq));

		RankSort(stData, msgSeq);

		if (g_iType == g_iTypePre)
			::SendMessage(hWnd, UM_VOLTAGERANK, (WPARAM)&msgSeq, 0);

		g_iTypePre = g_iType;

		Sleep(1000);
	}

	return 0;
}