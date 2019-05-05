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

void ConnectServer::PlayerConnect(int aIndex)
{
	OBJECTSTRUCT *pObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);
	Custom[aIndex].VipIndex = Manager.VipCount(gObj[aIndex].AccountID);
	Custom[aIndex].Resets = Manager.CountResets(gObj[aIndex].Name);
	Custom[aIndex].Masters = Manager.CountMasters(gObj[aIndex].Name);
	Custom[aIndex].mCash = Manager.GoldCount(gObj[aIndex].AccountID);
	Custom[aIndex].jail = Manager.GetJail(gObj[aIndex].Name);
	Custom[aIndex].jaula = Manager.GetJaula(gObj[aIndex].Name);

	GetPrivateProfileString("Server","Noticia de conex�o","Bem vindo(a) %s",this->Notice[0],160,CFG_GAMESERVER);
	func.MsgUser(aIndex,0,this->Notice[0],gObj[aIndex].Name);

	if(Custom[aIndex].VipIndex == Free)
	{
		GetPrivateProfileString("Server","Noticia de conex�o Free","[%s] Voc� � um usu�rio(a): Free",this->Notice[1],160,CFG_GAMESERVER);
		func.MsgUser(aIndex,0,this->Notice[1],gObj[aIndex].Name);
	}

	if(Custom[aIndex].VipIndex == Bronze)
	{
		GetPrivateProfileString("Server","Noticia de conex�o Vip1","[%s] Voc� � um usu�rio(a): VIP Bronze",this->Notice[2],160,CFG_GAMESERVER);
		func.MsgUser(aIndex,0,"Experiencia adicional %d",Vips.Exp1);
		func.MsgUser(aIndex,0,this->Notice[2],gObj[aIndex].Name);
	}

	if(Custom[aIndex].VipIndex == Prata)
	{
		GetPrivateProfileString("Server","Noticia de conex�o Vip2","[%s] Voc� � um usu�rio(a): VIP Prata",this->Notice[3],160,CFG_GAMESERVER);
		func.MsgUser(aIndex,0,"Experiencia %d",Vips.Exp2);
		func.MsgUser(aIndex,0,this->Notice[3],gObj[aIndex].Name);
	}

	if(Custom[aIndex].VipIndex == Ouro)
	{
		GetPrivateProfileString("Server","Noticia de conex�o Vip3","[%s] Voc� � um usu�rio(a): VIP Ouro",this->Notice[4],160,CFG_GAMESERVER);
		func.MsgUser(aIndex,0,"Experiencia Adicional %d",Vips.Exp3);
		func.MsgUser(aIndex,0,this->Notice[4],gObj[aIndex].Name);
	}
	if (Custom[aIndex].VipIndex == Platina)
	{
		GetPrivateProfileString("Server", "Noticia de conex�o Vip4", "[%s] Voc� � um usu�rio(a): VIP Platina", this->Notice[5], 160, CFG_GAMESERVER);
		func.MsgUser(aIndex, 0, "Experiencia Adicional %d", Vips.Exp4);
		func.MsgUser(aIndex, 0, this->Notice[5], gObj[aIndex].Name);
	}
	if (Custom[aIndex].VipIndex == Infinity)
	{
		GetPrivateProfileString("Server", "Noticia de conex�o Vip5", "[%s] Voc� � um usu�rio(a): VIP Infinity", this->Notice[6], 160, CFG_GAMESERVER);
		func.MsgUser(aIndex, 0, "Experiencia Adicional %d", Vips.Exp5);
		func.MsgUser(aIndex, 0, this->Notice[6], gObj[aIndex].Name);
	}
}

void ConnectServer::GameMasterConnect(int aIndex)
{
	char ConnectNoticePlayer[160];

	GetPrivateProfileString("Server","Noticia de conex�o de GM/ADM"," ",this->Notice[7],160,CFG_GAMESERVER);
	GetPrivateProfileString("Server","Noticia de conex�o de Player","[ %s ] Acabou de Logar",ConnectNoticePlayer,160,CFG_GAMESERVER);
	if(gObj[aIndex].AuthorityCode > 1)
	{
		func.MsgAllUser(0,this->Notice[7],gObj[aIndex].Name);
		gObj[aIndex].AuthorityCode = 32;
	}	
	else
	{
		func.MsgOutputAll(ConnectNoticePlayer,gObj[aIndex].Name);
	}
}

void ConnectServer::ServerType(int aIndex)
{
	this->Iniciante = GetPrivateProfileInt("ConnectControl","Ativo",0,CFG_SVINFO);
	this->MinResets = GetPrivateProfileInt("ConnectControl","MinResets",0,CFG_SVINFO);
	this->MaxResets = GetPrivateProfileInt("ConnectControl","MaxResets",0,CFG_SVINFO);
	this->ServerVip = GetPrivateProfileInt("ConnectControl","SomenteVip",0,CFG_SVINFO);
	this->Type =	  GetPrivateProfileInt("ConnectControl","VipLvl",0,CFG_SVINFO);
	this->Staff =	  GetPrivateProfileInt("ConnectControl","SomenteStaff",0,CFG_SVINFO);


	if(this->Iniciante > 0)
	{
		if(Custom[aIndex].Resets > this->MaxResets || Custom[aIndex].Resets < this->MinResets)
		{
			func.MsgUser(aIndex,1,"[%s] Servidor para [%d-%d] Resets!",gObj[aIndex].Name,this->MinResets,this->MaxResets);
			gObjCloseSet(aIndex,2);
			return;
		}
	}
	if(this->Staff > 0){
		if(gObj[aIndex].AuthorityCode < 8){
			func.MsgUser(aIndex,1,"[%s] Servidor para Staff",gObj[aIndex].Name);
			gObjCloseSet(aIndex,2);
			return;
		}
	}
	if(this->ServerVip > 0)
	{
		if(Custom[aIndex].VipIndex < this->Type)
		{
			func.MsgUser(aIndex,1,"[%s] Servidor exclusivo para vip's!",gObj[aIndex].Name);
			gObjCloseSet(aIndex,2);
			return;
		}
	}
}

void ConnectServer::InGame(int aIndex)
{
	PlayerConnect(aIndex);
	GameMasterConnect(aIndex);
	ServerType(aIndex);
	VipSystem(aIndex);
	Qest_PGW.Q_CreateStruct(aIndex);
	Qest_PGW_Boss.Q_CreateStruct(aIndex);
	Qest_PGW_Loot.Q_CreateStruct(aIndex);
	Pdiario.premiar(aIndex);
}

ConnectServer Connect;