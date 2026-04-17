#pragma once
#include "../Data/structs.h"
#include "../Data/DataFactory.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <random>

// parent class for all the sort scenes

struct SortSceneStats
{
	int steps;
	int swaps;
	int comparisons;
};

class SortScene
{
protected:
	std::string sortSceneName = "";
	std::string additionalInformation = "";
	int steps = 0;
	int swaps = 0;
	int comparisons = 0;
	std::vector<SortBarData> barsList = {};
	int listSize = 0;
	bool sorted = false;

private:
	//
public:
	virtual void Advance() {}
	virtual void AdditionalResetOps() {}
	virtual void AdditionalStartOps() {};
	SortScene();
	~SortScene();
	void ResetStats();
	void Start();
	void Shuffle();


	std::string GetName() { return sortSceneName; }
	SortSceneStats GetStats() {
		return{
			steps,
			swaps,
			comparisons,
		};
	}
	std::vector<SortBarData> GetBarsList() {
		return barsList;
	}
	std::string GetAdditionalInfo() {
		return additionalInformation;
	}

};


inline SortScene::SortScene() {

}
inline SortScene::~SortScene() {

}

inline void SortScene::Shuffle()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(barsList.begin(), barsList.end(), std::default_random_engine(seed));
}

inline void SortScene::ResetStats()
{
	steps = -1;
	swaps = 0;
	comparisons = 0;
	sorted = false;
}

inline void SortScene::Start()
{
	ResetStats();
	AdditionalResetOps();
	barsList = DataFactory::Instance().GetBarsList(120);
	listSize = barsList.size();
	Shuffle();
	AdditionalStartOps();
}