#include <iostream>
#include "PokeWater.h"
#include "Pokemon.h"

using namespace std;

PokeWater::PokeWater(int pokemonID, const std::string& name) : Pokemon(pokemonID, name)
{
	this->pokemonID = pokemonID;
	this->HP = 700;
	this->cHP = 700;
	this->ATK = 50;
	this->cATK = 50;
	this->MAG_DEF = 10;
	this->PHY_DEF = 20;
	this->burning = false;
	this->drowning = false;
	this->electrified = false;
	this->rooted = false;
	this->ar = 0;
}

PokeWater::~PokeWater()
{

}

void PokeWater::attackTo(Pokemon* target, Arena currentArena)
{
	target->setDrowning(true);
	target->takeDamage(this->ATK, this);
}

void PokeWater::levelUp()
{
	this->HP = this->HP + 70;
	this->cHP = this->cHP + 70;
	this->ATK = this->ATK + 5;
	this->cATK = this->cATK + 5;
	this->MAG_DEF = this->MAG_DEF + 1;
	this->PHY_DEF = this->PHY_DEF + 2;
}


void PokeWater::takeDamage(int atk, Pokemon* pokemon)
{
	this->setDrowning(false);
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
	
	if(this->electrified == true && this->HP > 0)
	{
		if(2*(Helpers::getEffectDamage(ELECTRIFIED)) - this->MAG_DEF > 0)
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"electrified!!!!"<<endl;
			this->HP = this->HP - (2*(Helpers::getEffectDamage(ELECTRIFIED)) - this->MAG_DEF);
		}
		else
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"electrified!!!!"<<endl;
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

void PokeWater::retArena(Arena currentArena)
{
	if(Helpers::getArenaName(currentArena) == "Magma")
	{
		this->setAr(0);
	}
	if(Helpers::getArenaName(currentArena) == "Ocean")
	{
		this->setHP(this->getHP() + 100);
		this->setATK(this->getATK() + 10);
		this->setAr(1);
	}
	if(Helpers::getArenaName(currentArena) == "ElectriCity")
	{
		this->setHP(this->getHP() - 100);
		this->setATK(this->getATK() - 10);
		this->setAr(-1);
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