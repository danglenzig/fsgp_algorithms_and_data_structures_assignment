#pragma once
#include "../Data/structs.h"
#include <vector>
#include <iostream>

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
	int steps = 0;
	int swaps = 0;
	int comparisons = 0;
private:
	//
public:
	SortScene();
	~SortScene();
	virtual void Advance(std::vector<SortBarData> &barList) {}
	virtual void ResetStats() {}

	std::string GetName() { return sortSceneName; }
	SortSceneStats GetStats() {
		return{
			steps,
			swaps,
			comparisons,
		};
	}

};


SortScene::SortScene() {

}
SortScene::~SortScene() {

}