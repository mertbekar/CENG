#include <iostream>
#include "Pokemon.h"


using namespace std;

Pokemon::Pokemon(int pokemonID, const std::string& name)
{
	this->name = name;
	this->pokemonID = pokemonID;
	this->HP = 0;
	this->cHP = 0;
	this->ATK = 0;
	this->cATK = 0;
	this->MAG_DEF = 0;
	this->PHY_DEF = 0;
	this->burning = false;
	this->drowning = false;
	this->electrified = false;
	this->rooted = false;
	this->ar = 0;
}

Pokemon::~Pokemon()
{

}

int Pokemon::getPokemonID() const
{
	return this->pokemonID;
}

const string& Pokemon::getName() const
{
	return this->name;
}

int Pokemon::getHP() const
{
	return this->HP;
}

int Pokemon::getATK() const
{
	return this->ATK;
}

int Pokemon::getMAG_DEF() const
{
	return this->MAG_DEF;
}

int Pokemon::getPHY_DEF() const
{
	return this->PHY_DEF;
}

bool Pokemon::isBurning() const
{
	return this->burning;
}

bool Pokemon::isDrowning() const
{
	return this->drowning;
}

bool Pokemon::isElectrified() const
{
	return this->electrified;
}

bool Pokemon::isRooted() const
{
	return this->rooted;
}

void Pokemon::setHP(int hp)
{
	this->HP = hp;
}

void Pokemon::setATK(int atk)
{
	this->ATK = atk;
}

void Pokemon::setBurning(bool burning)
{
	this->burning = burning;
}

void Pokemon::setDrowning(bool drowning)
{
	this->drowning = drowning;
}

void Pokemon::setElectrified(bool electrified)
{
	this->electrified = electrified;
}

void Pokemon::setRooted(bool rooted)
{
	this->rooted = rooted;
}


int Pokemon::getAr()
{
	return this->ar;
}

void Pokemon::setAr(int ar)
{
	this->ar = ar;
}

void Pokemon::setcHP(int hp)
{
	this->cHP = hp;
}

void Pokemon::setcATK(int atk)
{
	this->cATK = atk;
}

int Pokemon::getcHP()
{
	return this->cHP;
}

int Pokemon::getcATK()
{
	return this->cATK;
}
