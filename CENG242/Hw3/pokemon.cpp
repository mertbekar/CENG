#include "pokemon.h"
#include <iostream>
#include <string>
using namespace std;

Pokemon::Pokemon(const string& name, const string& type, int num)
{
	this->name = name;
	this->type = type;
	this->num = num;
	this->expe = 0;
}

Pokemon::Pokemon(const Pokemon& poke)
{
	if(this!= &poke)
	{
		this->name = poke.name;
		this->type = poke.type;
		this->num = poke.num;
		this->expe = poke.expe;
	}
}

Pokemon::~Pokemon()
{
	
}

const string& Pokemon::getName() const
{
	return this->name;
}

bool Pokemon::operator>>(const Pokemon& poke)
{
	if(this->num == -1)
	{
		return false;
	}
	if(this->expe >= this->num)
	{
		this->name = poke.name;
		this->type = poke.type;
		this->num = poke.num;
		return true;
	}
	else
		return false;
}

Pokemon operator&(Pokemon& poke1, Pokemon& poke2)
{
	poke1.expe = poke1.expe + 1;
	poke2.expe = poke2.expe + 1;
	if(poke1.type == "fire" && poke2.type == "water")
	{
		return poke2;
	}
	if(poke1.type == "fire" && poke2.type == "grass")
	{
		return poke1;
	}
	if(poke1.type == "fire" && poke2.type == "electric")
	{
		return poke1;
	}
	if(poke1.type == "fire" && poke2.type == "flying")
	{
		return poke2;
	}
	if(poke1.type == "water" && poke2.type == "fire")
	{
		return poke1;
	}
	if(poke1.type == "water" && poke2.type == "grass")
	{
		return poke2;
	}
	if(poke1.type == "water" && poke2.type == "electric")
	{
		return poke2;
	}
	if(poke1.type == "water" && poke2.type == "flying")
	{
		return poke1;
	}
	if(poke1.type == "grass" && poke2.type == "fire")
	{
		return poke2;
	}
	if(poke1.type == "grass" && poke2.type == "water")
	{
		return poke1;
	}
	if(poke1.type == "grass" && poke2.type == "electric")
	{
		return poke1;
	}
	if(poke1.type == "grass" && poke2.type == "flying")
	{
		return poke2;
	}
	if(poke1.type == "electric" && poke2.type == "fire")
	{
		return poke2;
	}
	if(poke1.type == "electric" && poke2.type == "water")
	{
		return poke1;
	}
	if(poke1.type == "electric" && poke2.type == "grass")
	{
		return poke2;
	}
	if(poke1.type == "electric" && poke2.type == "flying")
	{
		return poke1;
	}
	if(poke1.type == "flying" && poke2.type == "fire")
	{
		return poke1;
	}
	if(poke1.type == "flying" && poke2.type == "water")
	{
		return poke2;
	}
	if(poke1.type == "flying" && poke2.type == "grass")
	{
		return poke1;
	}
	if(poke1.type == "flying" && poke2.type == "electric")
	{
		return poke2;
	}
}

Pokemon& Pokemon::operator=(const Pokemon& poke)
{
	if(this!=&poke)
	{
		this->name = poke.name;
		this->type = poke.type;
		this->num = poke.num;
		this->expe = poke.expe;
	}
	return *this;
}

