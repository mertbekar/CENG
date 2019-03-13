#include "pokemon.h"
#include "region.h"
#include "trainer.h"
#include <string>
#include <vector>
#include <iostream>

Trainer::Trainer(const string& traname, const Pokemon& trapoke, const int tramin[3], const int tramax[3])
{
	Pokemon *poke = new Pokemon(trapoke);
	this->name = traname;
	this->pokemon = poke;
	pokemons.push_back(poke);
	this->mincor[0] = tramin[0];
	this->mincor[1] = tramin[1];
	this->mincor[2] = tramin[2];
	this->maxcor[0] = tramax[0];
	this->maxcor[1] = tramax[1];
	this->maxcor[2] = tramax[2];
}

Trainer::Trainer(const Trainer& tirrek)
{
	this->name = tirrek.name;
	this->pokemon= tirrek.pokemon;
	//pokemons.push_back(tirrek.pokemon);
	this->mincor[0] = tirrek.mincor[0];
	this->mincor[1] = tirrek.mincor[1];
	this->mincor[2] = tirrek.mincor[2];
	this->maxcor[0] = tirrek.maxcor[0];
	this->maxcor[1] = tirrek.maxcor[1];
	this->maxcor[2] = tirrek.maxcor[2];
	for (int i = 0; i < tirrek.pokemons.size(); ++i)
	{
		this->pokemons[i] = new Pokemon(*(tirrek.pokemons[i]));
	}
}

void Trainer::scanRegion(Region& reg)
{
	int minx = mincor[0];
	int miny = mincor[1];
	int minz = mincor[2];
	int maxx = maxcor[0];
	int maxy = maxcor[1];
	int maxz = maxcor[2];
	int x = minx;
	int y = miny;
	int z = minz;
	while(x <= maxx)
	{
		while(y <= maxy)
		{
			while(z <= maxz)
			{
				try
				{
					catchPokemon(reg(x, y, z));
					//cout<<x<<y<<z<<endl;
					z++;
				}
				catch (exception& e) 
				{
					z++;	
				}
			}
			y++;
			z = minz;
		}
		x++;
		y = miny;
		z = minz;
	}
}


Trainer::~Trainer()
{
	int i = 0;
	while(i<pokemons.size())
	{
		delete pokemons[i];
		i++;
	}
	//return so;
		//pokemons.clear();
}

ostream& showPokemons(ostream& so, const Trainer& tirrek)
{
	int i=0;
	while(i<tirrek.pokemons.size())
	{
		so<<tirrek.pokemons[i]->getName()<<endl;
		i++;
	}
	return so;
}




/*void Trainer::catchPokemon(const Pokemon& poke)
{
	this->pokemons.push_back(poke);
}*/