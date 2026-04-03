#pragma once

#include "SortScene.h"
#include <iostream>

class BubbleSortScene : public SortScene
{
private:
	//
public:
	void Advance(std::vector<SortBarData>& barList) override;
	

	BubbleSortScene() {
		sortSceneName = "Bubble Sort: ";
	}

};


void BubbleSortScene::Advance(std::vector<SortBarData>& barList)
{
	// TODO
}