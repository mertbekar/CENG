#ifndef POKEWATER_H
#define POKEWATER_H

#include "Pokemon.h"

class PokeWater : public Pokemon
{
public:
	// Add your public methods/attributes here.
	PokeWater(int pokemonID, const std::string& name);
	~PokeWater();
	void attackTo(Pokemon* const pokemon, Arena currentArena);
	void levelUp();
	void takeDamage(int atk, Pokemon* pokemon);
	void retArena(Arena currentArena);

protected:
	// Add your protected methods/attributes here.

private:
	// Add your private methods/attributes here.
	
};

#endif
