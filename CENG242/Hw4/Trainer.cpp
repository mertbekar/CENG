#include "Trainer.h"

using namespace std;


Trainer::Trainer(int trainerID, const std::string& name, Arena favoriteArena, std::vector<Pokemon *>& pokemons)
{
	this->trainerID = trainerID;
	this->name = name;
	this->favoriteArena = favoriteArena;
	this->pokemons = pokemons;
}

Trainer::~Trainer()
{

}

int Trainer::getTrainerID() const
{
	return this->trainerID;
}

const std::string& Trainer::getName() const
{
	return this->name;
}

Arena Trainer::getFavoriteArena() const
{
	return this->favoriteArena;
}

std::vector<Pokemon*>& Trainer::getPokemons()
{
	return this->pokemons;
}

std::vector<Pokemon*> Trainer::getPokemons2()
{
	return this->pokemons;
}