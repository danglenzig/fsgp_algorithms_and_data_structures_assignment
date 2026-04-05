#pragma once
#include "SortScene.h"
#include <string>
#include <iostream>
#include <iomanip>

class HeapSortScene : public SortScene
{
private:
	int buildComparisons = 0;
	int extractComparisons = 0;
	int idx = 0;
	int end = 0;

	Color sortedColor = { 85,160,47,255 }; // greenish
	Color compareColor = GOLD;
	Color largestColor = RED;
	Color defaultColor = GRAY;
	Color buildColor = DARKBLUE;

	// incremental sift-down state
	bool siftInProgress = false; // true when a sift is currently running across ticks
	int siftRoot = -1; // original root for the current sift (for build phase)
	int siftIdx = -1; // current node index being sifted (moves down)
	int siftTargetEnd = 0; // current heap size to consider (used in extract phase)

	enum EnumHeapSortPhase {
		BUILD_HEAP,
		EXTRACT
	};
	EnumHeapSortPhase currentPhase = EnumHeapSortPhase::BUILD_HEAP;

	void SiftDownBuild(); // performs one step of sift-down for the build phase
	void SiftDownExtract(); // performs one step of sift-down for the extract phase
	void BuildHeapInfoString();

public:
	void Advance() override;
	void AdditionalResetOps() override {
		currentPhase = EnumHeapSortPhase::BUILD_HEAP;
		idx = 0;
		siftInProgress = false;
		siftRoot = -1;
		siftIdx = -1;
		siftTargetEnd = 0;
		buildComparisons = 0;
		extractComparisons = 0;
	}
	void AdditionalStartOps() override {
		idx = (int)((barsList.size() / 2) - 1);
		end = barsList.size() - 1;
		siftInProgress = false;
		siftRoot = -1;
		siftIdx = -1;
		siftTargetEnd = 0;
	}
	HeapSortScene() {
		sortSceneName = "Heap Sort: ";
	}
};

void HeapSortScene::Advance()
{
	if (sorted) { return; }
	comparisons = buildComparisons + extractComparisons;


	steps++;

	switch (currentPhase) {
	case EnumHeapSortPhase::BUILD_HEAP:

		// when all internal nodes processed, switch to extract phase
		if (idx < 0 && !siftInProgress) {
			// barsList should now be organized as a proper max-heap
			currentPhase = EnumHeapSortPhase::EXTRACT;
			// reset end if not already set by Start()
			if (end <= 0) {
				end = (int)barsList.size() - 1;
			}

			for (int i = 0; i < barsList.size(); i++) {
				barsList[i].color = defaultColor;
			}

			BuildHeapInfoString();
			return;
		}

		// If no sift is active, start one for the current idx
		if (!siftInProgress) {
			siftInProgress = true;
			siftRoot = idx;
			siftIdx = siftRoot;
			//buildComparisons = 0;
		}

		// Perform one step of sift-down for this root
		SiftDownBuild();

		// If sift finished this tick, move to the next root
		if (!siftInProgress) {
			// decrement the idx counter
			idx--;
			// add the build phase comparisons to the total comparisons
			//comparisons += buildComparisons;
			// update the additional information string
			BuildHeapInfoString();
		}
		return;

	case EnumHeapSortPhase::EXTRACT:
		
		// If heap is fully extracted and no sift in progress, done
		if (end <= 0 && !siftInProgress) {
			// mark remaining single element sorted
			if (!barsList.empty()) barsList[0].color = sortedColor;
			sorted = true;
			BuildHeapInfoString();
			return;
		}

		// If no sift is active, perform the swap (move max to end) and start sift-down
		if (!siftInProgress) {
			// swap root (max) with element at end
			swaps++;
			std::swap(barsList[0], barsList[end]);

			// mark the placed element as sorted for visibility
			barsList[end].color = { 85,160,47,255 };

			// prepare for sift-down on reduced heap
			siftInProgress = true;
			siftIdx = 0;
			siftTargetEnd = end; // sift should operate on elements [0 .. end-1]
			//extractComparisons = 0;
		}

		// Continue (or finish) the sift-down for this extract pass
		SiftDownExtract();

		// If sift finished this tick, decrement end and accumulate comparisons
		if (!siftInProgress) {
			end--;
			//comparisons += extractComparisons;
		}
		BuildHeapInfoString();
		return;
	}
}

void HeapSortScene::SiftDownBuild()
{

	// reset visualization
	for (int i = 0; i < barsList.size(); i++) {
		barsList[i].color = buildColor;
	}


	int size = (int)barsList.size();
	// If siftIdx is a leaf, we're done
	int leftChildIdx = 2 * siftIdx + 1;
	if (leftChildIdx >= size) {
		siftInProgress = false;
		return;
	}

	// Find the largest among parent and children (do comparisons and count them)
	int largest = siftIdx;

	// compare left child
	buildComparisons++;
	if (barsList[leftChildIdx].height > barsList[largest].height) {
		largest = leftChildIdx;
	}

	// compare right child if exists
	int rightChildIdx = leftChildIdx + 1;
	if (rightChildIdx < size) {
		buildComparisons++;
		if (barsList[rightChildIdx].height > barsList[largest].height) {
			largest = rightChildIdx;
		}
	}

	// Visualization: highlight current nodes
	barsList[siftIdx].color = compareColor;
	if (largest != siftIdx) barsList[largest].color = largestColor;

	// If largest is not parent, swap and continue sift on the child next tick
	if (largest != siftIdx) {
		std::swap(barsList[siftIdx], barsList[largest]);
		swaps++;
		// continue sifting down from 'largest' on the next Advance
		siftIdx = largest;
		// keep siftInProgress = true
		return;
	}

	// If heap property satisfied at this node, finish this sift
	siftInProgress = false;
	// NOTE: clear highlight for nodes involved?
	// (we'll let Update/other code set default colors next tick)

}

void HeapSortScene::SiftDownExtract()
{
	int size = siftTargetEnd; // do not include element at index 'end'
	// If siftIdx is leaf, done
	int leftChildIdx = 2 * siftIdx + 1;
	if (leftChildIdx >= size) {
		siftInProgress = false;
		return;
	}

	// reset visualization on sift area
	for (int i = 0; i < siftTargetEnd; i++) {
		barsList[i].color = defaultColor;
	}


	// Find the largest among parent and children (do comparisons and count them)
	int largest = siftIdx;

	// compare left child
	extractComparisons++;
	if (barsList[leftChildIdx].height > barsList[largest].height) {
		largest = leftChildIdx;
	}

	// compare right child if exists
	int rightChildIdx = leftChildIdx + 1;
	if (rightChildIdx < size) {
		extractComparisons++;
		if (barsList[rightChildIdx].height > barsList[largest].height) {
			largest = rightChildIdx;
		}
	}

	// Visualization: highlight current nodes
	barsList[siftIdx].color = compareColor;
	if (largest != siftIdx) barsList[largest].color = largestColor;

	// If largest is not parent, swap and continue sift on the child next tick
	if (largest != siftIdx) {
		std::swap(barsList[siftIdx], barsList[largest]);
		swaps++;
		// continue sifting down from 'largest' on the next Advance
		siftIdx = largest;
		// keep siftInProgress = true
		return;
	}

	// If heap property satisfied at this node, finish this sift
	siftInProgress = false;
	// Note: clear highlight for nodes involved?




}

void HeapSortScene::BuildHeapInfoString()
{
	additionalInformation = std::format(
		"build phase comparisons: {}, extract phase comparisons: {}", buildComparisons, extractComparisons
	);
}
