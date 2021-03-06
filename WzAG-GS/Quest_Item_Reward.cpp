#include "StdAfx.h"
#include "Quest_Item_Reward.h" 

Quest_Item_Reward	Quest_Item_RewardInfo[255];
int Quest_Item_RewardCount;
void ReadQuest_Item_Reward()
{
	FILE *fp;
	BOOL bRead = FALSE;
	DWORD dwArgv = 0;
	char sLineTxt[255] = { 0 };
	Quest_Item_RewardCount = 1;

	fp = fopen(CFG_Quest_Item_Reward, "r");

	if (!fp)
	{
		MessageBoxA(NULL, CFG_Quest_Item_Reward, "Error!", MB_OK);
		::ExitProcess(0);
	}

	rewind(fp);

	while (fgets(sLineTxt, 255, fp) != NULL)
	{
		if (sLineTxt[0] == '/')continue;
		if (sLineTxt[0] == ';')continue;

		int n[20];
		char GetItemName[20];
		char GetExibName[20];

		sscanf(sLineTxt, "%s %d %d %d %d %d %d %d %d %d %s", &GetItemName, &n[1], &n[2], &n[3], &n[4], &n[5], &n[6], &n[7], &n[8], &n[9], &GetExibName);
		sprintf(Quest_Item_RewardInfo[Quest_Item_RewardCount].itemnome, "%s", GetItemName);
		sprintf(Quest_Item_RewardInfo[Quest_Item_RewardCount].itemexib, "%s", GetExibName);
		Quest_Item_RewardInfo[Quest_Item_RewardCount].itemindex = n[1];
		Quest_Item_RewardInfo[Quest_Item_RewardCount].itemid = n[2];
		Quest_Item_RewardInfo[Quest_Item_RewardCount].itemlvl = n[3];
		Quest_Item_RewardInfo[Quest_Item_RewardCount].itemdur = n[4];
		Quest_Item_RewardInfo[Quest_Item_RewardCount].itemskill = n[5];
		Quest_Item_RewardInfo[Quest_Item_RewardCount].itemluck = n[6];
		Quest_Item_RewardInfo[Quest_Item_RewardCount].itemopt = n[7];
		Quest_Item_RewardInfo[Quest_Item_RewardCount].itemexc = n[8];
		Quest_Item_RewardInfo[Quest_Item_RewardCount].itempreco = n[9];
		Quest_Item_RewardCount++;
	}

	rewind(fp);
	fclose(fp);
}
