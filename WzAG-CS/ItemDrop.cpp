#include  "StdAfx.h"

MAPDROPRATE DropItens[255];
DWORD MapContadorDrops;

void CarregarMapDropRate()
{
	ConfigMapDropRate();
	func.HookThis((DWORD)&MapDropRateMonster, 0x00404E67);
}

bool ConfigMapDropRate()
{
	Tokenizer			token;
	TokenizerGroup		Class;
	TokenizerSection	Section;

	token.ParseFile(std::string(ITEM_DROP), Class);
	if (Class.GetSection(0, Section))
	{
		MapContadorDrops = Section.RowCount;
		for (int i = 0; i < Section.RowCount; i++)
		{
			DropItens[i].Tipo = Section.Rows[i].GetInt(0);
			DropItens[i].Index = Section.Rows[i].GetInt(1);
			DropItens[i].Mapa = Section.Rows[i].GetInt(2);
			DropItens[i].Mob = Section.Rows[i].GetInt(3);
			DropItens[i].Level = Section.Rows[i].GetInt(4);
			DropItens[i].Luck = Section.Rows[i].GetInt(5);
			DropItens[i].Skill = Section.Rows[i].GetInt(6);
			DropItens[i].Opcao = Section.Rows[i].GetInt(7);
			DropItens[i].Durabilidade = Section.Rows[i].GetInt(8);
			DropItens[i].Excelente = Section.Rows[i].GetInt(9);
			DropItens[i].Ancient = Section.Rows[i].GetInt(10);
			DropItens[i].Rate = Section.Rows[i].GetInt(11);
		}
	}
	return true;
}

int MapDropRateMonster(OBJECTSTRUCT * lpMonster, OBJECTSTRUCT * lpObject)
{												
	for (int i = 0; i < MapContadorDrops; i++)
	{
		if(lpMonster->MapNumber == DropItens[i].Mapa && lpMonster->Class == DropItens[i].Mob) //ok
		{
			if ( DropItens[i].Rate == 100)
			{
				DWORD Item		= ITEMGET(DropItens[i].Tipo, DropItens[i].Index);
				DWORD Damage	= gObjMonsterTopHitDamageUser(lpMonster);
				ItemSerialCreateSend(lpMonster->m_Index,(DWORD)lpMonster->MapNumber,(DWORD)lpMonster->X, (DWORD)lpObject->Y, Item, DropItens[i].Level, (DWORD)DropItens[i].Luck, (DWORD)DropItens[i].Skill, (DWORD)DropItens[i].Durabilidade, (DWORD)DropItens[i].Opcao,Damage, (DWORD)DropItens[i].Excelente, (DWORD)DropItens[i].Ancient);	
			}
			if (rand() % 10000 < DropItens[i].Rate)
			{
				DWORD Item		= ITEMGET(DropItens[i].Tipo, DropItens[i].Index);
				DWORD Damage	= gObjMonsterTopHitDamageUser(lpMonster);
				ItemSerialCreateSend(lpMonster->m_Index,(DWORD)lpMonster->MapNumber,(DWORD)lpMonster->X, (DWORD)lpObject->Y, Item, DropItens[i].Level, (DWORD)DropItens[i].Luck, (DWORD)DropItens[i].Skill, (DWORD)DropItens[i].Durabilidade, (DWORD)DropItens[i].Opcao,Damage, (DWORD)DropItens[i].Excelente, (DWORD)DropItens[i].Ancient);	
			}
		}
		if(lpMonster->MapNumber == DropItens[i].Mapa && DropItens[i].Mob < 0 ) // ok
		{
			if (DropItens[i].Rate == 100)
			{
				DWORD Item		= ITEMGET(DropItens[i].Tipo, DropItens[i].Index);
				DWORD Damage	= gObjMonsterTopHitDamageUser(lpMonster);
				ItemSerialCreateSend(lpMonster->m_Index,(DWORD)lpMonster->MapNumber,(DWORD)lpMonster->X, (DWORD)lpObject->Y, Item, DropItens[i].Level, (DWORD)DropItens[i].Luck, (DWORD)DropItens[i].Skill, (DWORD)DropItens[i].Durabilidade, (DWORD)DropItens[i].Opcao,Damage, (DWORD)DropItens[i].Excelente, (DWORD)DropItens[i].Ancient);	
			}
			if (rand() % 10000 < DropItens[i].Rate)
			{
				DWORD Item		= ITEMGET(DropItens[i].Tipo, DropItens[i].Index);
				DWORD Damage	= gObjMonsterTopHitDamageUser(lpMonster);
				ItemSerialCreateSend(lpMonster->m_Index,(DWORD)lpMonster->MapNumber,(DWORD)lpMonster->X, (DWORD)lpObject->Y, Item, DropItens[i].Level, (DWORD)DropItens[i].Luck, (DWORD)DropItens[i].Skill, (DWORD)DropItens[i].Durabilidade, (DWORD)DropItens[i].Opcao,Damage, (DWORD)DropItens[i].Excelente, (DWORD)DropItens[i].Ancient);	
			}
		}
		if(lpMonster->Class == DropItens[i].Mob && DropItens[i].Mapa == 255 ) //ok
		{
			if (DropItens[i].Rate == 100)
			{
				DWORD Item		= ITEMGET(DropItens[i].Tipo, DropItens[i].Index);
				DWORD Damage	= gObjMonsterTopHitDamageUser(lpMonster);
				ItemSerialCreateSend(lpMonster->m_Index,(DWORD)lpMonster->MapNumber,(DWORD)lpMonster->X, (DWORD)lpObject->Y, Item, DropItens[i].Level, (DWORD)DropItens[i].Luck, (DWORD)DropItens[i].Skill, (DWORD)DropItens[i].Durabilidade, (DWORD)DropItens[i].Opcao,Damage, (DWORD)DropItens[i].Excelente, (DWORD)DropItens[i].Ancient);	
			}
			if (rand() % 10000 < DropItens[i].Rate)
			{
				DWORD Item		= ITEMGET(DropItens[i].Tipo, DropItens[i].Index);
				DWORD Damage	= gObjMonsterTopHitDamageUser(lpMonster);
			    ItemSerialCreateSend(lpMonster->m_Index,(DWORD)lpMonster->MapNumber,(DWORD)lpMonster->X, (DWORD)lpObject->Y, Item, DropItens[i].Level, (DWORD)DropItens[i].Luck, (DWORD)DropItens[i].Skill, (DWORD)DropItens[i].Durabilidade, (DWORD)DropItens[i].Opcao,Damage, (DWORD)DropItens[i].Excelente, (DWORD)DropItens[i].Ancient);	
			}
		}
		if(DropItens[i].Mapa == 255  && DropItens[i].Mob < 0 )
		{
			if (DropItens[i].Rate == 100)
			{
				DWORD Item		= ITEMGET(DropItens[i].Tipo, DropItens[i].Index);
				DWORD Damage	= gObjMonsterTopHitDamageUser(lpMonster);
				ItemSerialCreateSend(lpMonster->m_Index,(DWORD)lpMonster->MapNumber,(DWORD)lpMonster->X, (DWORD)lpObject->Y, Item, DropItens[i].Level, (DWORD)DropItens[i].Luck, (DWORD)DropItens[i].Skill, (DWORD)DropItens[i].Durabilidade, (DWORD)DropItens[i].Opcao,Damage, (DWORD)DropItens[i].Excelente, (DWORD)DropItens[i].Ancient);	
			}
			if (rand() % 10000 < DropItens[i].Rate)
			{
				DWORD Item		= ITEMGET(DropItens[i].Tipo, DropItens[i].Index);
				DWORD Damage	= gObjMonsterTopHitDamageUser(lpMonster);
				ItemSerialCreateSend(lpMonster->m_Index,(DWORD)lpMonster->MapNumber,(DWORD)lpMonster->X, (DWORD)lpObject->Y, Item, DropItens[i].Level, (DWORD)DropItens[i].Luck, (DWORD)DropItens[i].Skill, (DWORD)DropItens[i].Durabilidade, (DWORD)DropItens[i].Opcao,Damage, (DWORD)DropItens[i].Excelente, (DWORD)DropItens[i].Ancient);	
			}
		}
		
	}
	return gEventMonsterItemDrop(lpMonster, lpObject);
}