#pragma once
#include <vector>
#include <tuple>
#include <string>
using namespace std;

class Town
{
public:
public: int rate;
public: vector<pair<string, int>> ProductionRecources;
public: vector<string> upgrades;
public: vector<string> affinities;
public: vector<pair<string, int>> ItemRates;
public: pair<int, int> Enteloot;
	 
	//const vector<vector<pair<int, int>>> craftingCost;

	vector<pair<int,int>> CalcRecourcesAccumulated(int Ticks){
		int cycles = floor(Ticks / rate);
		vector<pair<int, int>> OutVector;
		for (pair<string,int> var : ProductionRecources) {
			OutVector.emplace_back(var.first, var.second * cycles);
		}
		return OutVector;
	};
	int CalcEnteloot(int Ticks) {
		int cycles = floor(Ticks / Enteloot.first);
		return cycles * Enteloot.second;
	}
	
	//tuple<int, int, vector<pair<int, int>>> BuildUpgrade(vector <pair<int, int>> mayorRecources, int Enteloot, int BuildOption) {
	//	for (pair<int,int> var : mayorRecources)
	//	{
	//		if 
	//	}
	//}
	


};

