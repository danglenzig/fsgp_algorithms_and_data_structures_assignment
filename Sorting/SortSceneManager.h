#pragma once

#include "../Tools/EventSystem.h"
#include "../Data/structs.h"
#include "../Data/DataFactory.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>
#include "raylib.h"

class SortSceneManager
{
private:

	const float MIN_STEP_INTERVAL = 0.02f;
	const float MAX_STEP_INTERVAL = 1.0f;
	const float STEP_ADJUST_INCREMENT = 0.02f;
	const bool USE_PLACEHOLDER_DATA = true;

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
	SortSceneDrawData GetPlaceholderData();

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

	// TODO...step the current sort scene.
}

void SortSceneManager::UpdateDrawData()
{

	if (currentDrawData.algorithmString == "PLACEHOLDER DATA") { return; } // no need to update
	
	// TODO: get load data from currentSortScene into currentDrawData
}

SortSceneDrawData SortSceneManager::GetPlaceholderData()
{
	std::string placeholderStr = "PLACEHOLDER DATA";
	//std::string placeholderStr = "Bubble Sort: ";
	float placeholderInterval = stepInterval;
	int placeholderStepCount = 20;
	int placeholderComparisons = 20;
	std::vector<SortBarData> placeholderBars = DataFactory::Instance().GetSortedBarsList(120);
	return SortSceneDrawData(
		placeholderStr, placeholderInterval, placeholderStepCount, placeholderComparisons, placeholderBars
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
void SortSceneManager::SetIsActive(const bool& value)
{
	if (isActive == value) { return; }
	currentDrawData = GetPlaceholderData(); // initialize with valid placeholder data
	isActive = value;
	if (isActive) { tA = 0.0f; }
}

SortSceneDrawData SortSceneManager::GetDrawData()
{
	if (!isActive) {
		return SortSceneDrawData(); // blank data
	}

	return currentDrawData;
}

