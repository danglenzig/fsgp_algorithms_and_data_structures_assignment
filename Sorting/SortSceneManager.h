#pragma once

#include "../Tools/EventSystem.h"
#include "../Data/structs.h"
#include "../Data/DataFactory.h"
#include "../Sorting/SortScene.h"
#include "../Sorting/BubbleSortScene.h"
#include "../Sorting/SelectionSortScene.h"
#include "../Sorting/InsertionSortScene.h"
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
	int currentSortSceneIdx = 0;
	std::vector<std::unique_ptr<SortScene>> sortScenes;
	size_t updateHandle = -1;
	size_t increaseSpeedHandle = -1;
	size_t decreaseSpeedHandle = -1;
	size_t nextHandle = -1;
	size_t resetHandle = -1;
	float stepInterval = 0.2f;
	float tA = 0.0f;
	bool isActive = false;
	SortSceneDrawData currentDrawData;

	void OnFrameUpdate(const float& dT);
	void OnIncreaseStepSpeedPressed();
	void OnDecreaseStepSpeedPressed();
	void OnNextPressed();
	void OnResetPressed();
	void UpdateDrawData();
	//void ShuffleBars();
	//SortSceneDrawData InitSceneData();

public:
	// constructor / destructor
	SortSceneManager(const float& stepInterval);
	~SortSceneManager() {
		EventSystem::Instance().FrameUpdate.Unsubscribe(updateHandle);
		EventSystem::Instance().IncreaseStepSpeedPressed.Unsubscribe(increaseSpeedHandle);
		EventSystem::Instance().DecreaseStepSpeedPressed.Unsubscribe(decreaseSpeedHandle);
		EventSystem::Instance().ResetPressed.Unsubscribe(resetHandle);
		EventSystem::Instance().NextPressed.Unsubscribe(nextHandle);
		
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
	resetHandle = EventSystem::Instance().ResetPressed.Subscribe(
		[this]() {
			OnResetPressed();
		}
	);
	nextHandle = EventSystem::Instance().NextPressed.Subscribe(
		[this]() {
			OnNextPressed();
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
		currentSortScene->Advance();
		currentDrawData.stepCount = currentSortScene->GetStats().steps;
		currentDrawData.comparisons = currentSortScene->GetStats().comparisons;
		currentDrawData.swaps = currentSortScene->GetStats().swaps;
		currentDrawData.barsList = currentSortScene->GetBarsList();
	}
	currentDrawData.stepInterval = stepInterval;
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

void SortSceneManager::OnNextPressed()
{
	currentSortSceneIdx = (currentSortSceneIdx + 1) % sortScenes.size();
	if (sortScenes[currentSortSceneIdx]) {
		currentSortScene = sortScenes[currentSortSceneIdx].get();
	}
	currentSortScene->Start();
}

void SortSceneManager::OnResetPressed()
{
	if (currentSortScene) {
		currentSortScene->Start();
	}

}

// ======= Public functions =======

void SortSceneManager::InitializeSceneData() {

	// create SortScenes and store it polymorphically
	sortScenes.push_back(std::make_unique<BubbleSortScene>());
	//sortScenes.push_back(std::make_unique<BubbleEarlyEscapeSortScene>());
	sortScenes.push_back(std::make_unique<SelectionSortScene>());
	sortScenes.push_back(std::make_unique<InsertionSortScene>());
	//sortScenes.push_back(std::make_unique<HeapSortScene>());
	if (!sortScenes.empty()) {
		currentSortScene = sortScenes[currentSortSceneIdx].get();
		currentSortScene->Start();
	}
}



void SortSceneManager::SetIsActive(const bool& value)
{
	if (isActive == value) { return; }

	isActive = value;
	if (isActive) { tA = 0.0f; }
}

SortSceneDrawData SortSceneManager::GetDrawData()
{
	
	return currentDrawData;
}

