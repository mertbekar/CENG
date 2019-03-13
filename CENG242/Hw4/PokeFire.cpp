#include <iostream>
#include "PokeFire.h"
#include "Pokemon.h"

using namespace std;

PokeFire::PokeFire(int pokemonID, const std::string& name) : Pokemon(pokemonID, name)
{
	this->pokemonID = pokemonID;
	this->name = name;
	this->HP = 600;
	this->cHP = 600;
	this->ATK = 60;
	this->cATK = 60;
	this->MAG_DEF = 20;
	this->PHY_DEF = 10;
	this->burning = false;
	this->drowning = false;
	this->electrified = false;
	this->rooted = false;
	this->ar = 0;
}

PokeFire::~PokeFire()
{

}

void PokeFire::attackTo(Pokemon* target, Arena currentArena)
{
	target->setBurning(true);
	target->takeDamage(this->ATK, this);
}



void PokeFire::levelUp()
{
	this->HP = this->HP + 60;
	this->cHP = this->cHP + 60;
	this->ATK = this->ATK + 6;
	this->cATK = this->cATK + 6;
	this->MAG_DEF = this->MAG_DEF + 2;
	this->PHY_DEF = this->PHY_DEF + 1;
}

void PokeFire::takeDamage(int atk, Pokemon* pokemon)
{
	this->setBurning(false);
	if(atk - this->PHY_DEF > 1)
	{
		cout<<"\t\t\t"<<pokemon->getName()<<'('<<pokemon->getHP()<<')'<<" "<<"hit"<<" "<<this->name<<'('<<this->HP<<')'<<" "<<atk - this->PHY_DEF;
		if(pokemon->getAr() == 0)
		{
			cout<<"(/)"<<endl;
		}
		if(pokemon->getAr() == 1)
		{
			cout<<"(+)"<<endl;
		}
		if(pokemon->getAr() == -1)
		{
			cout<<"(-)"<<endl;
		}
		this->HP = this->HP - (atk - this->PHY_DEF);
	}
	else
		{
		cout<<"\t\t\t"<<pokemon->getName()<<'('<<pokemon->getHP()<<')'<<" "<<"hit"<<" "<<this->name<<'('<<this->HP<<')'<<" "<<1;
		if(pokemon->getAr() == 0)
		{
			cout<<"(/)"<<endl;
		}
		if(pokemon->getAr() == 1)
		{
			cout<<"(+)"<<endl;
		}
		if(pokemon->getAr() == -1)
		{
			cout<<"(-)"<<endl;
		}
		this->HP = this->HP - 1;
	}
	if(this->drowning == true && this->HP > 0)
	{
		if(2*(Helpers::getEffectDamage(DROWNING)) - this->MAG_DEF > 0)
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"drowning!!!!"<<endl;
			this->HP = this->HP - (2*(Helpers::getEffectDamage(DROWNING)) - this->MAG_DEF);
		}
		else
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"drowning!!!!"<<endl;
		}
	}
	if(this->electrified == true && this->HP > 0)
	{
		if(Helpers::getEffectDamage(ELECTRIFIED) - this->MAG_DEF)
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"electrified!!"<<endl;
			this->HP = this->HP - (Helpers::getEffectDamage(ELECTRIFIED) - this->MAG_DEF);
		}
		else
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"electrified!!"<<endl;
		}
	}
	if(this->rooted == true && this->HP > 0)
	{
		if(Helpers::getEffectDamage(ROOTED) - this->MAG_DEF > 0)
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"rooted!!"<<endl;
			this->HP = this->HP - (Helpers::getEffectDamage(ROOTED) - this->MAG_DEF);
		}
		else
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"rooted!!"<<endl;
		}
	}
}


void PokeFire::retArena(Arena currentArena)
{
	if(Helpers::getArenaName(currentArena) == "Magma")
	{
		this->setHP(this->getHP() + 100);
		this->setATK(this->getATK() + 10);
		this->setAr(1);
	}
	if(Helpers::getArenaName(currentArena) == "Ocean")
	{
		this->setHP(this->getHP() - 100);
		this->setATK(this->getATK() - 10);
		this->setAr(-1);
	}
	if(Helpers::getArenaName(currentArena) == "ElectriCity")
	{
		this->setAr(0);
	}
	if(Helpers::getArenaName(currentArena) == "Forest")
	{
		this->setAr(0);
	}
	if(Helpers::getArenaName(currentArena) == "Sky")
	{
		this->setHP(this->getHP() - 100);
		this->setATK(this->getATK() - 10);
		this->setAr(-1);
	}
	if(Helpers::getArenaName(currentArena) == "Stadium")
		this->setAr(0);
}