#include <iostream>
#include "PokeGrass.h"
#include "Pokemon.h"

using namespace std;

PokeGrass::PokeGrass(int pokemonID, const std::string& name) : Pokemon(pokemonID, name)
{
	this->pokemonID = pokemonID;
	this->HP = 800;
	this->cHP = 800;
	this->ATK = 40;
	this->cATK = 40;
	this->MAG_DEF = 0;
	this->PHY_DEF = 30;
	this->burning = false;
	this->drowning = false;
	this->electrified = false;
	this->rooted = false;
	this->ar = 0;
}

PokeGrass::~PokeGrass()
{

}

void PokeGrass::attackTo(Pokemon* target, Arena currentArena)
{
	target->setRooted(true);
	target->takeDamage(this->ATK, this);
}

void PokeGrass::levelUp()
{
	this->HP = this->HP + 80;
	this->cHP = this->cHP + 80;
	this->ATK = this->ATK + 4;
	this->cATK = this->cATK + 4;
	this->PHY_DEF = this->PHY_DEF + 3;
}


void PokeGrass::takeDamage(int atk, Pokemon* pokemon)
{
	this->setRooted(false);
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
		if(2*(Helpers::getEffectDamage(BURNING)) - this->MAG_DEF > 0)
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"burning!!!!"<<endl;
			this->HP = this->HP - (2*(Helpers::getEffectDamage(BURNING)) - this->MAG_DEF);
		}
		else
		{
			cout<<"\t\t\t"<<this->name<<'('<<this->HP<<')'<<" "<<"is"<<" "<<"burning!!!!"<<endl;
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
}

void PokeGrass::retArena(Arena currentArena)
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
		this->setAr(0);
	}
	if(Helpers::getArenaName(currentArena) == "Forest")
	{
		this->setHP(this->getHP() + 100);
		this->setATK(this->getATK() + 10);
		this->setAr(1);
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