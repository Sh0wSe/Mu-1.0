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
	Custom[aIndex].VipIndex = Manager.VipCount(gObj[aIndex].AccountID);
	Custom[aIndex].Resets = Manager.CountResets(gObj[aIndex].Name);
	Custom[aIndex].Masters = Manager.CountMasters(gObj[aIndex].Name);

	GetPrivateProfileString("Server","Noticia de conex�o","Bem vindo(a) %s",this->Notice[0],160,CFG_GAMESERVER);
	func.MsgUser(aIndex,0,this->Notice[0],gObj[aIndex].Name);
}

void ConnectServer::GameMasterConnect(int aIndex)
{
	GetPrivateProfileString("Server","Noticia de conex�o de GM/ADM","%s Online",this->Notice[1],160,CFG_GAMESERVER);

	if(gObj[aIndex].AuthorityCode > 1)
	{
		func.MsgAllUser(0,this->Notice[1],gObj[aIndex].Name);
	}
}

void ConnectServer::InGame(int aIndex)
{
	PlayerConnect(aIndex);
	GameMasterConnect(aIndex);
}

ConnectServer Connect;