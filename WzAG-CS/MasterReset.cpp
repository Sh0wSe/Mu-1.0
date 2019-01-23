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

bool cMReset::Load()
{
	this->_Active = GetPrivateProfileInt("Master Reset","Active",1,CFG_MRESET) > 0 ? true : false;
	GetPrivateProfileString("Master Reset","Sintax","/mreset",this->_Syntax,50,CFG_MRESET);

	MReset._Level[0] = GetPrivateProfileInt("Free","NeedLevel",1,CFG_MRESET);
	MReset._Zen[0] = GetPrivateProfileInt("Free","NeedZen",1,CFG_MRESET);
	MReset._NeedFor[0] = GetPrivateProfileInt("Free","NeedStr",1,CFG_MRESET);
	MReset._NeedAgi[0] = GetPrivateProfileInt("Free","NeedAgi",1,CFG_MRESET);
	MReset._NeedVit[0] = GetPrivateProfileInt("Free","NeedVit",1,CFG_MRESET);
	MReset._NeedEne[0] = GetPrivateProfileInt("Free","NeedEne",1,CFG_MRESET);
	MReset._NeedCmd[0] = GetPrivateProfileInt("Free","NeedCmd",1,CFG_MRESET);
	MReset._NextLevel[0] = GetPrivateProfileInt("Free","ReturnLevel",1,CFG_MRESET);
	MReset._Zerar[0] = GetPrivateProfileInt("Free","Zerar status",1,CFG_MRESET);

	MReset._Level[1] = GetPrivateProfileInt("Vip1","NeedLevel",1,CFG_MRESET);
	MReset._Zen[1] = GetPrivateProfileInt("Vip1","NeedZen",1,CFG_MRESET);
	MReset._NeedFor[1] = GetPrivateProfileInt("Vip1","NeedStr",1,CFG_MRESET);
	MReset._NeedAgi[1] = GetPrivateProfileInt("Vip1","NeedAgi",1,CFG_MRESET);
	MReset._NeedVit[1] = GetPrivateProfileInt("Vip1","NeedVit",1,CFG_MRESET);
	MReset._NeedEne[1] = GetPrivateProfileInt("Vip1","NeedEne",1,CFG_MRESET);
	MReset._NeedCmd[1] = GetPrivateProfileInt("Vip1","NeedCmd",1,CFG_MRESET);
	MReset._NextLevel[1] = GetPrivateProfileInt("Vip1","ReturnLevel",1,CFG_MRESET);
	MReset._Zerar[1] = GetPrivateProfileInt("Vip1","Zerar status",1,CFG_MRESET);

	MReset._Level[2] = GetPrivateProfileInt("Vip2","NeedLevel",1,CFG_MRESET);
	MReset._Zen[2] = GetPrivateProfileInt("Vip2","NeedZen",1,CFG_MRESET);
	MReset._NeedFor[2] = GetPrivateProfileInt("Vip2","NeedStr",1,CFG_MRESET);
	MReset._NeedAgi[2] = GetPrivateProfileInt("Vip2","NeedAgi",1,CFG_MRESET);
	MReset._NeedVit[2] = GetPrivateProfileInt("Vip2","NeedVit",1,CFG_MRESET);
	MReset._NeedEne[2] = GetPrivateProfileInt("Vip2","NeedEne",1,CFG_MRESET);
	MReset._NeedCmd[2] = GetPrivateProfileInt("Vip2","NeedCmd",1,CFG_MRESET);
	MReset._NextLevel[2] = GetPrivateProfileInt("Vip2","ReturnLevel",1,CFG_MRESET);
	MReset._Zerar[2] = GetPrivateProfileInt("Vip2","Zerar status",1,CFG_MRESET);

	MReset._Level[3] = GetPrivateProfileInt("Vip3","NeedLevel",1,CFG_MRESET);
	MReset._Zen[3] = GetPrivateProfileInt("Vip3","NeedZen",1,CFG_MRESET);
	MReset._NeedFor[3] = GetPrivateProfileInt("Vip3","NeedStr",1,CFG_MRESET);
	MReset._NeedAgi[3] = GetPrivateProfileInt("Vip3","NeedAgi",1,CFG_MRESET);
	MReset._NeedVit[3] = GetPrivateProfileInt("Vip3","NeedVit",1,CFG_MRESET);
	MReset._NeedEne[3] = GetPrivateProfileInt("Vip3","NeedEne",1,CFG_MRESET);
	MReset._NeedCmd[3] = GetPrivateProfileInt("Vip3","NeedCmd",1,CFG_MRESET);
	MReset._NextLevel[3] = GetPrivateProfileInt("Vip3","ReturnLevel",1,CFG_MRESET);
	MReset._Zerar[3] = GetPrivateProfileInt("Vip3","Zerar status",1,CFG_MRESET);

	GetPrivateProfileString("Querys","UPDATE MRESET","",this->_Query[0],255,CFG_QUERY);
	GetPrivateProfileString("Querys","UPDATE MRESET FREE","",this->_Query[1],255,CFG_QUERY);
	GetPrivateProfileString("Querys","UPDATE MRESET VIP1","",this->_Query[2],255,CFG_QUERY);
	GetPrivateProfileString("Querys","UPDATE MRESET VIP2","",this->_Query[3],255,CFG_QUERY);
	GetPrivateProfileString("Querys","UPDATE MRESET VIP3","",this->_Query[4],255,CFG_QUERY);

	return true;
}

void cMReset::Increase(int aIndex, int Num)
{
	switch (Num)
	{
	case 0:
		{
			Manager.ExecFormat(this->_Query[0],gObj[aIndex].Name);
			Manager.ExecFormat(this->_Query[1],gObj[aIndex].AccountID);
		}
		break;
	case 1:
		{
			Manager.ExecFormat(this->_Query[0],gObj[aIndex].Name);
			Manager.ExecFormat(this->_Query[2],gObj[aIndex].AccountID);
		}
		break;
	case 2:
		{
			Manager.ExecFormat(this->_Query[0],gObj[aIndex].Name);
			Manager.ExecFormat(this->_Query[3],gObj[aIndex].AccountID);
		}
		break;
	case 3:
		{
			Manager.ExecFormat(this->_Query[0],gObj[aIndex].Name);
			Manager.ExecFormat(this->_Query[4],gObj[aIndex].AccountID);
		}
		break;
	}
}

void cMReset::InitExec(int aIndex)
{
	if (!this->_Active)
	{
		func.MsgUser(aIndex,1,"Comando desabilitado.");
		return;
	}
	else if (MReset._Level[Custom[aIndex].VipIndex] > gObj[aIndex].Level)
	{
		func.MsgUser(aIndex,1,"Voc� n�o possui level %d",MReset._Level[Custom[aIndex].VipIndex]);
		return;
	}
	else if (MReset._Zen[Custom[aIndex].VipIndex] > gObj[aIndex].Money)
	{
		func.MsgUser(aIndex,1,"Voc� n�o possui zen %d",MReset._Zen[Custom[aIndex].VipIndex]);
		return;
	}
	else if (gObj[aIndex].Strength < MReset._NeedFor[Custom[aIndex].VipIndex])
	{
		func.MsgUser(aIndex,1,"Voc� deve ser full em for�a.");
		return;
	}
	else if (gObj[aIndex].Dexterity < MReset._NeedAgi[Custom[aIndex].VipIndex])
	{
		func.MsgUser(aIndex,1,"Voc� deve ser full em agilidade.");
		return;
	}
	else if (gObj[aIndex].Vitality < MReset._NeedVit[Custom[aIndex].VipIndex])
	{
		func.MsgUser(aIndex,1,"Voc� deve ser full em vitalidade.");
		return;
	}
	else if (gObj[aIndex].Energy < MReset._NeedEne[Custom[aIndex].VipIndex])
	{
		func.MsgUser(aIndex,1,"Voc� deve ser full em energia.");
		return;
	}
	else if (gObj[aIndex].Leadership < MReset._NeedCmd[Custom[aIndex].VipIndex] && gObj[aIndex].Class == 4)
	{
		func.MsgUser(aIndex,1,"Voc� deve ser full em comando.");
		return;
	}
	else
	{
		if (MReset._Zerar[Custom[aIndex].VipIndex] == 1)
		{
			gObj[aIndex].Strength = 25;
			gObj[aIndex].Dexterity = 25;
			gObj[aIndex].Vitality = 25;
			gObj[aIndex].Energy = 25;

			if (gObj[aIndex].Class == 4)
			{
				gObj[aIndex].Leadership = 25;
			}
		}

		gObj[aIndex].Level             = MReset._NextLevel[Custom[aIndex].VipIndex];
		gObj[aIndex].Money             -= MReset._Zen[Custom[aIndex].VipIndex];
		gObj[aIndex].Experience        = 0;
		gObj[aIndex].Level			   = MReset._NextLevel[Custom[aIndex].VipIndex];
		GCMoneySend(aIndex,gObj[aIndex].Money);

		MReset.Increase(aIndex,Custom[aIndex].VipIndex);

		if (gObj[aIndex].Class == WIZARD || gObj[aIndex].Class == KNIGHT || gObj[aIndex].Class == MAGUMSA || gObj[aIndex].Class == DARKLORD)
		{
			gObjTeleport(aIndex,0,134,128);
		}
		else if (gObj[aIndex].Class == ELF)
		{
			gObjTeleport(aIndex,3,174,114);
		}

		gObj[aIndex].MaxLife = gObj[aIndex].VitalityToLife * gObj[aIndex].Vitality;
	    gObj[aIndex].Life = gObj[aIndex].MaxLife;
		gObj[aIndex].AddLife = 0;
		gObj[aIndex].MaxMana = gObj[aIndex].EnergyToMana * gObj[aIndex].Energy;
		gObj[aIndex].Mana = gObj[aIndex].MaxMana;
		gObj[aIndex].AddMana = 0;
		gObjSetBP(aIndex);
		gObj[aIndex].BP = gObj[aIndex].MaxBP;
		gObj[aIndex].AddBP = 0;
		func.UpdateCharacter(aIndex, true);
		func.MsgUser(aIndex,1,"Master Reset efetuado com sucesso.");
	}
}

cMReset MReset;
