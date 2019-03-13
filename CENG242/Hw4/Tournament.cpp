#include "Tournament.h"
#include <iostream>

using namespace std;

int d = -1;

Tournament::Tournament(const std::string& tournamentName)
{
	this->tournamentName = tournamentName;

}

Tournament::~Tournament()
{

}

const string& Tournament::getTournamentName() const
{
	return this->tournamentName;
}

void Tournament::registerTrainers(const std::vector<Trainer*>& trainers)
{
	Trainers = trainers;
}

Trainer* Tournament::commence()
{
	//cout<<Trainers[0]->getName()<<Trainers[1]->getName()<<Trainers[2]->getName()<<Trainers[3]->getName()<<Trainers[4]->getName()<<Trainers[5]->getName()<<Trainers[6]->getName()<<Trainers[7]->getName()<<endl;
	int duel = 0;
	int i = 0;
	int u = 0;
	int j = this->Trainers.size()- 1;
	int size = this->Trainers.size();
	Trainer* trainer1;
	Trainer* trainer2;
	Trainer* trainer3;
	while(size>1)
	{
		trainer1 = this->Trainers[i];
		trainer2 = this->Trainers[j];
		cout<<"Started Round: "<<i<<" "<<"vs"<<" "<<j<<endl;
		//cout<<"\t"<<trainer1->getName()<<" vs "<<trainer2->getName()<<":"<<endl;
		duel = duel + duelBetween(trainer1, trainer2, trainer1->getFavoriteArena());
		duel = duel - duelBetween(trainer2, trainer1, trainer2->getFavoriteArena());
		duel = duel + duelBetween(trainer1, trainer2, STADIUM);
		if(duel > 0)
		{
			cout<<"Round Winner: "<<trainer1->getName()<<"!"<<endl;
			//trainer3 = this->Trainers[j];
			//cout<<"silinmesi gereken:"<<trainer3->getName()<<"erase:"<<Trainers[j]->getName()<<endl;
			//Trainers.erase(Trainers.begin() + j);
			//cout<<"yerine geçti:"<<Trainers[j]->getName()<<endl;
			//delete trainer3;
			j--;
			i++;
		}
		else
		{
			cout<<"Round Winner: "<<trainer2->getName()<<"!"<<endl;
			trainer3 = this->Trainers[j];
			Trainers[i] = trainer3;
			//cout<<"silinmesi gereken:"<<trainer3->getName()<<"erase:"<<Trainers[i]->getName()<<endl;
			//Trainers.erase(Trainers.begin()+ i);
			//cout<<"yerine geçti:"<<Trainers[i]->getName()<<endl;
			//delete trainer3;
			i++;
			j--;
		}
		duel = 0;
		if(j<i)
		{
			i = 0;
			size = size / 2;
			j = size - 1;
		}
	}
	/*while(this->trainers[k] == NULL)
	{
		k++;
	}*/
	Trainer* winner = Trainers.front();
	//cout<<"Round"<<"\t"<<"Winner:"<<"\t"<<winner->getName()<<"!"<<endl<<endl;
	return winner;
}


int Tournament::duelBetween(Trainer* trainer1, Trainer* trainer2, Arena currentArena)
{
	int i = 0;
	int j = 0;
	int t = -1;
	int k = 0;
	int z = 0;
	cout<<"\t"<<trainer1->getName()<<" "<<"vs "<<trainer2->getName()<<": "<<Helpers::getArenaName(currentArena)<<endl;
	(trainer1->getPokemons())[i]->retArena(currentArena);
	(trainer2->getPokemons())[j]->retArena(currentArena);
	while(i < trainer1->getPokemons().size() && j < trainer2->getPokemons().size())
	{
		if(k == 1)
			(trainer2->getPokemons())[j]->retArena(currentArena);
		if(k == -1)
			(trainer1->getPokemons())[i]->retArena(currentArena);
		//if(d == -1)
			t = duelBetween((trainer1->getPokemons())[i], (trainer2->getPokemons())[j], currentArena);
		//if(d == 1)
			//z = duelBetween((trainer2->getPokemons())[j], (trainer1->getPokemons())[i], currentArena);
		//d = z;
		if(t == 1)
		{
			j++;
			k = 1;
		}
		if(t == -1)
		{
			i++;
			k = -1;
		}
	}
	for(int m = 0; m < (trainer1->getPokemons()).size(); m++)
	{
		(trainer1->getPokemons())[m]->setHP((trainer1->getPokemons())[m]->getcHP());
		(trainer1->getPokemons())[m]->setATK((trainer1->getPokemons())[m]->getcATK());
		(trainer1->getPokemons())[m]->setBurning(false);
		(trainer1->getPokemons())[m]->setDrowning(false);
		(trainer1->getPokemons())[m]->setRooted(false);
		(trainer1->getPokemons())[m]->setElectrified(false);
		(trainer1->getPokemons())[m]->setAr(0);
	}
	for(int n = 0; n < trainer2->getPokemons().size(); n++)
	{
		(trainer2->getPokemons())[n]->setHP((trainer2->getPokemons())[n]->getcHP());
		(trainer2->getPokemons())[n]->setATK((trainer2->getPokemons())[n]->getcATK());
		(trainer2->getPokemons())[n]->setBurning(false);
		(trainer2->getPokemons())[n]->setDrowning(false);
		(trainer2->getPokemons())[n]->setRooted(false);
		(trainer2->getPokemons())[n]->setElectrified(false);
		(trainer2->getPokemons())[n]->setAr(0);
	}
	d = -1;
	if(i == trainer1->getPokemons().size())
	{
		cout<<"\t"<<"WinnerT:"<<trainer2->getName()<<endl;
		cout<<endl;
		d = -1;
		return -1;
	}
	if(j == trainer2->getPokemons().size())
	{
		cout<<"\t"<<"WinnerT:"<<trainer1->getName()<<endl;
		cout<<endl;
		d = -1;
		return 1;
	}
}


int Tournament::duelBetween(Pokemon *pokemon1, Pokemon *pokemon2, Arena currentArena)
{
	
	while(1)
	{
		if(pokemon1->getHP() <= 0 || pokemon2->getHP() <= 0)
		{
			break;
		}
		else
		{
			if(d == -1)
			{
				pokemon1->attackTo(pokemon2, currentArena);
				if(pokemon1->getHP() <= 0 || pokemon2->getHP() <= 0)
				{
					break;
				}
				else
				{
					pokemon2->attackTo(pokemon1, currentArena);
				}
			}
			if(d == 1)
			{
				pokemon2->attackTo(pokemon1, currentArena);
				if(pokemon1->getHP() <= 0 || pokemon2->getHP() <= 0)
				{
					break;
				}
				else
				{
					pokemon1->attackTo(pokemon2, currentArena);
				}
			}
		}
	}
	if(pokemon2->getHP() <= 0)
	{
		pokemon1->levelUp();
		cout<<"\t\t"<<"WinnerP:"<<pokemon1->getName()<<endl;
		d = 1;
		return 1;
	}
	if(pokemon1->getHP() <= 0)
	{
		pokemon2->levelUp();
		cout<<"\t\t"<<"WinnerP:"<<pokemon2->getName()<<endl;
		d = -1;
		return -1;
	}
}
