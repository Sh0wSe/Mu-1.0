#include "StdAfx.h"


Q_PGW_LOOT Qest_PGW_Loot;

void Q_PGW_LOOT::Config()
{
	int EnableExQuest = GetPrivateProfileInt("Quest", "Ativar", 1, CFG_QUEST_LOOT);

	if (EnableExQuest < 1) {
		return;
	}
}

bool Q_PGW_LOOT::IsBadFileLine(char *FileLine, int &Flag)
{
	Qest_PGW_Loot.Config();

	if (Flag == 0)
	{
		if (isdigit(FileLine[0]))
		{
			Flag = FileLine[0] - 48;
			return true;
		}
	}
	else if (Flag < 0 || Flag > 9)
	{
		Flag = 0;
	}

	if (!strncmp(FileLine, "end", 3))
	{
		Flag = 0;
		return true;
	}

	if (FileLine[0] == '/' || FileLine[0] == '\n')
		return true;

	for (UINT i = 0; i < strlen(FileLine); i++)
	{
		if (isalnum(FileLine[i]))
			return false;
	}
	return true;
}

void Q_PGW_LOOT::Q_Num()
{
	for (int i(0); i<1000; i++)
	{
		Number[i].Mob = 0;
		Number[i].Coun = 0;
		Number[i].proc = 0;
		Number[i].rew = 0;
		Number[i].gift = 0;
		Number[i].Zen = 0;
		Number[i].exp = 0;
		Number[i].lvl =0;
		Number[i].resets = 0;
		Number[i].teleport = 0;
		Number[i].map = 0;
		Number[i].x = 0;
		Number[i].y = 0;
		Number[i].reqmap = 0;
		Number[i].msg[0] = NULL;
		Number[i].msg2[0] = NULL;
		Number[i].msg3[0] = NULL;
	}
}

void Q_PGW_LOOT::Q_Load()
{
	Qest_PGW_Loot.Config();

	Q_Num();
	FILE *file;
	file = fopen(CFG_QUEST_LOOT, "r");
	if (file == NULL)
	{
		MessageBoxA(0, CFG_QUEST_LOOT, "CRITICAL ERROR", 0);
		ExitProcess(1);
		return;
	}
	char Buff[256];
	int Flag = 0;
	Count = 0;

	while (!feof(file))
	{
		fgets(Buff, 256, file);

		if (IsBadFileLine(Buff, Flag)) continue;

		if (Flag == 1)
		{
			int n[14];
			char mes[100];
			char mes2[100];
			char mes3[100];
			//gets(mes);
			sscanf(Buff, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d \"%[^\"]\" \"%[^\"]\" \"%[^\"]\"", &n[0], &n[1], &n[2], &n[3], &n[4], &n[5], &n[6], &n[7], &n[8], &n[9], &n[10], &n[11], &n[12], &n[13], &mes, &mes2, &mes3);
			Number[Count].Mob = n[0];
			Number[Count].Coun = n[1];
			Number[Count].proc = n[2];
			Number[Count].rew = n[3];
			Number[Count].gift = n[4];
			Number[Count].Zen = n[5];
			Number[Count].exp = n[6];
			Number[Count].lvl = n[7];
			Number[Count].resets = n[8];
			Number[Count].teleport = n[9];
			Number[Count].map = n[10];
			Number[Count].x = n[11];
			Number[Count].y = n[12];
			Number[Count].reqmap = n[13];
			sprintf(Number[Count].msg, "%s", mes);
			sprintf(Number[Count].msg2, "%s", mes2);
			sprintf(Number[Count].msg3, "%s", mes3);
			Count++;
		}
	}
	fclose(file);
}

void Q_PGW_LOOT::Q_CreateStruct(int aIndex)
{
	OBJECTSTRUCT * lpObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);

	QuestLoot[aIndex].Quest_Num = Manager.QuestLoot_Num(gObj[aIndex].Name);
	//-------------------------------------------------------------------------
	QuestLoot[aIndex].Quest_kill = Manager.QuestLoot_Kill(gObj[aIndex].Name);
	//-------------------------------------------------------------------------
	QuestLoot[aIndex].Quest_Start = Manager.QuestLoot_Start(gObj[aIndex].Name);
	//-------------------------------------------------------------------------
	
	if (QuestLoot[aIndex].Quest_kill < 0){QuestLoot[aIndex].Quest_kill = 0;}
	if (QuestLoot[aIndex].Quest_Num < 0){QuestLoot[aIndex].Quest_Num = 0;}
	if (QuestLoot[aIndex].Quest_Start < 0){QuestLoot[aIndex].Quest_Start = 0;}
}

void Q_PGW_LOOT::Q_NPC(int aIndex, int aNPC)
{
	Qest_PGW_Loot.Config();

	OBJECTSTRUCT * lpObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);
	OBJECTSTRUCT *gObjNPC = (OBJECTSTRUCT*)OBJECT_POINTER(aNPC);

	if ((gObjNPC->Class == NPC_Quest_Loot))
	{

		if (QuestLoot[aIndex].Quest_Num < Count)
		{

			if (QuestLoot[aIndex].Quest_Start == 0)
			{
				if (QuestLoot[aIndex].Quest_Num == 0 && lpObj->Level >= Number[QuestLoot[aIndex].Quest_Num].lvl && Custom[aIndex].Resets >= Number[QuestLoot[aIndex].Quest_Num].resets &&  lpObj->MapNumber == Number[QuestLoot[aIndex].Quest_Num].reqmap)
				{
					ChatTargetSend(gObjNPC, "[Quest] Aceita!", aIndex);
					QuestLoot[aIndex].Quest_Start = 1;
					//Manager.ExecFormat("UPDATE [MuOnline].[dbo].[Character] SET Quest_Loot_Start = 1 WHERE Name='%s'", lpObj->Name);
					MsgOutput(aIndex, "[Quest] Quest N: %d", QuestLoot[aIndex].Quest_Num + 1);
					MsgOutput(aIndex, "[Quest] %s", Number[QuestLoot[aIndex].Quest_Num].msg);
					MsgOutput(aIndex, "[Quest] Mate %s [%d/%d]", Number[QuestLoot[aIndex].Quest_Num].msg2, QuestLoot[aIndex].Quest_kill, Number[QuestLoot[aIndex].Quest_Num].Coun);
					if (Number[QuestLoot[aIndex].Quest_Num].teleport > 0) {
						gObjTeleport(gObj[aIndex].m_Index, Number[QuestLoot[aIndex].Quest_Num].map, Number[QuestLoot[aIndex].Quest_Num].x, Number[QuestLoot[aIndex].Quest_Num].y);
					}
					return;
				}
				else if (QuestLoot[aIndex].Quest_Num > 0 && lpObj->Level >= Number[QuestLoot[aIndex].Quest_Num].lvl && Custom[aIndex].Resets >= Number[QuestLoot[aIndex].Quest_Num].resets)
				{
					QuestLoot[aIndex].Quest_Start = 1;
					//Manager.ExecFormat("UPDATE [MuOnline].[dbo].[Character] SET Quest_Loot_Start = 1 WHERE Name='%s'", lpObj->Name);
					MsgOutput(aIndex, "[Quest] Quest N: %d", QuestLoot[aIndex].Quest_Num + 1);
					MsgOutput(aIndex, "[Quest] %s", Number[QuestLoot[aIndex].Quest_Num].msg);
					MsgOutput(aIndex, "[Quest] Mate %s [%d/%d]", Number[QuestLoot[aIndex].Quest_Num].msg2, QuestLoot[aIndex].Quest_kill, Number[QuestLoot[aIndex].Quest_Num].Coun);
					ChatTargetSend(gObjNPC, "Complete essa nova Quest!", aIndex);
					if (Number[QuestLoot[aIndex].Quest_Num].teleport > 0) {
						gObjTeleport(gObj[aIndex].m_Index, Number[QuestLoot[aIndex].Quest_Num].map, Number[QuestLoot[aIndex].Quest_Num].x, Number[QuestLoot[aIndex].Quest_Num].y);
					}
					return;
				}
				else if (lpObj->Level < Number[QuestLoot[aIndex].Quest_Num].lvl && Custom[aIndex].Resets < Number[QuestLoot[aIndex].Quest_Num].resets) {
					MsgOutput(aIndex, "[Quest]Volte no level: %d", Number[QuestLoot[aIndex].Quest_Num].lvl);
					MsgOutput(aIndex, "[Quest]Volte com %d resets", Number[QuestLoot[aIndex].Quest_Num].resets);
				}
				else if (lpObj->Level < Number[QuestLoot[aIndex].Quest_Num].lvl) {
					MsgOutput(aIndex, "[Quest]Volte no level: %d", Number[QuestLoot[aIndex].Quest_Num].lvl);
				}
				else if (Custom[aIndex].Resets < Number[QuestLoot[aIndex].Quest_Num].resets) {
					MsgOutput(aIndex, "[Quest]Volte com %d resets", Number[QuestLoot[aIndex].Quest_Num].resets);
				}
				else if (lpObj->MapNumber != Number[QuestLoot[aIndex].Quest_Num].reqmap) {
					MsgOutput(aIndex, "[Quest] %s", Number[QuestLoot[aIndex].Quest_Num].msg3);
				}
				else {
					MsgOutput(aIndex, "Verificar esse erro...");
				}
			}
			else if (QuestLoot[aIndex].Quest_Start == 1)
			{

				if (QuestLoot[aIndex].Quest_kill == Number[QuestLoot[aIndex].Quest_Num].Coun)
				{
					int ExQuest_gift = Presents(aIndex, Number[QuestLoot[aIndex].Quest_Num].rew, Number[QuestLoot[aIndex].Quest_Num].gift);
					if (ExQuest_gift == false)
					{
						ChatTargetSend(gObjNPC, "Premia��o desativada!", aIndex);
						return;
					}

					if (QuestLoot[aIndex].Quest_Num == Count)
					{
						MsgOutput(aIndex, "[Quest] Finalizada");
						return;
					}

					QuestLoot[aIndex].Quest_Start = 0;
					QuestLoot[aIndex].Quest_Num++;
					QuestLoot[aIndex].Quest_kill = 0;
					ChatTargetSend(gObjNPC, "Parab�ns!", aIndex);
					//Manager.ExecFormat("UPDATE [MuOnline].[dbo].[Character] SET Quest_Loot_Start = 0 WHERE Name='%s'", lpObj->Name);
					//Manager.ExecFormat("UPDATE [MuOnline].[dbo].[Character] SET Quest_Loot_Kill = 0 WHERE Name='%s'", lpObj->Name);
					//Manager.ExecFormat("UPDATE [MuOnline].[dbo].[Character] SET Quest_Loot_Num = Quest_Loot_Num + 1 WHERE Name='%s'", lpObj->Name);
					return;
				}
				else
				{
					ChatTargetSend(gObjNPC, "Volte quando tiver terminado!", aIndex);
					MsgOutput(aIndex, "[Quest] %s", Number[QuestLoot[aIndex].Quest_Num].msg);
					MsgOutput(aIndex, "[Quest] %s [%d/%d]", Number[QuestLoot[aIndex].Quest_Num].msg2, QuestLoot[aIndex].Quest_kill, Number[QuestLoot[aIndex].Quest_Num].Coun);
					return;
				}
			}
		}
		else
		{
			ChatTargetSend(gObjNPC, "Voc� completou todas as Quest!", aIndex);
			MsgOutput(aIndex, "Voc� completou todas as Quest");
			return;
		}
	}
}

void Q_PGW_LOOT::KilledMob(int aIndex)
{
	Qest_PGW_Loot.Config();
	OBJECTSTRUCT * lpObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);
	int iRate = rand() % 100 + 1;
	if (iRate >= 0)
	{

		if (QuestLoot[aIndex].Quest_kill < Number[QuestLoot[aIndex].Quest_Num].Coun)
		{
			QuestLoot[aIndex].Quest_kill++;
			
			
			if (QuestLoot[aIndex].Quest_kill == Number[QuestLoot[aIndex].Quest_Num].Coun)
			{
				MsgOutput(aIndex, "[Quest Loot] Retorne ao NPC!");
				func.MsgUser(aIndex, 0, "[Quest Loot] Retorne ao NPC!");
			}
		}
	}
}

bool Q_PGW_LOOT::Presents(int aIndex, int Present, int Gifts)
{
	OBJECTSTRUCT * lpObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);

	switch (Present)
	{

	case 1:
	{
		lpObj->LevelUpPoint += Gifts;
		lpObj->Level += Number[QuestLoot[aIndex].Quest_Num].exp;
		if (lpObj->DbClass == DB_MAGIC_GLADIATOR || lpObj->DbClass == DB_DARK_LORD) {
			lpObj->LevelUpPoint += Number[QuestLoot[aIndex].Quest_Num].exp * 7;
		}
		else {
			lpObj->LevelUpPoint += Number[QuestLoot[aIndex].Quest_Num].exp * 5;
		}
		GCMoneySend(aIndex, lpObj->Money += Number[QuestLoot[aIndex].Quest_Num].Zen);
		func.UpdateCharacter(aIndex, false);
		MsgOutput(aIndex, "[Quest] %s  Premiado com %d Points", lpObj->Name, Gifts);
		if (Number[QuestLoot[aIndex].Quest_Num].Zen > 0) { MsgOutput(aIndex, "[Quest] %s  Premiado com %d Zen", lpObj->Name, Number[QuestLoot[aIndex].Quest_Num].Zen); }
		if (Number[QuestLoot[aIndex].Quest_Num].exp > 0) { MsgOutput(aIndex, "[Quest] %s  Premiado com %d Level", lpObj->Name, Number[QuestLoot[aIndex].Quest_Num].exp); }
		func.LevelUPSend(aIndex);
	}
	break;
	case 2:
	{
		Manager.ExecFormat("UPDATE [MuOnline].[dbo].[Character] SET Resets = Resets + %d WHERE Name='%s'", lpObj->Name);
		lpObj->Level += Number[QuestLoot[aIndex].Quest_Num].exp;
		if (lpObj->DbClass == DB_MAGIC_GLADIATOR || lpObj->DbClass == DB_DARK_LORD) {
			lpObj->LevelUpPoint += Number[QuestLoot[aIndex].Quest_Num].exp * 7;
		}
		else {
			lpObj->LevelUpPoint += Number[QuestLoot[aIndex].Quest_Num].exp * 5;
		}
		MsgOutput(aIndex, "[Quest] %s  Premiado com %d Resets", lpObj->Name, Gifts);
		if (Number[QuestLoot[aIndex].Quest_Num].Zen > 0) { MsgOutput(aIndex, "[Quest] %s  Premiado com %d Zen", lpObj->Name, Number[QuestLoot[aIndex].Quest_Num].Zen); }
		if (Number[QuestLoot[aIndex].Quest_Num].exp > 0) { MsgOutput(aIndex, "[Quest] %s  Premiado com %d Level", lpObj->Name, Number[QuestLoot[aIndex].Quest_Num].exp); }
		GCMoneySend(aIndex, lpObj->Money += Number[QuestLoot[aIndex].Quest_Num].Zen);
		func.LevelUPSend(aIndex);

	}
	break;
	case 3:
	{
		Manager.ExecFormat("UPDATE [MuOnline].[dbo].[MEMB_INFO] SET Gold = Gold + %d WHERE memb___id='%s'", lpObj->AccountID);
		lpObj->Level += Number[QuestLoot[aIndex].Quest_Num].exp;
		if (lpObj->DbClass == DB_MAGIC_GLADIATOR || lpObj->DbClass == DB_DARK_LORD) {
			lpObj->LevelUpPoint += Number[QuestLoot[aIndex].Quest_Num].exp * 7;
		}
		else {
			lpObj->LevelUpPoint += Number[QuestLoot[aIndex].Quest_Num].exp * 5;
		}
		MsgOutput(aIndex, "[Quest] %s  Premiado com %d Golds", lpObj->Name, Gifts);
		if (Number[QuestLoot[aIndex].Quest_Num].Zen > 0) { MsgOutput(aIndex, "[Quest] %s  Premiado com %d Zen", lpObj->Name, Number[QuestLoot[aIndex].Quest_Num].Zen); }
		if (Number[QuestLoot[aIndex].Quest_Num].exp > 0) { MsgOutput(aIndex, "[Quest] %s  Premiado com %d Level", lpObj->Name, Number[QuestLoot[aIndex].Quest_Num].exp); }
		GCMoneySend(aIndex, lpObj->Money += Number[QuestLoot[aIndex].Quest_Num].Zen);
		func.LevelUPSend(aIndex);
	}
	break;
	case 4: {
		char  str[20];
		_itoa(Gifts, str, 10);
		int qtd = 1;
		char cmd[255] = "";
		int index = 0;
		int id = 0;
		int lvl = 0;
		int dur = 0;
		int opt = 0;
		int exc = 0;
		int luck = 0;
		int skill = 0;
		int preco = 0;
		bool existe = 0;
		int saldo = 0;
		int exib;
		for (int x = 0; x < Quest_Item_RewardCount; x++)
		{
			if (!strcmp(Quest_Item_RewardInfo[x].itemnome, str) || !strcmp(Quest_Item_RewardInfo[x].itemexib, str))
			{
				exib = x;
				index = Quest_Item_RewardInfo[x].itemindex;
				id = Quest_Item_RewardInfo[x].itemid;
				lvl = Quest_Item_RewardInfo[x].itemlvl;
				dur = Quest_Item_RewardInfo[x].itemdur;
				opt = Quest_Item_RewardInfo[x].itemopt;
				exc = Quest_Item_RewardInfo[x].itemexc;
				luck = Quest_Item_RewardInfo[x].itemluck;
				skill = Quest_Item_RewardInfo[x].itemskill;
				preco = Quest_Item_RewardInfo[x].itempreco;
				existe = 1;
			}
		}
		if (existe) {
			sprintf(cmd, "%d %d %d %d %d %d %d %d", index, id, lvl, dur, skill, luck, opt, exc);
			Command.makecomprar(aIndex, cmd, ("Quest Reward %s", Quest_Item_RewardInfo[exib].itemexib));
			lpObj->Level += Number[QuestLoot[aIndex].Quest_Num].exp;
			if (lpObj->DbClass == DB_MAGIC_GLADIATOR || lpObj->DbClass == DB_DARK_LORD) {
				lpObj->LevelUpPoint += Number[QuestLoot[aIndex].Quest_Num].exp * 7;
			}
			else {
				lpObj->LevelUpPoint += Number[QuestLoot[aIndex].Quest_Num].exp * 5;
			}
			GCMoneySend(aIndex, lpObj->Money += Number[QuestLoot[aIndex].Quest_Num].Zen);
			MsgOutput(aIndex, "[Quest] %s  Premiado com %s ", lpObj->Name, Quest_Item_RewardInfo[exib].itemexib);
			if (Number[QuestLoot[aIndex].Quest_Num].Zen > 0) { MsgOutput(aIndex, "[Quest] %s  Premiado com %d Zen", lpObj->Name, Number[QuestLoot[aIndex].Quest_Num].Zen); }
			if (Number[QuestLoot[aIndex].Quest_Num].exp > 0) { MsgOutput(aIndex, "[Quest] %s  Premiado com %d Level", lpObj->Name, Number[QuestLoot[aIndex].Quest_Num].exp); }
			func.LevelUPSend(aIndex);
		}
	}
			break;
	}

	return true;
}

