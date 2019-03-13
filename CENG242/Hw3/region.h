#ifndef REGION_H
#define REGION_H

#include "pokemon.h"
#include <iostream>
#include <exception>
#include <stack>

using namespace std;

class Region {
	private:
		// Add private members, methods and constructors here as you need
		struct node{
			int pokemon_coormin[3];
			int pokemon_coormax[3];
			Pokemon* pokemon;
			node *left;
			node *right;
			node *parent;
			};
		node* root;
		int mincor[3];
		int maxcor[3];
		stack<node*> poks;
		void helperxPlace(node *poketop, const Pokemon& poke, int x, int y, int z);
		Pokemon& helperOperator(node *poketop, int x, int y, int z);
		void helperCount(node *poketop, const int[3], const int[3]) const;
		/*void ilkGezici(node *planode, node *poketop) const;
		void ikinciGezici(node *planode, node *poketop) const;*/
		node* rootBulucu(node *poketop, const int[3], const int[3]) const;
		void cpy(node *poketop, node *reg) const;
		void delop(node *poketop, node *pok, int x, int y, int);
		/*void treeGezici(node *poketop) const;*/
		void delReg(node *poketop);
		void nodeBul(node *poketop);
		void silerGider(node *poketop, int [3], int [3]);
		/*void treeGezici2(Region) const;
		void silici(node *poketop);*/
	
	public:
		// Do NOT make any modifications below
		Region(const int[3], const int[3]);
		Region(const Region&);
		~Region();
		int getMinBorder(char) const;
		int getMaxBorder(char) const;
		void placePokemon(const Pokemon&, int, int, int);
		Pokemon& operator()(int, int, int);
		int getPokemonCount(const int[3], const int[3]) const;
		Region crop(const int[3], const int[3]) const;
		void patch(Region);
		Region& operator=(const Region&);
};

class pokemonException: public exception {
 	virtual const char* what() const throw() {
    		return "There does not exist any pokemon in the given coordinates!";
	}
};

#endif
