#pragma once

#include "../Tools/EventSystem.h"
#include "../Data/structs.h"
#include "../Data/DataFactory.h"
#include "../Sorting/SortScene.h"
#include "../Sorting/BubbleSortScene.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>
#include "raylib.h"

class SortSceneManager
{
private:

	const float MIN_STEP_INTERVAL = 0.02f;
	const float MAX_STEP_INTERVAL = 1.0f;
	const float STEP_ADJUST_INCREMENT = 0.02f;
	const bool USE_PLACEHOLDER_DATA = true;

	SortScene* currentSortScene = nullptr;
	std::vector<std::unique_ptr<SortScene>> sortScenes;
	size_t updateHandle = -1;
	size_t increaseSpeedHandle = -1;
	size_t decreaseSpeedHandle = -1;
	float stepInterval = 0.2f;
	float tA = 0.0f;
	bool isActive = false;
	SortSceneDrawData currentDrawData;

	void OnFrameUpdate(const float& dT);
	void OnIncreaseStepSpeedPressed();
	void OnDecreaseStepSpeedPressed();
	void UpdateDrawData();
	void ShuffleBars();
	SortSceneDrawData InitSceneData();

public:
	// constructor / destructor
	SortSceneManager(const float& stepInterval);
	~SortSceneManager() {
		EventSystem::Instance().FrameUpdate.Unsubscribe(updateHandle);
		EventSystem::Instance().IncreaseStepSpeedPressed.Unsubscribe(increaseSpeedHandle);
		EventSystem::Instance().DecreaseStepSpeedPressed.Unsubscribe(decreaseSpeedHandle);
		
	}

	// public functions
	void SetIsActive(const bool& value);
	SortSceneDrawData GetDrawData();
	void InitializeSceneData();
};

// constructor / destructor
SortSceneManager::SortSceneManager(const float& _stepInterval)
{
	updateHandle = EventSystem::Instance().FrameUpdate.Subscribe(
		[this](const float& dT) {
			OnFrameUpdate(dT);
		}
	);
	increaseSpeedHandle = EventSystem::Instance().IncreaseStepSpeedPressed.Subscribe(
		[this]() {
			OnIncreaseStepSpeedPressed();
		}
	);
	decreaseSpeedHandle = EventSystem::Instance().DecreaseStepSpeedPressed.Subscribe(
		[this]() {
			OnDecreaseStepSpeedPressed();
		}
	);

	stepInterval = _stepInterval;
}



// ======= Private functions =======
void SortSceneManager::OnFrameUpdate(const float& dT)
{
	if (!isActive) { return; }
	tA += dT;
	if (tA < stepInterval) { return; }
	tA = 0.0f;	

	UpdateDrawData();
}



void SortSceneManager::UpdateDrawData()
{
	if (currentSortScene) {
		currentDrawData.algorithmString = currentSortScene->GetName();
		currentSortScene->Advance(currentDrawData.barsList);
		currentDrawData.stepCount = currentSortScene->GetStats().steps;
		currentDrawData.comparisons = currentSortScene->GetStats().comparisons;
		currentDrawData.swaps = currentSortScene->GetStats().swaps;
	}
	currentDrawData.stepInterval = stepInterval;
}

SortSceneDrawData SortSceneManager::InitSceneData()
{
	std::string placeholderStr = "PLACEHOLDER";
	//std::string placeholderStr = "Bubble Sort: ";
	float placeholderInterval = stepInterval;
	int placeholderStepCount = -1;
	int placeholderComparisons = -1;
	int placeholderSwaps = -1;
	std::vector<SortBarData> placeholderBars = DataFactory::Instance().GetBarsList(120);
	return SortSceneDrawData(
		placeholderStr, placeholderInterval, placeholderStepCount, placeholderComparisons, placeholderSwaps, placeholderBars
	);
}

void SortSceneManager::OnIncreaseStepSpeedPressed()
{
	if (!isActive) { return; }

	stepInterval = std::max(
		MIN_STEP_INTERVAL, stepInterval - STEP_ADJUST_INCREMENT
	);
}

void SortSceneManager::OnDecreaseStepSpeedPressed()
{
	if (!isActive) { return; }
	stepInterval = std::min(
		MAX_STEP_INTERVAL, stepInterval + STEP_ADJUST_INCREMENT
	);
}

// ======= Public functions =======

void SortSceneManager::InitializeSceneData() {
	currentDrawData = currentDrawData = InitSceneData();
	ShuffleBars();


	// create a Bubble SortScene and store it polymorphically
	//BubbleSortScene bubbleSortScene = BubbleSortScene();
	// create a BubbleEarlyEscape SortScene
	// create a Selection SortScene
	// create an Insertion SortScene

	// add them to the list
	sortScenes.push_back(std::make_unique<BubbleSortScene>());
	// ...
	// ...
	if (!sortScenes.empty()) {
		currentSortScene = sortScenes[0].get();
	}

}

void SortSceneManager::ShuffleBars()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(currentDrawData.barsList.begin(), currentDrawData.barsList.end(), std::default_random_engine(seed));
}

void SortSceneManager::SetIsActive(const bool& value)
{
	if (isActive == value) { return; }

	isActive = value;
	if (isActive) { tA = 0.0f; }
}

SortSceneDrawData SortSceneManager::GetDrawData()
{
	//if (!isActive) {
	//	
	//	return SortSceneDrawData(); // blank data
	//}
	return currentDrawData;
}

