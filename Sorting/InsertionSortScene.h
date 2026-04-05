#pragma once

#include "SortScene.h"
#include <iostream>

class InsertionSortScene : public SortScene
{
private:
	int outerIndex = 1;     // current item to insert into the sorted left side
	int innerIndex = 1;     // walks left while shifting bigger values right
	bool passActive = false; // whether we're currently shifting for this outerIndex

	Color defaultColor = GRAY;
	Color keyColor = RED;               // the "key" being inserted
	Color compareColor = GOLD;              // current comparison
	Color sortedColor = { 85,160,47,255 };  // sorted: greenish

public:
	void Advance() override;
	void AdditionalResetOps() override {
		outerIndex = 0;
		innerIndex = 0;
		passActive = false;
	}
	InsertionSortScene() {
		sortSceneName = "Insertion Sort: ";
	}
};

void InsertionSortScene::Advance()
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
	// everything left of outerIndex is sorted-so-far, the rest default
	for (int i = 0; i < listSize; i++) {
		if (i < outerIndex) barsList[i].color = sortedColor;
		else barsList[i].color = defaultColor;
	}

	// Start a new insertion pass if needed
	if (!passActive) {
		innerIndex = outerIndex;
		passActive = true;
	}

	// Visualize the "key" (the element currently being inserted)
	if (innerIndex >= 0 && innerIndex < listSize) {
		barsList[innerIndex].color = keyColor;
	}

	// If we've reached the left edge, this pass is done
	if (innerIndex <= 0) {
		outerIndex++;
		passActive = false;
		return;
	}

	// Visualize the element being compared with the key (one comparison per tick)
	comparisons++;
	barsList[innerIndex - 1].color = compareColor;

	// If left is bigger, shift right (swap adjacent bars)
	if (barsList[innerIndex - 1].height > barsList[innerIndex].height) {
		std::swap(barsList[innerIndex - 1], barsList[innerIndex]);
		swaps++;
		innerIndex--; // keep moving left
		return;
	}

	// Otherwise, it's in the right spot; finish this pass and advance outerIndex
	outerIndex++;
	passActive = false;

}