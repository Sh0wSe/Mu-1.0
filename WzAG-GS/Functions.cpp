//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # DARK PLUGIN - POWERED BY FIRE TEAM
// # GAME SERVER: 99.60T (C) WEBZEN.
// # VERS�O: 1.0.0.0
// # Autor: Maykon
// # Skype: Maykon.ale
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # O Senhor � meu pastor e nada me faltar�!
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "StdAfx.h"

void Functions::HookThis(DWORD dwMyFuncOffset,DWORD dwJmpOffset)
{
	*(DWORD*)(dwJmpOffset+1) = dwMyFuncOffset-(dwJmpOffset+5);
}


void Functions::HookProc(DWORD Offset, DWORD Func)
{
	*(BYTE*)(Offset) = 0xE9;
	*(DWORD*)(Offset+1) = (DWORD)(Func)-(Offset+5);
}

void Functions::SetNop(DWORD Offset,int Size)
{
	for (int n = 0; n < Size; n++)
	{
		*(BYTE*)(Offset + n) = 0x90;
	}
}

void Functions::Set00(DWORD Offset,int Size)
{
	for (int n = 0; n < Size; n++)
	{
		*(BYTE*)(Offset + n) = 0x00;
	}
}

void Functions::SetRetn(DWORD Offset)
{
	*(BYTE*)(Offset) = 0xC3;
}

void Functions::SetRRetn(DWORD Offset)
{
	*(BYTE*)(Offset) = 0xC3;
	*(BYTE*)(Offset+1) = 0x90;
	*(BYTE*)(Offset+2) = 0x90;
	*(BYTE*)(Offset+3) = 0x90;
	*(BYTE*)(Offset+4) = 0x90;
}

void Functions::SetByte(DWORD Offset,BYTE btValue)
{
	*(BYTE*)(Offset) = btValue;
}

void Functions::SetString(DWORD Offset, char* btString, int Size)
{
	memset((char*) Offset, 0x00, Size);
	memcpy((char*) Offset, btString, Size);
}

int Functions::Get_PlayerIndex(char *Name)
{    
	for (int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{     
		if (gObj[i].Connected > 2)
		{
			if (!strcmp(gObj[i].Name, Name))
			{
				return i;
			}
		}
	}

	return -1;
}

void Functions::MosterAdd(int MobID, int MapID, int CoordX, int CoordY)
{
	int mIndex = gObjAddMonster(MapID);

	if (mIndex >= 0)
	{
		gObjSetMonster(mIndex, MobID);
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(mIndex);
		gObj->Class = MobID;
		gObj->MapNumber = MapID;
		gObj->X = CoordX;
		gObj->Y = CoordY;
	}

	//return mIndex;
}

void Functions::GCWarePassSend(int aIndex, PMSG_WAREHOUSEPASSSEND * lpMsg)
{
	switch (lpMsg->Type)
	{
	case 0:
	{
		if (lpMsg->Pass != gObj[aIndex].WarehousePW)
		{
			GCWarehouseStateSend(aIndex, 10);
			return;
		}
		else
		{
			gObj[aIndex].WarehouseLock = 0;
			gObj[aIndex].WarehousePW = 0;
			GCWarehouseStateSend(aIndex, gObj[aIndex].WarehouseLock);
			GCWarehouseRecivePassword(aIndex, lpMsg);
		}
	}
	break;
	case 1:
	{
		gObj[aIndex].WarehouseLock = 1;
		gObj[aIndex].WarehousePW = lpMsg->Pass;
		GCWarehouseStateSend(aIndex, gObj[aIndex].WarehouseLock);
	}
	break;
	case 2:
	{
		if (lpMsg->Pass != gObj[aIndex].WarehousePW)
		{
			GCWarehouseStateSend(aIndex, 10);
			return;
		}
		else
		{
			GCWarehouseStateSend(aIndex, 0);
			GCWarehouseRecivePassword(aIndex, lpMsg);
		}
	}
	break;
	}

	GCWarehouseRecivePassword(aIndex, lpMsg);
}

void Functions::gObjLifeCheckHook(LPOBJ lpTargetObj, LPOBJ lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, BYTE Skill,int iShieldDamage)
{	
	PBYTE a_aIndex = 0;
	PBYTE b_mIndex = 0;

	a_aIndex = (PBYTE)lpObj;
	b_mIndex = (PBYTE)lpTargetObj;

	WORD mIndex = 0;
	WORD aIndex = 0;

	memcpy(&mIndex, b_mIndex+0x00,sizeof(WORD));
	memcpy(&aIndex, a_aIndex+0x00,sizeof(WORD));

	OBJECTSTRUCT *mObj = (OBJECTSTRUCT*) OBJECT_POINTER (mIndex);
	OBJECTSTRUCT *pObj = (OBJECTSTRUCT*) OBJECT_POINTER (aIndex);

	// --------------------------------------------------
	// - [] Quest System
	// --------------------------------------------------
	bool classe = (gObj[aIndex].DbClass == DB_FAIRY_ELF || gObj[aIndex].DbClass == DB_MUSE_ELF) && (QuestUser[aIndex].Quest_Num < Qest_PGW_ELF.Count);

	if(QuestUser[aIndex].Quest_Start == 1 && mObj->Class == Qest_PGW.Number[QuestUser[aIndex].Quest_Num].Mob && mObj->Life <= 0 && !classe)
	{
		Qest_PGW.KilledMob(aIndex);
	}
	if (QuestUser[aIndex].Quest_Start == 1 && mObj->Class == Qest_PGW_ELF.Number[QuestUser[aIndex].Quest_Num].Mob && mObj->Life <= 0 && classe)
	{
		Qest_PGW_ELF.KilledMob(aIndex);
	}
	if (QuestBoss[aIndex].Quest_Start == 1 && mObj->Class == Qest_PGW_Boss.Number[QuestBoss[aIndex].Quest_Num].Mob && mObj->Life <= 0)
	{
		Qest_PGW_Boss.KilledMob(aIndex);
	}
	if (QuestLoot[aIndex].Quest_Start == 1 && mObj->Class == Qest_PGW_Loot.Number[QuestLoot[aIndex].Quest_Num].Mob && mObj->Life <= 0)
	{
		Qest_PGW_Loot.KilledMob(aIndex);
	}
	if (Hydra._Active != 0)
	{
		if (mObj->Life <= 0 && mObj->Live != 0 )
		{	
			if (mObj->Type == MONSTER && mObj->Class == 49)
			{	
				func.FireWork(pObj->m_Index);
				Hydra.Dropitem(pObj->m_Index);
				Hydra._Alive = false;
			}
		}
	}

	gObjLifeCheck(mObj,pObj,AttackDamage,DamageSendType,MSBFlag,MSBDamage,Skill,iShieldDamage); 
}

void Functions::FireWork(int aIndex)
{
	if (gObj[aIndex].X <= 5 || gObj[aIndex].X >= 250 || gObj[aIndex].Y <= 5 || gObj[aIndex].Y >= 250)
	{
		return;
	}

	char PacketRand[5] = {-2, -1, 0, 1, 2};

	BYTE Packet[20][7];

	for (int i = 0; i < 20; i++)
	{
		BYTE RandX = (rand() % 5)*2 - 4;
		BYTE RandY = (rand() % 5)*2 - 4;

		Packet[i][0] = 0xC1;
		Packet[i][1] = 0x07;
		Packet[i][2] = 0xF3;
		Packet[i][3] = 0x40;
		Packet[i][4] = 0x00;
		Packet[i][5] = (BYTE)(gObj[aIndex].X + PacketRand[RandX]);
		Packet[i][6] = (BYTE)(gObj[aIndex].Y + PacketRand[RandY]);
	}

	for (int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{
		if (gObj[i].Connected == 3 /*&& gObj[i].MapNumber == 7*/)
		{
			for (int n = 0; n < 3; n++)
			{
				DataSend(i,Packet[n],(DWORD)Packet[n][1]);
			}
		}
	}
}

BOOL Functions::HookgObjAttack(OBJECTSTRUCT* lpObj, OBJECTSTRUCT* lpTargetObj, class CMagicInf* lpMagic, int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo)
{
	if (lpTargetObj->Authority > 1)
	{
		return FALSE;
	}

	if (!Duel.Attack(lpObj, lpTargetObj))
	{
		return FALSE;
	}

	if (!Pega.Attack(lpObj, lpTargetObj))
	{
		return FALSE;
	}

	if (!Sobre.Attack(lpObj, lpTargetObj))
	{
		return FALSE;
	}

	if (lpObj->Type == PLAYER)	
	{
		func.CheckPet(lpObj);
	}

	if (lpTargetObj->Type == PLAYER)
	{
		func.CheckPet(lpTargetObj);
	}
	if (pGens.ISGENS != FALSE)
	{
		if (GensDisplay(lpObj, lpTargetObj, lpMagic, magicsend, MSBFlag, AttackDamage) != true)
		{
			return true;
		}
	}
	return gObjAttack(lpObj, lpTargetObj, lpMagic, magicsend, MSBFlag, AttackDamage, bCombo);
}

void Functions::gObjPlayerKillerEx(LPOBJ lpObj, LPOBJ lpTargetObj)
{  
	gObjPlayerKiller(lpObj,lpTargetObj); 

	//Fac��es.
	if (pGens.ISGENS != FALSE)
	{
		GensRanking(lpObj, lpTargetObj);
	}
}

void Functions::gObjInterfaceTimeCheckEx(LPOBJ lpObj)
{
	Sobre.PlayerOut(lpObj);
	//func.Check(lpObj);

	gObjInterfaceTimeCheck(lpObj);
}

void Functions::gObjSecondProcEx()
{
	Duel.Run();
	Pega.Run();
	Sobre.Run();
	Tips.Send();
	News.Send();

	for (int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{
		if (gObj[i].Type == PLAYER)
		{
			func.HealthBarSend(&gObj[i]);
			func.ChkJail(&gObj[i]);
			//func.QuestsSend(&gObj[i]);
		}
	}

	gObjSecondProc();
}

void Functions::gObjCloseSetEx(int aIndex, int flag)
{
	Duel.Quit(&gObj[aIndex]);
	Pega.Quit(&gObj[aIndex]);
	Sobre.Quit(&gObj[aIndex]);

	return gObjCloseSet(aIndex,flag);  
}

short Functions::gObjDelEx(int aIndex)
{
	Duel.Quit(&gObj[aIndex]);
	Pega.Quit(&gObj[aIndex]);
	Sobre.Quit(&gObj[aIndex]);

	return gObjDel(aIndex);
}

void Functions::gObjUserDieEx(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	Duel.Die(lpObj);
	Sobre.Die(lpObj,lpTargetObj);

	gObjUserDie(lpObj, lpTargetObj);
}

BYTE Functions::gObjInventoryMoveItemEx(int aIndex, unsigned char source, unsigned char target, int& durSsend, int& durTsend, unsigned char sFlag, unsigned char tFlag, unsigned char* siteminfo)
{
	if (!Pega.Equip(&gObj[aIndex], source, target))
	{
		return (BYTE)(-1);
	}

	return gObjInventoryMoveItem(aIndex, source, target, durSsend, durTsend, sFlag, tFlag, siteminfo);
}

bool Functions::gObjLevelUpEx(LPOBJ lpObj, int addexp, int iMonsterType, int iEventType)
{
	bool Result = gObjLevelUp(lpObj, addexp, iMonsterType, iEventType);

	PMSG_UPDATEPOINTS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xFE;
	pMsg.subcode = 0x08;
	pMsg.LevelUpPoint = lpObj->LevelUpPoint;

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	return Result;
}

BOOL Functions::GetBoxPosition(int MapNumber, int X, int Y, int W, int H, short& rX, short& rY)
{
	BOOL Result = FALSE;

	__asm
	{
		PUSH rY;
		PUSH rX;
		PUSH H;
		PUSH W;
		PUSH Y;
		PUSH X;
		PUSH MapNumber;
		MOV ECX, 0x9541288;
		MOV EDI, 0x403DF5;
		CALL EDI;
		MOV Result, EAX;
	}

	return Result;
}

void Functions::MakeNoticeMsg(void * lpNotice, BYTE btType, char * szNoticeMsg)
{
	PMSG_NOTICE * pNotice = (PMSG_NOTICE *)lpNotice;
	pNotice->type  = btType;

	wsprintf(pNotice->Notice,szNoticeMsg);
	PHeadSetB((LPBYTE)pNotice,0x0D,strlen(pNotice->Notice) + sizeof(PMSG_NOTICE) - sizeof(pNotice->Notice) + 1);
}

void Functions::MsgUser(int aIndex, int Type, char * Msg, ...)
{
	char szBuffer[1024];
	va_list pArguments;
	va_start(pArguments,Msg);
	vsprintf(szBuffer,Msg,pArguments);
	va_end(pArguments);
	PMSG_NOTICE pNotice;
	MakeNoticeMsg(&pNotice,Type,szBuffer);
	DataSend(aIndex,(LPBYTE)&pNotice,pNotice.h.size);
}

void Functions::MsgAllUser(int Type, char * Msg, ...)
{
	char szBuffer[1024];
	va_list pArguments;
	va_start(pArguments,Msg);
	vsprintf(szBuffer,Msg,pArguments);
	va_end(pArguments);
	PMSG_NOTICE pNotice;
	MakeNoticeMsg(&pNotice,Type,szBuffer);
	DataSendAll((LPBYTE)&pNotice,pNotice.h.size);
}

void Functions::AllSendServerMsg(char* chatmsg)
{
	char szBuffer[1024];
	va_list pArguments;
	va_start(pArguments,chatmsg);
	vsprintf(szBuffer,chatmsg,pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;

	func.MakeNoticeMsg(&pNotice,0,szBuffer);

	for(int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{
		if (gObj[i].Connected == 3)
		{
			if (gObj[i].Type == PLAYER)
			{
				DataSend(i,(unsigned char*)&pNotice, pNotice.h.size);
			}
		}
	}
}

void Functions::BlueChatSend(LPCSTR Nome, LPSTR Mensagem, DWORD aIndex)
{
	BYTE Buffer[0xFF] = {0};
	BYTE Len=strlen(Mensagem);
	Buffer[0] = 0xC1;
	Buffer[1] = Len + 15;
	Buffer[2] = 0x00;
	strncpy((char*)&Buffer[3],Nome,10);
	Buffer[13] = 0x7E;
	strncpy((char*)&Buffer[14],Mensagem,Len);

	for (int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{
		if (gObj[i].Connected == 3)
		{
			DataSend(i,(unsigned char*)Buffer,Buffer[1]);
		}
	}
}

void Functions::GreenChatSend(LPCSTR Nome, LPSTR Mensagem, DWORD aIndex)
{
	BYTE Buffer[0xFF] = {0};
	BYTE Len=strlen(Mensagem);
	Buffer[0] = 0xC1;
	Buffer[1] = Len + 15;
	Buffer[2] = 0x00;
	Buffer[13] = 0x40;
	strncpy((char*)&Buffer[3],Nome,10);
	strncpy((char*)&Buffer[14],Mensagem,Len);

	for (int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{
		if (gObj[i].Connected == 3)
		{
			DataSend(i, (unsigned char*)Buffer, Buffer[1]);
		}
	} 
}

void Functions::YellowChatSend(LPCSTR Nome, LPSTR Mensagem, DWORD aIndex)
{
	BYTE Buffer[0xFF] = {0};
	BYTE Len = strlen(Mensagem);
	Buffer[0] = 0xC1;
	Buffer[1] = Len + 14;
	Buffer[2] = 0x02;
	strncpy((char*)&Buffer[3],Nome,10);
	strncpy((char*)&Buffer[13],Mensagem,Len);

	for (int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{
        if (gObj[i].Connected == 3)
		{
			DataSend(i,(unsigned char*)Buffer,Buffer[1]);
		}
	}
}

void Functions::UpdateCharacter(int aIndex, bool Effect)
{
	PMSG_UPDATECHARACTER pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(PMSG_UPDATECHARACTER);
	pMsg.h.headcode = 0xFE;
	pMsg.Code = (Effect == true) ? 1 : 2;
	pMsg.Experience = gObj[aIndex].Experience;
	pMsg.NextExperience = gObj[aIndex].NextExp;
	pMsg.Level = (WORD)(gObj[aIndex].Level);
	pMsg.LevelUpPoint = gObj[aIndex].LevelUpPoint;
	pMsg.Strength = (WORD)(gObj[aIndex].Strength);
	pMsg.Dexterity = (WORD)(gObj[aIndex].Dexterity);
	pMsg.Vitality = (WORD)(gObj[aIndex].Vitality);
	pMsg.Energy = (WORD)(gObj[aIndex].Energy);
	pMsg.Leadership = (WORD)(gObj[aIndex].Leadership);
	pMsg.Life = (WORD)(gObj[aIndex].Life);
	pMsg.MaxLife = (WORD)(gObj[aIndex].MaxLife);
	pMsg.Mana = (WORD)(gObj[aIndex].Mana);
	pMsg.MaxMana = (WORD)(gObj[aIndex].MaxMana);
	pMsg.BP = (WORD)(gObj[aIndex].BP);
	pMsg.MaxBP = (WORD)(gObj[aIndex].MaxBP);

	DataSend(aIndex, (LPBYTE)(&pMsg), pMsg.h.size);

	CBalancer::gObjCalCharacter(aIndex);
	gObjCalcMaxLifePower(aIndex);
}

int Functions::GetInventoryItemCount(int aIndex, int Type, int Level)
{
	int count = 0;

	for(unsigned i = 12; i < 76; i++)
	{
		if(gObj[aIndex].pInventory[i].m_Type == Type)
		{
			if(Level != -1)
			{
				if(!gObj[aIndex].pInventory[i].m_Level == Level)
				{
					continue;
				}
			}

			count++;
		}
	}

	return count;
}

int Functions::gObjDeleteItemsCount(int gObjId, short Type, short Level, int Count)
{
	int count = 0;

	for(int i = 12; i < 76; i++)
	{
		if(gObj[gObjId].pInventory[i].m_Type == Type && gObj[gObjId].pInventory[i].m_Level == Level)
		{
			gObjInventoryDeleteItem(gObjId, i);
			GCInventoryItemDeleteSend(gObjId, i, 1);

			count++;

			if(Count == count)
				break;
		}
	}

	return count;
}

void Functions::Check(LPOBJ lpObj)
{
	Custom[lpObj->m_Index].VipIndex = Manager.VipCount(lpObj->AccountID);
	Custom[lpObj->m_Index].Resets = Manager.CountResets(lpObj->Name);
	Custom[lpObj->m_Index].Masters = Manager.CountMasters(lpObj->Name);
	Custom[lpObj->m_Index].mCash = Manager.GoldCount(lpObj->AccountID);
	Custom[lpObj->m_Index].m_GensState = Manager.genstate(lpObj->Name);
	Custom[lpObj->m_Index].m_GensScore_D = Manager.gensrank(2);
	Custom[lpObj->m_Index].m_GensScore_V = Manager.gensrank(1);
	Custom[lpObj->m_Index].jail = Manager.GetJail(lpObj->Name);
}

void Functions::HealthBarSend(LPOBJ lpObj)
{
	BYTE Buffer[1500];
	PMSG_HEALTHBAR Result;
	PMSG_HEALTHBAR_INFO Info;

	Result.h.c = 0xC2;
	Result.h.headcode = 0xF3;
	Result.Code = 0xE2;
	Result.Count = 0;
	
	int Size = sizeof(Result);

	for (int i = 0; i < MAX_VIEWPORT; i++)
	{
		bool x = (lpObj->VpPlayer[i].type == MONSTER || lpObj->VpPlayer[i].type == PLAYER);
		
		if (!x)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[i].number) == 0)
		{
			continue;
		}

		LPOBJ lpTargetObj = &gObj[lpObj->VpPlayer[i].number];
		x = (lpTargetObj->Class > 99 && lpTargetObj->Class < 105);
		if (lpTargetObj->Class == 200 || lpTargetObj->Live == 0 || lpTargetObj->m_State != 2 || CC_MAP_RANGE(lpTargetObj->MapNumber) != 0 || x)
		{
			continue;
		}

		if (lpObj->VpPlayer[i].type = PLAYER && gObj[lpTargetObj->m_Index].AuthorityCode < 1) {
			Info.gen = Custom[lpTargetObj->m_Index].m_GensState;
		}
		else {
			Info.gen = 0;
		}
		if (lpTargetObj->Class == Qest_PGW.Number[QuestUser[lpObj->m_Index].Quest_Num].Mob) {
			Info.qqnt = Qest_PGW.Number[QuestUser[lpObj->m_Index].Quest_Num].Coun;
			Info.qcnt = QuestUser[lpObj->m_Index].Quest_kill;
			Info.qtip = 1;
		}
		else if (lpTargetObj->Class == Qest_PGW_ELF.Number[QuestUser[lpObj->m_Index].Quest_Num].Mob) {
			Info.qqnt = Qest_PGW_ELF.Number[QuestUser[lpObj->m_Index].Quest_Num].Coun;
			Info.qcnt = QuestUser[lpObj->m_Index].Quest_kill;
			Info.qtip = 1;
		}
		else if (lpTargetObj->Class == Qest_PGW_Loot.Number[QuestLoot[lpObj->m_Index].Quest_Num].Mob) {
			Info.qqnt = Qest_PGW_Loot.Number[QuestLoot[lpObj->m_Index].Quest_Num].Coun;
			Info.qcnt = QuestLoot[lpObj->m_Index].Quest_kill;
			Info.qtip = 2;
		}
		else if (lpTargetObj->Class == Qest_PGW_Boss.Number[QuestBoss[lpObj->m_Index].Quest_Num].Mob) {
			Info.qqnt = Qest_PGW_Boss.Number[QuestBoss[lpObj->m_Index].Quest_Num].Coun;
			Info.qcnt = QuestBoss[lpObj->m_Index].Quest_kill;
			Info.qtip = 3;
		}
		else {
				Info.qqnt = 0;
				Info.qcnt = 0;
				Info.qtip = 0;
		}
		
		Info.Index = lpObj->VpPlayer[i].number;
		Info.Rate = (BYTE)((lpTargetObj->Life * 100) / (lpTargetObj->MaxLife + lpTargetObj->AddLife));

		memcpy(&Buffer[Size], &Info, sizeof(Info));
		Size += sizeof(Info);
		Result.Count++;
	}

	Result.h.sizeH = HIBYTE(Size);
	Result.h.sizeL = LOBYTE(Size);

	memcpy(Buffer, &Result, sizeof(Result));

	DataSend(lpObj->m_Index, Buffer, Size);
}
void Functions::QuestsSend(LPOBJ lpObj)
{
	BYTE Buffer[1500];
	PMSG_QUESTS Result;
	PMSG_QUESTS_INFO Info;

	Result.h.c = 0xC2;
	Result.h.headcode = 0xF3;
	Result.Code = 0xF2;
	Result.Count = 0;

	int Size = sizeof(Result);

	for (int i = 0; i < MAX_VIEWPORT; i++)
	{
		bool x = (lpObj->VpPlayer[i].type == MONSTER);
		if (!x)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[i].number) == 0)
		{
			continue;
		}

		LPOBJ lpTargetObj = &gObj[lpObj->VpPlayer[i].number];

		if (lpTargetObj->Class == 200 || lpTargetObj->Live == 0 || lpTargetObj->m_State != 2 || CC_MAP_RANGE(lpTargetObj->MapNumber) != 0)
		{
			continue;
		}

		if (lpTargetObj->Class == Qest_PGW.Number[QuestUser[lpObj->m_Index].Quest_Num].Mob) {
			Info.qqnt = Qest_PGW.Number[QuestUser[lpObj->m_Index].Quest_Num].Coun;
			Info.qcnt = QuestUser[lpObj->m_Index].Quest_kill;
			Info.qtip = 1;
		}
		else if (lpTargetObj->Class == Qest_PGW_ELF.Number[QuestUser[lpObj->m_Index].Quest_Num].Mob) {
			Info.qqnt = Qest_PGW_ELF.Number[QuestUser[lpObj->m_Index].Quest_Num].Coun;
			Info.qcnt = QuestUser[lpObj->m_Index].Quest_kill;
			Info.qtip = 1;
		}
		else if (lpTargetObj->Class == Qest_PGW_Loot.Number[QuestLoot[lpObj->m_Index].Quest_Num].Mob) {
			Info.qqnt = Qest_PGW_Loot.Number[QuestLoot[lpObj->m_Index].Quest_Num].Coun;
			Info.qcnt = QuestLoot[lpObj->m_Index].Quest_kill;
			Info.qtip = 2;
		}
		else if (lpTargetObj->Class == Qest_PGW_Boss.Number[QuestBoss[lpObj->m_Index].Quest_Num].Mob) {
			Info.qqnt = Qest_PGW_Boss.Number[QuestBoss[lpObj->m_Index].Quest_Num].Coun;
			Info.qcnt = QuestBoss[lpObj->m_Index].Quest_kill;
			Info.qtip = 3;
		}
		else {
			Info.qqnt = 0;
			Info.qcnt = 0;
			Info.qtip = 0;
		}

		Info.Index = lpObj->VpPlayer[i].number;
		memcpy(&Buffer[Size], &Info, sizeof(Info));
		Size += sizeof(Info);
		Result.Count++;
	}

	Result.h.sizeH = HIBYTE(Size);
	Result.h.sizeL = LOBYTE(Size);

	memcpy(Buffer, &Result, sizeof(Result));

	DataSend(lpObj->m_Index, Buffer, Size);
}

void Functions::CheckPet(LPOBJ lpObj)
{
	if (!(lpObj->pInventory[8].m_Type >= ITEMGET(13,0) && lpObj->pInventory[8].m_Type <= ITEMGET(13, 5))) 
	{
		GCInventoryItemDeleteSend(lpObj->m_Index,8,0);
	}
}
bool Functions::SystemIPS( char * ini )
{
	memset((DWORD*)0x004FDACE, 0xEB, 1);
	memset((DWORD*)0x004FDACF, 0x73, 1);

	char Connect_Load[50], IP[16];
	int JS = 0, DS = 0, GS = 0;

	GetPrivateProfileString("GameServerInfo", "Connect_Load", "127.0.0.1 55970 55960 55901", Connect_Load, 50, ini);
	sscanf(Connect_Load, "%s %d %d %d" , IP, &JS, &DS, &GS);


	memcpy((DWORD*)0x0AB550B0, IP, strlen(IP));
	memcpy((DWORD*)0x0AB551B0, IP, strlen(IP));


	*(unsigned int*)0x0A78EF1C = (unsigned int)JS;
	*(unsigned int*)0x0A78EF18 = (unsigned int)DS;
	*(unsigned int*)0x0A78EF20 = (unsigned int)GS;


	return 1;
}

bool Functions::IniciaIni()
{
	char aini[] = "Data\\ServerInfo.dat";
	bool rResultado;
	rResultado = func.SystemIPS(aini);
	return 1;
}

void Functions::LevelUPSend(DWORD aIndex)
{
	// ---
	OBJECTSTRUCT * lpObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);
	// ---
	GCLevelUpMsgSend(aIndex,gObj[aIndex].Level,gObj[aIndex].LevelUpPoint,(WORD)(gObj[aIndex].MaxLife+gObj[aIndex].AddLife),(WORD)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana),(WORD)(gObj[aIndex].MaxBP+gObj[aIndex].AddBP));
	// ---

}

bool Functions::IsOnGame(int iIndex)
{
	if ((iIndex >= 0)&&(iIndex<OBJECT_MAX))
	{

		if (gObj[iIndex].Connected==3)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}

bool Functions::IsBadFileLine(char *FileLine, int &Flag)
{
	if(Flag == 0)
	{
		if(isdigit(FileLine[0]))
		{
			Flag = FileLine[0] - 48;
			return true;
		}
	}
	else if(Flag < 0 || Flag > 9)
	{
		Flag = 0;
	}

	if(!strncmp(FileLine, "end", 3))
	{
		Flag = 0;
		return true;
	}

	if(FileLine[0] == '/' || FileLine[0] == '\n')
		return true;

	for(UINT i = 0; i < strlen(FileLine); i++)
	{
		if(isalnum(FileLine[i]))
			return false;
	}
	return true;
}

int Functions::GenExcOpt(int amount)
{
	// User input errors
	if (amount > 6) amount = 6;
	if (amount < 1) amount = 1;

	int opt_db[6]  = {1, 2, 4, 8, 16, 32};
	int exc = 0;

	std::random_shuffle(opt_db, opt_db + 6);

	for(int n=0; n < amount; n++)
		exc += opt_db[n];

	return exc;
}

void Functions::MsgOutputAll(char *message, ...)
{
	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, message);
	vsprintf(&szBuffer[0], message, pArguments);
	va_end(pArguments);

	for(int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{
		if(gObj[i].Connected == 3)
		{
			GCServerMsgStringSend(&szBuffer[0], i, 1);
		}
	}
}

bool Functions::WarehouseHacker(int aIndex)
{
	if(gObj[aIndex].WarehouseSave >= 1 )
	{
		CGWarehouseUseEnd(aIndex);
		return TRUE;
	}
	return FALSE;
}

int	Functions::MyRand(int min, int max)
{
	if(min == max) 
		return min;

	int rnd = rand();
	int r = min + rnd / (RAND_MAX / (max - min) + 1);

	return r;
}
void Functions::ChkJail(LPOBJ lpObj) {
	int aIndex = lpObj->m_Index;
	if (Custom[aIndex].jail == 1 && gObj[aIndex].Connected == 3) {
		if (gObj[aIndex].MapNumber > 0 || gObj[aIndex].X >= 129 || gObj[aIndex].X <= 126 || gObj[aIndex].Y >= 120 || gObj[aIndex].Y <= 116) {
			gObj[aIndex].m_PK_Level = 6;
			gObj[aIndex].m_PK_Count = 20;
			gObj[aIndex].m_PK_Time = 100;
			GCPkLevelSend(aIndex, gObj[aIndex].m_PK_Level);
			gObjTeleport(aIndex, 0, 128, 118);
		}
	}
}

void StartAddress(LPVOID lpThreadParameter)
{
	HANDLE v1;
	HANDLE v2;

	while ( TRUE )
	{
		Sleep(5000);
		v1 = GetCurrentProcess();
		SetProcessWorkingSetSize(v1, 0xFFFFFFFF, 0xFFFFFFFF);
		v2 = GetCurrentProcess();
		SetThreadPriority(v2, -2);
	}
}
Functions func;