#ifndef POKEELECTRIC_H
#define POKEELECTRIC_H

#include "Pokemon.h"

class PokeElectric : public Pokemon
{
public:
	// Add your public methods/attributes here.
	PokeElectric(int pokemonID, const std::string& name);
	~PokeElectric();
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
