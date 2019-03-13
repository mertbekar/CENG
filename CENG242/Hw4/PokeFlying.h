#ifndef POKEFLYING_H
#define POKEFLYING_H

#include "Pokemon.h"

class PokeFlying : public Pokemon
{
public:
	// Add your public methods/attributes here.
	PokeFlying(int pokemonID, const std::string& name);
	~PokeFlying();
	void levelUp();
	void attackTo(Pokemon* const pokemon, Arena currentArena);
	void takeDamage(int atk, Pokemon* pokemon);
	void retArena(Arena currentArena);


protected:
	// Add your protected methods/attributes here.

private:
	// Add your private methods/attributes here.
	
};

#endif
