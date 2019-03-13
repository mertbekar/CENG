#include <iostream>
#include "PokeFlying.h"
#include "Pokemon.h"

using namespace std;

PokeFlying::PokeFlying(int pokemonID, const std::string& name) : Pokemon(pokemonID, name)
{
	this->pokemonID = pokemonID;
	this->HP = 650;
	this->cHP = 650;
	this->ATK = 55;
	this->cATK = 55;
	this->MAG_DEF = 0;
	this->PHY_DEF = 15;
	this->burning = false;
	this->drowning = false;
	this->electrified = false;
	this->rooted = false;
	this->ar = 0;
}

PokeFlying::~PokeFlying()
{

}

void PokeFlying::attackTo(Pokemon* target, Arena currentArena)
{
	target->takeDamage(this->ATK, this);
}

void PokeFlying::levelUp()
{
	this->HP = this->HP + 65;
	this->cHP = this->cHP + 65;
	this->ATK = this->ATK + 5;
	this->cATK = this->cATK + 5;
	this->PHY_DEF = this->PHY_DEF + 3;
}

void PokeFlying::takeDamage(int atk, Pokemon* pokemon)
{
	this->setDrowning(false);
	this->setRooted(false);
	this->setBurning(false);
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
}

void PokeFlying::retArena(Arena currentArena)
{
	if(Helpers::getArenaName(currentArena) == "Magma")
	{
		this->setAr(0);
	}
	if(Helpers::getArenaName(currentArena) == "Ocean")
	{
		this->setHP(this->getHP() - 100);
		this->setATK(this->getATK() - 10);
		this->setAr(-1);
	}
	if(Helpers::getArenaName(currentArena) == "ElectriCity")
	{
		this->setHP(this->getHP() - 100);
		this->setATK(this->getATK() - 10);
		this->setAr(-1);
	}
	if(Helpers::getArenaName(currentArena) == "Forest")
	{
		this->setAr(0);
	}
	if(Helpers::getArenaName(currentArena) == "Sky")
	{
		this->setHP(this->getHP() + 100);
		this->setATK(this->getATK() + 10);
		this->setAr(1);
	}
	if(Helpers::getArenaName(currentArena) == "Stadium")
		this->setAr(0);
}