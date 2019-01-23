//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # DARK PLUGIN - POWERED BY FIRE TEAM
// # GAME SERVER: 99.60T (C) WEBZEN.
// # VERS�O: 1.0.0.0
// # Autor: Maykon
// # Skype: Maykon.ale
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # O Senhor � meu pastor e nada me faltar�!
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

#include "Database.h"

class CManager : public CDataBase
{
public:
	CManager( void );
	~CManager( void );
	bool Initialize();
	bool InitDatabase( );
	bool load();
	void OnMessage(const char * szMessage, ...);
	void OnError(const char * szMessage, ...);
	void OnQuery(const char * szMessage, ...);

	int VipCount( char* Account );
	int CountResets(char * Character);
	int CountMasters(char * Character);
	
private:
	char Host[50];
	char Database[50];
	char Usuario[50];
	char Senha[50];
};

extern CManager Manager;