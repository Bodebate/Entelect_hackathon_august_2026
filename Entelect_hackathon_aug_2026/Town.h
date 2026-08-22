#pragma once
#include <vector>
#include <tuple>

using namespace std;
class Town
{
	int rate;
	vector<pair<int, int>> ProductionRecources;
	vector<int> upgrades;
	vector<int> affinities;
	vector<pair<int, int>> sellRates;
	pair<int, int> Enteloot;
	
	const vector<vector<pair<int, int>>> craftingCost;

	struct Edge
	{
		int To;
		int Travel_time;
		int Travel_Cost;
	};

	vector<pair<int,int>> CalcRecourcesAccumulated(int Ticks){
		int cycles = floor(Ticks / rate);
		vector<pair<int, int>> OutVector;
		for (pair<int,int> var : ProductionRecources) {
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

