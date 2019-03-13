#ifndef POKEGRASS_H
#define POKEGRASS_H

#include "Pokemon.h"

class PokeGrass : public Pokemon
{
public:
	// Add your public methods/attributes here.
	PokeGrass(int pokemonID, const std::string& name);
	~PokeGrass();
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
