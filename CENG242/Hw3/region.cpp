#include "pokemon.h"
#include "region.h"
#include <iostream>
#include <exception>
#include <stack>

using namespace std;

int flagx = 2;
int flagy = 2;
int flagz = 1;
int u = 0;

Region::Region(const int minx[3], const int maxx[3])
{
	this->root = NULL;
	this->mincor[0] = minx[0];
	this->mincor[1] = minx[1];
	this->mincor[2] = minx[2];
	this->maxcor[0] = maxx[0];
	this->maxcor[1] = maxx[1];
	this->maxcor[2] = maxx[2];
}

Region::Region(const Region& reg)
{
	root = new node;
	root->right = NULL;
	root->left = NULL;
	if(reg.root->pokemon != NULL)
	{
		root->pokemon = new Pokemon(*(reg.root->pokemon));
	}
	else
		root->pokemon = NULL;
	root->pokemon_coormin[0] = reg.root->pokemon_coormin[0];
	root->pokemon_coormin[1] = reg.root->pokemon_coormin[1];
	root->pokemon_coormin[2] = reg.root->pokemon_coormin[2];
	root->pokemon_coormax[0] = reg.root->pokemon_coormax[0];
	root->pokemon_coormax[1] = reg.root->pokemon_coormax[1];
	root->pokemon_coormax[2] = reg.root->pokemon_coormax[2];
	this->mincor[0] = reg.root->pokemon_coormin[0];
	this->mincor[1] = reg.root->pokemon_coormin[1];
	this->mincor[2] = reg.root->pokemon_coormin[2];
	this->maxcor[0] = reg.root->pokemon_coormax[0];
	this->maxcor[1] = reg.root->pokemon_coormax[1];
	this->maxcor[2] = reg.root->pokemon_coormax[2];
	cpy(root, reg.root);
}

void Region::cpy(node *poketop, node *reg) const
{
	if(reg->left != NULL)
	{
		node *poketop2 = new node;
		poketop2->pokemon_coormin[0] = reg->left->pokemon_coormin[0];
		poketop2->pokemon_coormin[1] = reg->left->pokemon_coormin[1];
		poketop2->pokemon_coormin[2] = reg->left->pokemon_coormin[2];
		poketop2->pokemon_coormax[0] = reg->left->pokemon_coormax[0];
		poketop2->pokemon_coormax[1] = reg->left->pokemon_coormax[1];
		poketop2->pokemon_coormax[2] = reg->left->pokemon_coormax[2];
		poketop2->right = NULL;
		poketop2->left = NULL;
		poketop->left = poketop2;
		poketop2->parent = poketop;
		if(reg->left->pokemon != NULL)
		{
			poketop2->pokemon = new Pokemon(*(reg->left->pokemon));
		}
		else
			poketop2->pokemon = NULL;
		cpy(poketop2, reg->left);
	}
	if(reg->right != NULL)
	{
		node *poketop2 = new node;
		poketop2->pokemon_coormin[0] = reg->right->pokemon_coormin[0];
		poketop2->pokemon_coormin[1] = reg->right->pokemon_coormin[1];
		poketop2->pokemon_coormin[2] = reg->right->pokemon_coormin[2];
		poketop2->pokemon_coormax[0] = reg->right->pokemon_coormax[0];
		poketop2->pokemon_coormax[1] = reg->right->pokemon_coormax[1];
		poketop2->pokemon_coormax[2] = reg->right->pokemon_coormax[2];
		poketop2->right = NULL;
		poketop2->left = NULL;
		poketop->right = poketop2;
		poketop2->parent = poketop;
		if(reg->right->pokemon != NULL)
		{
			poketop2->pokemon = new Pokemon(*(reg->right->pokemon));
		}
		else
			poketop2->pokemon = NULL;
		cpy(poketop2, reg->right);
	}
}
Region::~Region()
{
	if(root != NULL)
	{
		delReg(root);
		delete root;
		root = NULL;
	}
}

void Region::delReg(node *poketop)
{
	if(poketop->pokemon_coormin[0] != root->pokemon_coormin[0] || poketop->pokemon_coormin[1] != root->pokemon_coormin[1] || poketop->pokemon_coormin[2] != root->pokemon_coormin[2] || poketop->pokemon_coormax[0] != root->pokemon_coormax[0] || poketop->pokemon_coormax[1] != root->pokemon_coormax[1] || poketop->pokemon_coormax[2] != root->pokemon_coormax[2])
	{
		if(poketop->left == NULL && poketop->right == NULL)
		{
			if(poketop->parent->right == poketop)
			{
				poketop->parent->right = NULL;
			}
			if(poketop->parent->left == poketop)
			{
				poketop->parent->left = NULL;
			}
			delete poketop->pokemon;
			delete poketop;
		}
	}
	if(poketop->left != NULL)
	{
		delReg(poketop->left);
		if(poketop->pokemon_coormin[0] != root->pokemon_coormin[0] || poketop->pokemon_coormin[1] != root->pokemon_coormin[1] || poketop->pokemon_coormin[2] != root->pokemon_coormin[2] || poketop->pokemon_coormax[0] != root->pokemon_coormax[0] || poketop->pokemon_coormax[1] != root->pokemon_coormax[1] || poketop->pokemon_coormax[2] != root->pokemon_coormax[2])
		{
			if(poketop->left == NULL && poketop->right == NULL)
			{
				if(poketop->parent->right == poketop)
				{
					poketop->parent->right = NULL;
				}
				if(poketop->parent->left == poketop)
				{
					poketop->parent->left = NULL;
				}
				delete poketop->pokemon;
				delete poketop;
			}
		}	
	}
	if(poketop->right != NULL)
	{
		delReg(poketop->right);
		if(poketop->pokemon_coormin[0] != root->pokemon_coormin[0] || poketop->pokemon_coormin[1] != root->pokemon_coormin[1] || poketop->pokemon_coormin[2] != root->pokemon_coormin[2] || poketop->pokemon_coormax[0] != root->pokemon_coormax[0] || poketop->pokemon_coormax[1] != root->pokemon_coormax[1] || poketop->pokemon_coormax[2] != root->pokemon_coormax[2])
		{
			if(poketop->left == NULL && poketop->right == NULL)
			{
				if(poketop->parent->right == poketop)
				{
					poketop->parent->right = NULL;
				}
				if(poketop->parent->left == poketop)
				{
					poketop->parent->left = NULL;
				}
					delete poketop->pokemon;
				delete poketop;
			}
		}
	}
}

int Region::getMinBorder(char coor) const
{
	if(coor == 'x')
	{
		return this->mincor[0];
	}
	if(coor == 'y')
	{
		return this->mincor[1];
	}
	if(coor == 'z')
	{
		return this->mincor[2];
	}
}

int Region::getMaxBorder(char coor) const
{
	if(coor == 'x')
	{
		return this->maxcor[0];
	}
	if(coor == 'y')
	{
		return this->maxcor[1];
	}
	if(coor == 'z')
	{
		return this->maxcor[2];
	}
}

void Region::placePokemon(const Pokemon& poke, int x, int y, int z)
{
	if(root == NULL)
	{
		root = new node;
		root->right = NULL;
		root->left = NULL;
		root->pokemon = NULL;
		root->parent = NULL;
		root->pokemon_coormin[0] = mincor[0];
		root->pokemon_coormin[1] = mincor[1];
		root->pokemon_coormin[2] = mincor[2];
		root->pokemon_coormax[0] = maxcor[0];
		root->pokemon_coormax[1] = maxcor[1];
		root->pokemon_coormax[2] = maxcor[2];
	}
	if(root->pokemon_coormin[0] != root->pokemon_coormax[0])
	{
		int newx = (root->pokemon_coormax[0]-root->pokemon_coormin[0])/2 + root->pokemon_coormin[0];
		if(x>newx)
		{
			if(root->right == NULL)
			{
				node *poketop = new node;
				poketop->pokemon = NULL;
				poketop->pokemon_coormin[1] = root->pokemon_coormin[1];
				poketop->pokemon_coormin[2] = root->pokemon_coormin[2];
				poketop->pokemon_coormax[0] = root->pokemon_coormax[0];
				poketop->pokemon_coormax[1] = root->pokemon_coormax[1];
				poketop->pokemon_coormax[2] = root->pokemon_coormax[2];
				poketop->pokemon_coormin[0] = newx + 1;
				poketop->left = NULL;
				poketop->right = NULL;
				root->right = poketop;
				poketop->parent = root;
				flagx = 2;
				helperxPlace(poketop, poke, x, y, z);
			}
			else
			{
				helperxPlace(root->right, poke, x, y, z);
			}
		}
		else
		{
			if(root->left == NULL)
			{
				node *poketop = new node;
				poketop->pokemon = NULL;
				poketop->pokemon_coormin[0] = root->pokemon_coormin[0];
				poketop->pokemon_coormin[1] = root->pokemon_coormin[1];
				poketop->pokemon_coormin[2] = root->pokemon_coormin[2];
				poketop->pokemon_coormax[1] = root->pokemon_coormax[1];
				poketop->pokemon_coormax[2] = root->pokemon_coormax[2];
				poketop->pokemon_coormax[0] = newx;
				poketop->left = NULL;
				poketop->right = NULL;
				root->left = poketop;
				poketop->parent = root;
				flagx = 2;
				helperxPlace(poketop, poke, x, y, z);
			}
			else
			{
				helperxPlace(root->left, poke, x, y, z);
			}
		}
	}
	if(root->pokemon_coormin[0] == root->pokemon_coormax[0])
	{
		if(root->pokemon_coormin[1] != root->pokemon_coormax[1])
		{
			int newy = (root->pokemon_coormax[1]-root->pokemon_coormin[1])/2 + root->pokemon_coormin[1];
			if(y>newy)
			{
				if(root->right == NULL)
				{
					node *poketop = new node;
					poketop->pokemon = NULL;
					poketop->pokemon_coormin[0] = x;
					poketop->pokemon_coormin[2] = root->pokemon_coormin[2];
					poketop->pokemon_coormax[0] = x;
					poketop->pokemon_coormax[1] = root->pokemon_coormax[1];
					poketop->pokemon_coormax[2] = root->pokemon_coormax[2];
					poketop->pokemon_coormin[1] = newy + 1;
					poketop->left = NULL;
					poketop->right = NULL;
					root->right = poketop;
					poketop->parent = root;
					flagx = 3;
					helperxPlace(poketop, poke, x, y, z);
				}
				else
				{
					helperxPlace(root->right, poke, x, y, z);
				}
			}	
			else
			{
				if(root->left == NULL)
				{
					node *poketop = new node;
					poketop->pokemon = NULL;
					poketop->pokemon_coormin[0] = x;
					poketop->pokemon_coormin[1] = root->pokemon_coormin[1];
					poketop->pokemon_coormin[2] = root->pokemon_coormin[2];
					poketop->pokemon_coormax[0] = x;
					poketop->pokemon_coormax[2] = root->pokemon_coormax[2];
					poketop->pokemon_coormax[1] = newy;
					poketop->left = NULL;
					poketop->right = NULL;
					root->left = poketop;
					poketop->parent = root;
					flagx = 3;
					helperxPlace(poketop, poke, x, y, z);
				}
				else
				{
					helperxPlace(root->left, poke, x, y, z);
				}
			}
		}
		if(root->pokemon_coormin[1] == root->pokemon_coormax[1])
		{
			if(root->pokemon_coormin[2] != root->pokemon_coormax[2])
			{
				int newz = (root->pokemon_coormax[2]-root->pokemon_coormin[2])/2 + root->pokemon_coormin[2];
				if(z>newz)
				{
					if(root->right == NULL)
					{
						node *poketop = new node;
						poketop->pokemon = NULL;
						poketop->pokemon_coormin[0] = x;
						poketop->pokemon_coormin[1] = root->pokemon_coormin[1];
						poketop->pokemon_coormax[0] = x;
						poketop->pokemon_coormax[1] = root->pokemon_coormax[1];
						poketop->pokemon_coormax[2] = root->pokemon_coormax[2];
						poketop->pokemon_coormin[2] = newz + 1;
						poketop->left = NULL;
						poketop->right = NULL;
						root->right = poketop;
						poketop->parent = root;
						flagx = 1;
						helperxPlace(poketop, poke, x, y, z);
					}
					else
					{
						root->right->parent = root;
						helperxPlace(root->right, poke, x, y, z);
					}
				}
				else
				{
					if(root->left == NULL)
					{
						node *poketop = new node;
						poketop->pokemon = NULL;
						poketop->pokemon_coormin[0] = x;
						poketop->pokemon_coormin[1] = y;
						poketop->pokemon_coormin[2] = root->pokemon_coormin[2];
						poketop->pokemon_coormax[0] = x;
						poketop->pokemon_coormax[1] = y;
						poketop->pokemon_coormax[2] = newz;
						poketop->left = NULL;
						poketop->right = NULL;
						root->left = poketop;
						poketop->parent = root;
						flagx = 1;
						helperxPlace(poketop, poke, x, y, z);
					}
					else
					{
						helperxPlace(root->left, poke, x, y, z);
					}
				}
			}
			if(root->pokemon_coormin[2] == root->pokemon_coormax[2])
			{
				root->pokemon = new Pokemon(poke);
			}
		}
	}
}

Pokemon& Region::operator()(int x, int y, int z)
{
	int newx = (root->pokemon_coormax[0] - root->pokemon_coormin[0])/2 + root->pokemon_coormin[0];
	if(x > newx)
	{
		if(root->right != NULL)
		{
			flagy=2;
			return helperOperator(root->right, x, y, z);
		}
	}
	if(x <= newx && (root->pokemon_coormax[0] != root->pokemon_coormin[0]))
	{
		if(root->left != NULL)
		{
			flagy=2;
			return helperOperator(root->left, x, y, z);
		}
	}
	if(x == newx && (root->pokemon_coormax[0] == root->pokemon_coormin[0]))
	{
		int newy = (root->pokemon_coormax[1] - root->pokemon_coormin[1])/2 + root->pokemon_coormin[1];
		if(y > newy && (root->pokemon_coormax[1] != root->pokemon_coormin[1]))
		{
			if(root->right != NULL)
			{
				flagy=3;
				return helperOperator(root->right, x, y, z);
			}
		}
		if(y <= newy && (root->pokemon_coormax[1] != root->pokemon_coormin[1]))
		{
			if(root->left != NULL)
			{
				flagy=3;
				return helperOperator(root->left, x, y, z);
			}
		}
		if(y == newy && (root->pokemon_coormax[1] == root->pokemon_coormin[1]))
		{
			int newz = (root->pokemon_coormax[2] - root->pokemon_coormin[2])/2 + root->pokemon_coormin[2];
			if(z > newz)
			{
				if(root->right != NULL)
				{
					flagy=1;
					return helperOperator(root->right, x, y, z);
				}
			}
			if(z <= newz && (root->pokemon_coormax[2] != root->pokemon_coormin[2]))
			{
				if(root->left != NULL)
				{
					flagy=1;
					return helperOperator(root->left, x, y, z);
				}
			}
			if(z == newz && (root->pokemon_coormax[2] == root->pokemon_coormin[2]))
			{
				return *(root->pokemon);
			}
		}
	}
	else
	{
		throw pokemonException();
	}
}

void Region::delop(node *poketop, node *pok, int x, int y, int z)
{
	if(poketop == pok)
	{
		flagz = 1;
		if(poketop->left == NULL && poketop->right == NULL)
		{
			if(poketop->parent->right == poketop)
			{
				poketop->parent->right = NULL;
			}
			if(poketop->parent->left == poketop)
			{
				poketop->parent->left = NULL;
			}
			delete poketop->pokemon;
			delete poketop;
		}
		
	}
	if(flagz == 1)
	{
		int newx = (poketop->pokemon_coormax[0] - poketop->pokemon_coormin[0])/2 + poketop->pokemon_coormin[0];
		if(poketop->pokemon_coormin[0] != poketop->pokemon_coormax[0])
		{
			if(x > newx)
			{
				if(poketop->right != NULL)
				{
					flagz = 2;
					delop(poketop->right, pok, x, y, z);
					if(poketop->left == NULL && poketop->right == NULL)
					{
						if(poketop == root)
						{
							delete root->pokemon;
							delete root;
							this->root = NULL;
						}
						else if(poketop->parent->right == poketop)
						{	
							poketop->parent->right = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
						else if(poketop->parent->left == poketop)
						{
							poketop->parent->left = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
						//delete poketop;
					}
				}
			}
			if(x < newx)
			{
				if(poketop->left != NULL)
				{
					flagz = 2;
					delop(poketop->left, pok, x, y, z);
					if(poketop->left == NULL && poketop->right == NULL)
					{
						if(poketop == root)
						{
							delete root->pokemon;
							delete root;
							this->root = NULL;
						}
						else if(poketop->parent->right == poketop)
						{
							poketop->parent->right = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
						else if(poketop->parent->left == poketop)
						{
							poketop->parent->left = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
					}
				}
			}
			if((x == newx) && (poketop->pokemon_coormin[0] != poketop->pokemon_coormax[0]))
			{
				if(poketop->left != NULL)
				{
					flagz = 2;
					delop(poketop->left, pok, x, y, z);
					if(poketop->left == NULL && poketop->right == NULL)
					{
						if(poketop == root)
						{
							delete root->pokemon;
							delete root;
							this->root = NULL;
						}
						else if(poketop->parent->right == poketop)
						{
							poketop->parent->right = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
						else if(poketop->parent->left == poketop)
						{
							poketop->parent->left = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
					}
				}
			}
		}
		if(poketop->pokemon_coormin[0] == poketop->pokemon_coormax[0])
		{
			flagz = 2;
			delop(poketop, pok, x, y, z);
		}
	}
	if(flagz == 2)
	{
		if(poketop->pokemon_coormin[1] != poketop->pokemon_coormax[1])
		{
			int newy = (poketop->pokemon_coormax[1] - poketop->pokemon_coormin[1])/2 + poketop->pokemon_coormin[1];
			if(y > newy)
			{
				if(poketop->right != NULL)
				{
					flagz = 3;
					delop(poketop->right, pok, x, y, z);
					if(poketop->left == NULL && poketop->right == NULL)
					{
						if(poketop == root)
						{
							delete root->pokemon;
							delete root;
							root = NULL;
						}
						else if(poketop->parent->right == poketop)
						{
							poketop->parent->right = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
						else if(poketop->parent->left == poketop)
						{
							poketop->parent->left = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
					}
				}
			}
			if(y < newy)
			{
				if(poketop->left != NULL)
				{
					flagz = 3;
					delop(poketop->left, pok, x, y, z);
					if(poketop->left == NULL && poketop->right == NULL)
					{
						if(poketop == root)
						{
							delete root->pokemon;
							delete root;
							root = NULL;
						}
						else if(poketop->parent->right == poketop)
						{
							poketop->parent->right = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
						else if(poketop->parent->left == poketop)
						{
							poketop->parent->left = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
					}
				}
			}
			if((y == newy) && (poketop->pokemon_coormin[1] != poketop->pokemon_coormax[1]))
			{
				if(poketop->left != NULL)
				{
					flagz = 3;
					delop(poketop->left, pok, x, y, z);
					if(poketop->left == NULL && poketop->right == NULL)
					{
						if(poketop == root)
						{
							delete root->pokemon;
							delete root;
							root = NULL;
						}
						else if(poketop->parent->right == poketop)
						{
							poketop->parent->right = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
						else if(poketop->parent->left == poketop)
						{
							poketop->parent->left = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
					}
				}
			}
		}
		if(poketop->pokemon_coormin[1] == poketop->pokemon_coormax[1])
		{
			flagz = 3;
			delop(poketop, pok, x, y, z);
		}
	}
	if(flagz == 3)
	{
		if(poketop->pokemon_coormin[2] != poketop->pokemon_coormax[2])
		{
			int newz = (poketop->pokemon_coormax[2] - poketop->pokemon_coormin[2])/2 + poketop->pokemon_coormin[2];
			if(z > newz)
			{
				if(poketop->right != NULL)
				{
					flagz = 1;
					delop(poketop->right, pok, x, y, z);
					if(poketop->left == NULL && poketop->right == NULL)
					{
						if(poketop == root)
						{
							delete root->pokemon;
							delete root;
							root = NULL;
						}
						else if(poketop->parent->right == poketop)
						{
							poketop->parent->right = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
						else if(poketop->parent->left == poketop)
						{
							poketop->parent->left = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
					}
				}
			}
			if(z < newz)
			{
				if(poketop->left != NULL)
				{
					flagz = 1;
					delop(poketop->left, pok, x, y, z);
					if(poketop->left == NULL && poketop->right == NULL)
					{
						if(poketop == root)
						{
							delete root->pokemon;
							delete root;
							root = NULL;
						}
						else if(poketop->parent->right == poketop)
						{
							poketop->parent->right = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
						else if(poketop->parent->left == poketop)
						{
							poketop->parent->left = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
					}
				}
			}
			if((z == newz) && (poketop->pokemon_coormin[2] != poketop->pokemon_coormax[2]))
			{
				if(poketop->left != NULL)
				{
					flagz = 1;
					delop(poketop->left, pok, x, y, z);
					if(poketop->left == NULL && poketop->right == NULL)
					{
						if(poketop == root)
						{
							delete root->pokemon;
							delete root;
							root = NULL;
						}
						if(poketop->parent->right == poketop)
						{
							poketop->parent->right = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
						if(poketop->parent->left == poketop)
						{
							poketop->parent->left = NULL;
							delete poketop->pokemon;
							delete poketop;
						}
					}
				}
			}
		}
		if(poketop->pokemon_coormin[2] == poketop->pokemon_coormax[2])
		{
			flagz = 1;
			delop(poketop, pok, x, y, z);
		}
	}
}

Pokemon& Region::helperOperator(node *poketop, int x, int y, int z)
{
	if(poketop->pokemon_coormin[0] == poketop->pokemon_coormax[0] && poketop->pokemon_coormin[1] == poketop->pokemon_coormax[1] && poketop->pokemon_coormin[2] == poketop->pokemon_coormax[2])
	{
		flagy = 2;
		Pokemon *tmp = poketop->pokemon;
		node *pp = poketop->parent;
		if(pp->left == poketop)
		{
			pp->left = NULL;
		}
		if(pp->right == poketop)
		{
			pp->right = NULL;
		}
		delete poketop;
		if(pp != NULL)
		{
			delop(root, pp, x, y, z);
		}
		return *tmp;
	}
	if(flagy == 1)
	{
		int newx = (poketop->pokemon_coormax[0] - poketop->pokemon_coormin[0])/2 + poketop->pokemon_coormin[0];
		if(poketop->pokemon_coormin[0] != poketop->pokemon_coormax[0])
		{
			if(x > newx)
			{
				if(poketop->right != NULL)
				{
					flagy = 2;
					return helperOperator(poketop->right, x, y, z);
				}
				else
				{
					throw pokemonException();
				}
			}
			if(x < newx)
			{
				if(poketop->left != NULL)
				{
					flagy = 2;
					return helperOperator(poketop->left, x, y, z);
				}
				else
				{
					throw pokemonException();
				}
			}
			if((x == newx) && (poketop->pokemon_coormin[0] != poketop->pokemon_coormax[0]))
			{
				if(poketop->left != NULL)
				{
					flagy = 2;
					return helperOperator(poketop->left, x, y, z);
				}
				else
				{
					throw pokemonException();
				}
			}
		}
		if(poketop->pokemon_coormin[0] == poketop->pokemon_coormax[0])
		{
			flagy = 2;
			return helperOperator(poketop, x, y, z);
		}
	}
	if(flagy == 2)
	{
		if(poketop->pokemon_coormin[1] != poketop->pokemon_coormax[1])
		{
			int newy = (poketop->pokemon_coormax[1] - poketop->pokemon_coormin[1])/2 + poketop->pokemon_coormin[1];
			if(y > newy)
			{
				if(poketop->right != NULL)
				{
					flagy = 3;
					return helperOperator(poketop->right, x, y, z);
				}
				else
				{
					flagy = 3;
					throw pokemonException();
				}
			}
			if(y < newy)
			{
				if(poketop->left != NULL)
				{
					flagy = 3;
					return helperOperator(poketop->left, x, y, z);
				}
				else
				{
					flagy = 3;
					throw pokemonException();
				}
			}
			if((y == newy) && (poketop->pokemon_coormin[1] != poketop->pokemon_coormax[1]))
			{
				if(poketop->left != NULL)
				{
					flagy = 3;
					return helperOperator(poketop->left, x, y, z);
				}
				else
				{
					flagy = 3;
					throw pokemonException();
				}
			}
		}
		if(poketop->pokemon_coormin[1] == poketop->pokemon_coormax[1])
		{
			flagy = 3;
			return helperOperator(poketop, x, y, z);
		}
	}
	if(flagy == 3)
	{
		if(poketop->pokemon_coormin[2] != poketop->pokemon_coormax[2])
		{
			int newz = (poketop->pokemon_coormax[2] - poketop->pokemon_coormin[2])/2 + poketop->pokemon_coormin[2];
			if(z > newz)
			{
				if(poketop->right != NULL)
				{
					flagy = 1;
					return helperOperator(poketop->right, x, y, z);
				}
				else
				{
					flagy = 1;
					throw pokemonException();
				}
			}
			if(z < newz)
			{
				if(poketop->left != NULL)
				{
					flagy = 1;
					return helperOperator(poketop->left, x, y, z);
				}
				else
				{
					flagy = 1;
					throw pokemonException();
				}
			}
			if((z == newz) && (poketop->pokemon_coormin[2] != poketop->pokemon_coormax[2]))
			{
				if(poketop->left != NULL)
				{
					flagy = 1;
					return helperOperator(poketop->left, x, y, z);
				}
				else
				{
					flagy = 1;
					throw pokemonException();
				}
			}
			if((z == newz) && (poketop->pokemon_coormin[2] != poketop->pokemon_coormax[2]) && (z <= poketop->pokemon_coormax[2]))
			{
				if(poketop->left != NULL)
				{
					flagy = 1;
					return helperOperator(poketop->left, x, y, z);
				}
				else
				{
					flagy = 1;
					throw pokemonException();
				}
			}
		}
		if(poketop->pokemon_coormin[2] == poketop->pokemon_coormax[2])
		{
			flagy = 1;
			return helperOperator(poketop, x, y, z);
		}
	}
	else
	{
		flagy = 2;
		throw pokemonException();
	}
}


void Region::helperxPlace(node *poketop, const Pokemon& poke, int x, int y, int z)
{
	if(poketop->pokemon_coormin[0] == poketop->pokemon_coormax[0] && poketop->pokemon_coormin[1] == poketop->pokemon_coormax[1] && poketop->pokemon_coormin[2] == poketop->pokemon_coormax[2])
	{
		/*poketop->left = NULL;
		poketop->right = NULL;*/
		poketop->pokemon = new Pokemon(poke);
		flagx = 2;
		return;
	}
	if(flagx == 1)
	{
		int newx = (poketop->pokemon_coormax[0] - poketop->pokemon_coormin[0])/2 + poketop->pokemon_coormin[0];
		if(poketop->pokemon_coormin[0] != poketop->pokemon_coormax[0])
		{
			if(x>newx)
			{
				if(poketop->right == NULL)
				{
					node *poketop2 = new node;
					poketop2->pokemon_coormin[0] = newx + 1;
					poketop2->pokemon_coormin[1] = poketop->pokemon_coormin[1];
					poketop2->pokemon_coormin[2] = poketop->pokemon_coormin[2];
					poketop2->pokemon_coormax[0] = poketop->pokemon_coormax[0];
					poketop2->pokemon_coormax[1] = poketop->pokemon_coormax[1];
					poketop2->pokemon_coormax[2] = poketop->pokemon_coormax[2];
					poketop2->pokemon = NULL;
					poketop2->left = NULL;
					poketop2->right = NULL;
					poketop2->parent = poketop;
					poketop->right = poketop2;
					flagx = 2;
					return helperxPlace(poketop2, poke, x, y, z);
				}
				else
				{
					flagx = 2;
					return helperxPlace(poketop->right, poke, x, y, z);
				}
			}
			else
			{
				if(poketop->left == NULL)
				{
					node *poketop3 = new node;
					poketop3->pokemon_coormin[0] = poketop->pokemon_coormin[0];
					poketop3->pokemon_coormin[1] = poketop->pokemon_coormin[1];
					poketop3->pokemon_coormin[2] = poketop->pokemon_coormin[2];
					poketop3->pokemon_coormax[0] = newx;
					poketop3->pokemon_coormax[1] = poketop->pokemon_coormax[1];
					poketop3->pokemon_coormax[2] = poketop->pokemon_coormax[2];
					poketop3->pokemon = NULL;
					poketop3->left = NULL;
					poketop3->right = NULL;
					poketop3->parent = poketop;
					poketop->left = poketop3;
					flagx = 2;
					return helperxPlace(poketop3, poke, x, y, z);
				}
				else
				{
					flagx = 2;
					return helperxPlace(poketop->left, poke, x, y, z);
				}
			}
		}
		if(poketop->pokemon_coormin[0] == poketop->pokemon_coormax[0])
		{
			flagx = 2;
			return helperxPlace(poketop, poke, x, y, z);
		}
	}
	if(flagx == 2)
	{
		if(poketop->pokemon_coormin[1] != poketop->pokemon_coormax[1])
		{
			int newy = (poketop->pokemon_coormax[1] - poketop->pokemon_coormin[1])/2 + poketop->pokemon_coormin[1];
			//node *poketop2 = new node;
			if(y>newy)
			{
				if(poketop->right == NULL)
				{
					node *poketop4 = new node;
					poketop4->pokemon_coormin[0] = poketop->pokemon_coormin[0];
					poketop4->pokemon_coormin[1] = newy + 1;
					poketop4->pokemon_coormin[2] = poketop->pokemon_coormin[2];
					poketop4->pokemon_coormax[0] = poketop->pokemon_coormax[0];
					poketop4->pokemon_coormax[1] = poketop->pokemon_coormax[1];
					poketop4->pokemon_coormax[2] = poketop->pokemon_coormax[2];
					poketop4->pokemon = NULL;
					poketop4->left = NULL;
					poketop4->right = NULL;
					poketop4->parent = poketop;
					poketop->right = poketop4;
					flagx = 3;
					return helperxPlace(poketop4, poke, x, y, z);
				}
				else
				{
					flagx = 3;
					return helperxPlace(poketop->right, poke, x, y, z);
				}
			}	
			else
			{
				if(poketop->left == NULL)
				{
					node *poketop5 = new node;
					poketop5->left = NULL;
					poketop5->right = NULL;
					poketop5->pokemon = NULL;
					poketop5->pokemon_coormin[0] = poketop->pokemon_coormin[0];;
					poketop5->pokemon_coormin[1] = poketop->pokemon_coormin[1];
					poketop5->pokemon_coormin[2] = poketop->pokemon_coormin[2];
					poketop5->pokemon_coormax[0] = poketop->pokemon_coormax[0];
					poketop5->pokemon_coormax[2] = poketop->pokemon_coormax[2];
					poketop5->pokemon_coormax[1] = newy;
					poketop->left = poketop5;
					poketop5->parent = poketop;
					flagx = 3;
					return helperxPlace(poketop5, poke, x, y, z);
				}
				else
				{
					flagx = 3;
					return helperxPlace(poketop->left, poke, x, y, z);
				}
			}
		}
		if(poketop->pokemon_coormin[1] == poketop->pokemon_coormax[1])
		{
			flagx = 3;
			return helperxPlace(poketop, poke, x, y, z);
		}
	}
	if(flagx == 3)
	{
		if(poketop->pokemon_coormin[2] != poketop->pokemon_coormax[2])
		{
			int newz = (poketop->pokemon_coormax[2] - poketop->pokemon_coormin[2])/2 + poketop->pokemon_coormin[2];
			if(z>newz)
			{
				if(poketop->right == NULL)
				{
					node *poketop6 = new node;
					poketop6->left = NULL;
					poketop6->right = NULL;
					poketop6->pokemon = NULL;
					poketop6->pokemon_coormin[0] = poketop->pokemon_coormin[0];
					poketop6->pokemon_coormin[1] = poketop->pokemon_coormin[1];
					poketop6->pokemon_coormax[0] = poketop->pokemon_coormax[0];
					poketop6->pokemon_coormax[1] = poketop->pokemon_coormax[1];
					poketop6->pokemon_coormax[2] = poketop->pokemon_coormax[2];
					poketop6->pokemon_coormin[2] = newz + 1;
					poketop->right = poketop6;
					poketop6->parent = poketop;
					flagx = 1;
					return helperxPlace(poketop6, poke, x, y, z);
				}
				else
				{
					flagx = 1;
					return helperxPlace(poketop->right, poke, x, y, z);
				}
			}
			else
			{
				if(poketop->left == NULL)
				{
					node *poketop7 = new node;
					poketop7->left = NULL;
					poketop7->right = NULL;
					poketop7->pokemon = NULL;
					poketop7->pokemon_coormin[0] = poketop->pokemon_coormin[0];
					poketop7->pokemon_coormin[1] = poketop->pokemon_coormin[1];
					poketop7->pokemon_coormin[2] = poketop->pokemon_coormin[2];
					poketop7->pokemon_coormax[0] = poketop->pokemon_coormax[0];
					poketop7->pokemon_coormax[1] = poketop->pokemon_coormax[1];
					poketop7->pokemon_coormax[2] = newz;
					poketop->left = poketop7;
					poketop7->parent = poketop;
					flagx = 1;
					return helperxPlace(poketop->left, poke, x, y, z);
				}
				else
				{
					flagx = 1;
					return helperxPlace(poketop->left, poke, x, y, z);
				}
			}
		}
		if(poketop->pokemon_coormin[2] == poketop->pokemon_coormax[2])
		{
			flagx = 1;
			return helperxPlace(poketop, poke, x, y, z);
		}
	}
}

int Region::getPokemonCount(const int a[3], const int b[3]) const
{
	int c = 0;
	helperCount(root, a, b);
	c = u;
	u = 0;
	return c;
}


void Region::helperCount(node *poketop, const int a[3], const int b[3]) const
{
	if(poketop->pokemon != NULL)
	{
		if(poketop->pokemon_coormin[0] >= a[0] && poketop->pokemon_coormin[1] >= a[1] && poketop->pokemon_coormin[2] >= a[2])
		{
			if(poketop->pokemon_coormax[0] <= b[0] && poketop->pokemon_coormax[1] <= b[1] && poketop->pokemon_coormax[2] <= b[2])
			{
				u++;
				return;
			}
		}
	}
	else if(poketop->left != NULL)
	{
		helperCount(poketop->left, a, b);
	}
	if(poketop->right != NULL)
	{
		helperCount(poketop->right, a, b);
	}
}

Region Region::crop(const int minx[3], const int maxx[3]) const
{
	Region plane = Region(minx, maxx);
	plane.root = rootBulucu(root, minx, maxx);
	cpy(plane.root, root);
	return plane;
}


/*void Region::ilkGezici(node *planode, node *poketop) const
{
	while(1)
	{
		if((poketop->pokemon_coormin[0] == planode->pokemon_coormin[0]) && (poketop->pokemon_coormin[1] == planode->pokemon_coormin[1]) && (poketop->pokemon_coormin[2] == planode->pokemon_coormin[2]))
		{
			if((poketop->pokemon_coormax[0] == planode->pokemon_coormax[0]) && (poketop->pokemon_coormax[1] == planode->pokemon_coormax[1]) && (poketop->pokemon_coormax[2] == planode->pokemon_coormax[2]))
			{
				break;
			}
		}
		if(poketop->left != NULL)
		{
			if((poketop->left->pokemon_coormin[0] <= planode->pokemon_coormin[0]) && (poketop->left->pokemon_coormin[1] <= planode->pokemon_coormin[1]) && (poketop->left->pokemon_coormin[2] <= planode->pokemon_coormin[2]))
			{
				if((poketop->left->pokemon_coormax[0] >= planode->pokemon_coormax[0]) && (poketop->left->pokemon_coormax[1] >= planode->pokemon_coormax[1]) && (poketop->left->pokemon_coormax[2] >= planode->pokemon_coormax[2]))
				{
					poketop = poketop->left;
				}
			}
		}
		if(poketop->right != NULL)
		{
			if((poketop->right->pokemon_coormin[0] <= planode->pokemon_coormin[0]) && (poketop->right->pokemon_coormin[1] <= planode->pokemon_coormin[1]) && (poketop->right->pokemon_coormin[2] <= planode->pokemon_coormin[2]))
			{
				if((poketop->right->pokemon_coormax[0] >= planode->pokemon_coormax[0]) && (poketop->right->pokemon_coormax[1] >= planode->pokemon_coormax[1]) && (poketop->right->pokemon_coormax[2] >= planode->pokemon_coormax[2]))
				{
					poketop = poketop->right;
				}
			}
		}
	}
		planode = poketop;
}

void Region::ikinciGezici(node *planode, node *poketop) const
{
	if(poketop->left != NULL)
	{
		node *planode2 = new node;
		planode2->pokemon_coormin[0] = poketop->left->pokemon_coormin[0];
		planode2->pokemon_coormin[1] = poketop->left->pokemon_coormin[1];
		planode2->pokemon_coormin[2] = poketop->left->pokemon_coormin[2];
		planode2->pokemon_coormax[0] = poketop->left->pokemon_coormax[0];
		planode2->pokemon_coormax[1] = poketop->left->pokemon_coormax[1];
		planode2->pokemon_coormax[2] = poketop->left->pokemon_coormax[2];
		planode2->pokemon = poketop->left->pokemon;
		planode2->right = NULL;
		planode2->left = NULL;
		planode->left = planode2;
		ikinciGezici(planode2, poketop->left);
	}
	if(poketop->right != NULL)
	{
		node *planode2 = new node;
		planode2->pokemon_coormin[0] = poketop->right->pokemon_coormin[0];
		planode2->pokemon_coormin[1] = poketop->right->pokemon_coormin[1];
		planode2->pokemon_coormin[2] = poketop->right->pokemon_coormin[2];
		planode2->pokemon_coormax[0] = poketop->right->pokemon_coormax[0];
		planode2->pokemon_coormax[1] = poketop->right->pokemon_coormax[1];
		planode2->pokemon_coormax[2] = poketop->right->pokemon_coormax[2];
		planode2->pokemon = poketop->right->pokemon;
		planode2->right = NULL;
		planode2->left = NULL;
		planode->right = planode2;
		ikinciGezici(planode2, poketop->right);
	}
}
*/

typename Region::node* Region::rootBulucu(node *poketop, const int minx[3], const int maxx[3]) const
{
	if((poketop->pokemon_coormin[0] == minx[0]) && (poketop->pokemon_coormin[1] == minx[1]) && (poketop->pokemon_coormin[2] == minx[2]))
	{
		if((poketop->pokemon_coormax[0] == maxx[0]) && (poketop->pokemon_coormax[1] == maxx[1]) && (poketop->pokemon_coormax[2] == maxx[2]))
		{
			/*cout<<"ok";*/
			node *planode = new node;
			planode->pokemon_coormin[0] = poketop->pokemon_coormin[0];
			planode->pokemon_coormin[1] = poketop->pokemon_coormin[1];
			planode->pokemon_coormin[2] = poketop->pokemon_coormin[2];
			planode->pokemon_coormax[0] = poketop->pokemon_coormax[0];
			planode->pokemon_coormax[1] = poketop->pokemon_coormax[1];
			planode->pokemon_coormax[2] = poketop->pokemon_coormax[2];
			planode->pokemon = poketop->pokemon;
			planode->right = NULL;
			planode->left = NULL;
			return planode;
		}
	}
	if(poketop->left != NULL)
	{
		if((poketop->left->pokemon_coormin[0] <= minx[0]) && (poketop->left->pokemon_coormin[1] <= minx[1]) && (poketop->left->pokemon_coormin[2] <= minx[2]))
		{
			if((poketop->left->pokemon_coormax[0] >= maxx[0]) && (poketop->left->pokemon_coormax[1] >= maxx[1]) && (poketop->left->pokemon_coormax[2] >= maxx[2]))
			{
				return rootBulucu(poketop->left, minx, maxx);
			}
		}
	}
	if(poketop->right != NULL)
	{
		if((poketop->right->pokemon_coormin[0] <= minx[0]) && (poketop->right->pokemon_coormin[1] <= minx[1]) && (poketop->right->pokemon_coormin[2] <= minx[2]))
		{
			if((poketop->right->pokemon_coormax[0] >= maxx[0]) && (poketop->right->pokemon_coormax[1] >= maxx[1]) && (poketop->right->pokemon_coormax[2] >= maxx[2]))
			{
				return rootBulucu(poketop->right, minx, maxx);
			}
		}
	}
}

/*void Region::treeGezici2(Region reg) const
{
	treeGezici(reg.root);
}

void Region::treeGezici(node *poketop) const
{
	//delReg(root);
	cout<<poketop->pokemon_coormin[0];
		cout<<poketop->pokemon_coormin[1];
		cout<<poketop->pokemon_coormin[2]<<endl;
		cout<<poketop->pokemon_coormax[0];
		cout<<poketop->pokemon_coormax[1];
		cout<<poketop->pokemon_coormax[2]<<endl;
		int mix = this->maxcor[0];
		int miy = this->maxcor[1];
		int miz = this->maxcor[2];
		int mx = 0;
		int my = 0;
		int mz = 0;
	if(poketop->left != NULL)
	{
		treeGezici(poketop->left);
	}
	if(poketop->right != NULL)
	{
		treeGezici(poketop->right);
	}
}*/

Region& Region::operator=(const Region& reg)
{
	delReg(root);
	delete root;
	root = new node;
	root->pokemon_coormin[0] = reg.root->pokemon_coormin[0];
	root->pokemon_coormin[1] = reg.root->pokemon_coormin[1];
	root->pokemon_coormin[2] = reg.root->pokemon_coormin[2];
	root->pokemon_coormax[0] = reg.root->pokemon_coormax[0];
	root->pokemon_coormax[1] = reg.root->pokemon_coormax[1];
	root->pokemon_coormax[2] = reg.root->pokemon_coormax[2];
	root->right = NULL;
	root->left = NULL;
	if(reg.root->pokemon != NULL)
	{
		root->pokemon = new Pokemon(*(reg.root->pokemon));
	}
	else
		root->pokemon = NULL;
	this->mincor[0] = reg.root->pokemon_coormin[0];
	this->mincor[1] = reg.root->pokemon_coormin[1];
	this->mincor[2] = reg.root->pokemon_coormin[2];
	this->maxcor[0] = reg.root->pokemon_coormax[0];
	this->maxcor[1] = reg.root->pokemon_coormax[1];
	this->maxcor[2] = reg.root->pokemon_coormax[2];
	cpy(root, reg.root);
	return *this;
}

void Region::patch(Region reg)
{
	int minq[3];
	int maxq[3];
	minq[0] = reg.root->pokemon_coormin[0];
	minq[1] = reg.root->pokemon_coormin[1]; 
	minq[2] = reg.root->pokemon_coormin[2]; 
	maxq[0] = reg.root->pokemon_coormax[0]; 
	maxq[1] = reg.root->pokemon_coormax[1]; 
	maxq[2] = reg.root->pokemon_coormax[2];  
	nodeBul(reg.root);
	silerGider(root, minq, maxq);
	while(!(poks.empty()))
	{
		node *poke;
		poke = poks.top();
		poks.pop();
		placePokemon(*(poke->pokemon), poke->pokemon_coormin[0], poke->pokemon_coormin[1], poke->pokemon_coormin[2]);
	}
}

/*void Region::silici(node *poketop)
{
	if(poketop->left == NULL && poketop->right == NULL)
	{
		if(poketop->parent->left == poketop)
		{
			poketop->parent->left = NULL;
		}
		if(poketop->parent->right == poketop)
		{
			poketop->parent->right = NULL;
		}
		delete poketop;
	}
	if(poketop->left != NULL)
	{
		silici(poketop->left);
		if(poketop->left == NULL && poketop->right == NULL)
		{
			if(poketop->parent->left == poketop)
			{
				poketop->parent->left = NULL;
			}
			if(poketop->parent->right == poketop)
			{
				poketop->parent->right = NULL;
			}
			delete poketop;
		}
	}
	if(poketop->right != NULL)
	{
		silici(poketop->right);
		if(poketop->left == NULL && poketop->right == NULL)
		{
			if(poketop->parent->left == poketop)
			{
				poketop->parent->left = NULL;
			}
			if(poketop->parent->right == poketop)
			{
				poketop->parent->right = NULL;
			}
			delete poketop;
		}
	}
}
*/
void Region::silerGider(node *poketop, int minq[3], int maxq[3])
{
	if(poketop->left == NULL && poketop->right == NULL)
	{
		if(poketop->pokemon_coormin[0] >= minq[0] && poketop->pokemon_coormin[1] >= minq[1] && poketop->pokemon_coormin[2] >= minq[2])
		{
			if(poketop->pokemon_coormax[0] >= maxq[0] && poketop->pokemon_coormax[1] >= maxq[1] && poketop->pokemon_coormax[0] >= maxq[2])
			{
				if(poketop->parent->left == poketop)
				{
					poketop->parent->left = NULL;
				}
				if(poketop->parent->right == poketop)
				{
					poketop->parent->right = NULL;
				}
				delete poketop;
			}
		}
	}
	if(poketop->left != NULL)
	{
		silerGider(poketop->left, minq, maxq);
		if(poketop->pokemon_coormin[0] >= minq[0] && poketop->pokemon_coormin[1] >= minq[1] && poketop->pokemon_coormin[2] >= minq[2])
		{
			if(poketop->pokemon_coormax[0] >= maxq[0] && poketop->pokemon_coormax[1] >= maxq[1] && poketop->pokemon_coormax[0] >= maxq[2])
			{
				if(poketop->parent->left == poketop)
				{
					poketop->parent->left = NULL;
				}
				if(poketop->parent->right == poketop)
				{
					poketop->parent->right = NULL;
				}
				delete poketop;
			}
		}
	}
	if(poketop->right != NULL)
	{
		silerGider(poketop->right, minq, maxq);
		if(poketop->pokemon_coormin[0] >= minq[0] && poketop->pokemon_coormin[1] >= minq[1] && poketop->pokemon_coormin[2] >= minq[2])
		{
			if(poketop->pokemon_coormax[0] >= maxq[0] && poketop->pokemon_coormax[1] >= maxq[1] && poketop->pokemon_coormax[0] >= maxq[2])
			{
				if(poketop->parent->left == poketop)
				{
					poketop->parent->left = NULL;
				}
				if(poketop->parent->right == poketop)
				{
					poketop->parent->right = NULL;
				}
				delete poketop;
			}
		}
	}
}

void Region::nodeBul(node *poketop)
{
	if(poketop->pokemon_coormin[0] == poketop->pokemon_coormax[0] && poketop->pokemon_coormin[1] == poketop->pokemon_coormax[1] && poketop->pokemon_coormin[2] == poketop->pokemon_coormax[2])
	{
		poks.push(poketop);
		//return;
	}
	if(poketop->left != NULL)
	{
		nodeBul(poketop->left);
	}
	if(poketop->right != NULL)
	{
		nodeBul(poketop->right);
	}
}

