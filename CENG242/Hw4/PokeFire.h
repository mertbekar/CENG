#ifndef POKEFIRE_H
#define POKEFIRE_H

#include "Pokemon.h"

class PokeFire : public Pokemon
{
public:
	// Add your public methods/attributes here.
	PokeFire(int pokemonID, const std::string& name);
	~PokeFire();
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
