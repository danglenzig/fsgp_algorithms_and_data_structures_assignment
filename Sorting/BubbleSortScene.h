#pragma once

#include "SortScene.h"
#include <iostream>

class BubbleSortScene : public SortScene
{
private:
	int outerIndex = 0;
	int innerIndex = 0;
	Color defaultColor = GRAY;
	Color compareColor = GOLD;
	Color sortedColor = { 85,160,47,255 };

public:
	void Advance() override;
	BubbleSortScene() {
		sortSceneName = "Bubble Sort: ";
	}
};


void BubbleSortScene::Advance()
{
	if (sorted) { return; }

	if (outerIndex >= listSize) {
		for (int i = 0; i < barsList.size(); i++) {
			barsList[i].color = sortedColor;
		}
		sorted = true;
		return;
	}

	steps++;

	if (innerIndex >= (listSize - outerIndex - 1)) {

		barsList[barsList.size() - 1 - outerIndex].color = sortedColor; // sorted: greenish

		for (int i = 0; i < (listSize - outerIndex - 1); i++) {
			barsList[i].color = defaultColor;
		}

		outerIndex++;
		innerIndex = 0;
	}

	comparisons++;

	barsList[innerIndex].color = compareColor;
	barsList[innerIndex + 1].color = compareColor;

	if (barsList[innerIndex].height > barsList[innerIndex + 1].height) {
		std::swap(barsList[innerIndex], barsList[innerIndex + 1]);
		
		swaps++;
	}
	innerIndex++;
}