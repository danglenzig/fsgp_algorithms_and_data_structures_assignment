#pragma once

#include "SortScene.h"
#include <iostream>

class SelectionSortScene : public SortScene
{

private:
	int outerIndex = 0;   // where we place the next smallest value
	int scanIndex = 1;    // scans the unsorted part
	int minIndex = 0;     // index of current minimum found

	Color defaultColor = GRAY;
	Color scanColor = GOLD;                 // currently comparing
	Color minColor = RED;                // current minimum candidate
	Color sortedColor = { 85,160,47,255 };  // sorted: greenish

public:
	void Advance() override;
	void AdditionalResetOps() override {
		outerIndex = 0;
		scanIndex = 1;
		minIndex = 0;
	}
	SelectionSortScene() {
		sortSceneName = "Selection Sort: ";
	}
};

inline void SelectionSortScene::Advance()
{
	if (sorted) { return; }

	// done: color everything sorted and stop
	if (outerIndex >= listSize) {
		for (int i = 0; i < (int)barsList.size(); i++) {
			barsList[i].color = sortedColor;
		}
		sorted = true;
		return;
	}

	steps++;


	// Reset colors each tick:
	// - everything after outerIndex becomes default
	// - everything before outerIndex stays sorted
	for (int i = 0; i < listSize; i++) {
		if (i < outerIndex) barsList[i].color = sortedColor;
		else barsList[i].color = defaultColor;
	}

	// Make sure indices are valid when starting or after finishing a pass
	if (scanIndex <= outerIndex) {
		minIndex = outerIndex;
		scanIndex = outerIndex + 1;
	}

	// If we finished scanning this pass, do the final swap and advance outerIndex
	if (scanIndex >= listSize) {
		if (minIndex != outerIndex) {
			std::swap(barsList[outerIndex], barsList[minIndex]);
			swaps++;
		}

		// outerIndex is now correct (smallest placed), mark it sorted
		barsList[outerIndex].color = sortedColor;

		outerIndex++;
		minIndex = outerIndex;
		scanIndex = outerIndex + 1;
		return;
	}
	// Highlight what we're looking at this tick
	barsList[minIndex].color = minColor;
	barsList[scanIndex].color = scanColor;

	// One comparison per tick
	comparisons++;
	if (barsList[scanIndex].height < barsList[minIndex].height) {
		minIndex = scanIndex;
	}

	scanIndex++;
}