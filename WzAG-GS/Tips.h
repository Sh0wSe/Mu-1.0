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

struct STips
{
	char Line[60];
};

class CTips
{
public:
	CTips();
	~CTips();

public:
	bool Load();
	void Send();

private:
	bool							_Active;
	int								_Interval;
	int								_Timer;
	std::vector<STips>				_Tips;
	std::vector<STips>::iterator	_Last;
};

extern CTips Tips;