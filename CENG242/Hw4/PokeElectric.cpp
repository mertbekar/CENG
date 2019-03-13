#include <iostream>
#include "PokeElectric.h"
#include "Pokemon.h"

using namespace std;

PokeElectric::PokeElectric(int pokemonID, const std::string& name) : Pokemon(pokemonID, name)
{
	this->pokemonID = pokemonID;
	this->HP = 500;
	this->cHP = 500;
	this->ATK = 70;
	this->cATK = 70;
	this->MAG_DEF = 30;
	this->PHY_DEF = 0;
	this->burning = false;
	this->drowning = false;
	this->electrified = false;
	this->rooted = false;
	this->ar = 0;
}

PokeElectric::~PokeElectric()
{

}

void PokeElectric::attackTo(Pokemon* target, Arena currentArena)
{
	
	target->setElectrified(true);
	target->takeDamage(this->ATK, this);
}

void PokeElectric::levelUp()
{
	this->HP = this->HP + 50;
	this->cHP = this->cHP + 50;
	this->ATK = this->ATK + 7;
	this->cATK = this->cATK + 7;
	this->MAG_DEF = this->MAG_DEF + 3;
}

void PokeElectric::takeDamage(int atk, Pokemon* pokemon)
{
	this->setElectrified(false);
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
	if(this->burning == true && this->HP > 0)
	{
		if(Helpers::getEffectDamage(BURNING) - this->MAG_DEF > 0)
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"burning!!"<<endl;
			this->HP = this->HP - (Helpers::getEffectDamage(BURNING) - this->MAG_DEF);
		}
		else
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"burning!!"<<endl;
		}
	}
	if(this->drowning == true && this->HP > 0)
	{
		if(Helpers::getEffectDamage(DROWNING) - this->MAG_DEF > 0)
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"drowning!!"<<endl;
			this->HP = this->HP - (Helpers::getEffectDamage(DROWNING) - this->MAG_DEF);
		}
		else
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"drowning!!"<<endl;
		}
	}
	if(this->rooted == true && this->HP > 0)
	{
		if(2*(Helpers::getEffectDamage(ROOTED)) - this->MAG_DEF > 0)
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"rooted!!!!"<<endl;
			this->HP = this->HP - (2*(Helpers::getEffectDamage(ROOTED)) - this->MAG_DEF);
		}
		else
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"rooted!!!!"<<endl;
		}
	}
}

void PokeElectric::retArena(Arena currentArena)
{
	if(Helpers::getArenaName(currentArena) == "Magma")
	{
		this->setHP(this->getHP() - 100);
		this->setATK(this->getATK() - 10);
		this->setAr(-1);
	}
	if(Helpers::getArenaName(currentArena) == "Ocean")
	{
			this->setAr(0);
	}
	if(Helpers::getArenaName(currentArena) == "ElectriCity")
	{
		this->setHP(this->getHP() + 100);
		this->setATK(this->getATK() + 10);
		this->setAr(1);
	}
	if(Helpers::getArenaName(currentArena) == "Forest")
	{
		this->setHP(this->getHP() - 100);
		this->setATK(this->getATK() - 10);
		this->setAr(-1);
	}
	if(Helpers::getArenaName(currentArena) == "Sky")
	{
		this->setAr(0);
	}
	if(Helpers::getArenaName(currentArena) == "Stadium")
		this->setAr(0);
}