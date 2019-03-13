#include "TrainerCenter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "PokeFire.h"
#include "PokeWater.h"
#include "PokeFlying.h"
#include "PokeGrass.h"
#include "PokeElectric.h"


using namespace std;

TrainerCenter::TrainerCenter(const string& inputFileName)
//int main()
{
	int c = 0;
	int pos = 0;
	int tc = 0;
	int dtc = 0;
	int pc = 0;
	int dpc = 0;
	int ID = 0;
	int PID = 0;
	string name;
	string Pname;
	string arena;
	string type;
	string line;
	string token;
	ifstream myfile(inputFileName.c_str());
	getline (myfile, line);
	pos = line.find(":");
	line.erase(0,pos+1);
	istringstream iss(line);
	iss >> tc;
	getline (myfile, line);
	pos = line.find(":");
	line.erase(0,pos+1);
	istringstream iss2(line);
	iss2 >> pc;
	dpc = pc;
	dtc = tc;
	while(tc > 0)
	{
		tc--;
		getline(myfile, line);
		getline(myfile, line);
		pos = line.find("--");
		token = line.substr(0,pos);
		istringstream iss3(token);
		iss3 >> ID;
		line.erase(0,pos+2);
		pos = line.find("--");
		name = line.substr(0, pos);
		line.erase(0,pos+2);
		arena = line;

		Arena favArena;
		if(arena == "Forest")
			favArena = FOREST;
		if(arena == "Magma")
			favArena = MAGMA;
		if(arena == "Ocean")
			favArena = OCEAN;
		if(arena == "ElectriCity")
			favArena = ELECTRICITY;
		if(arena == "Sky")
			favArena = SKY;
		vector<Pokemon*> Pokemons;
		Trainer* trainer1 = new Trainer(ID, name,favArena, Pokemons);
		Trainers.push_back(trainer1);
		while(pc > 0)
		{
			pc--;
			getline(myfile, line);
			pos = line.find("::");
			token = line.substr(0,pos);
			istringstream iss5(token);
			iss5 >> PID;
			line.erase(0,pos+2);
			pos = line.find("::");
			Pname = line.substr(0, pos);
			line.erase(0,pos+2);
			type = line;

			if(type == "FIRE")
			{
				PokeFire* pokemon = new PokeFire(PID, Pname);
				trainer1->getPokemons().push_back(pokemon);
			}
			if(type == "FLYING")
			{
				PokeFlying* pokemon = new PokeFlying(PID, Pname);
				trainer1->getPokemons().push_back(pokemon);
			}
			if(type == "ELECTRIC")
			{
				PokeElectric* pokemon = new PokeElectric(PID, Pname);
				trainer1->getPokemons().push_back(pokemon);
			}
			if(type == "WATER")
			{
				PokeWater* pokemon = new PokeWater(PID, Pname);
				trainer1->getPokemons().push_back(pokemon);
			}
			if(type == "GRASS")
			{
				PokeGrass* pokemon = new PokeGrass(PID, Pname);
				trainer1->getPokemons().push_back(pokemon);
			}
				//Trainers.back()->getPokemons().push_back(new PokeWater(ID, Pname));
		}
		pc = dpc;
		//cout<<"pc:"<<pc;
	}
	tc = dtc;
	//cout<<"tc:"<<tc;
	//myfile.close();
	//cout<<tc<<pc<<endl;
	//return 0;
}

TrainerCenter::~TrainerCenter()
{
	for(int i = 0; i < Trainers.size(); i++)
	{
		for(int j = 0; j < Trainers[i]->getPokemons().size(); j++)
		{
			delete (Trainers[i]->getPokemons())[j];
		}
	delete Trainers[i];
	}
}

std::vector<Trainer*>& TrainerCenter::getAllTrainers()
{
	return Trainers;
}

Trainer* TrainerCenter::getTrainerWithID(int trainerID)
{
	int i = 0;
	while(i < Trainers.size())
	{
		if(Trainers[i]->getTrainerID() == trainerID)
		{
			return Trainers[i];
		}
	}
}