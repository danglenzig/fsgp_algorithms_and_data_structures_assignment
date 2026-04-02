#pragma once

#include "../Tools/EventSystem.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "raylib.h"

struct SortBarData {
	Color color;
	int height;
	int width;
};

struct SortSceneDrawData {
	std::string algorithmString;
	float stepInterval;
	std::vector<SortBarData> barsList;
};




class SortSceneManager
{
private:

	const float MIN_STEP_INTERVAL = 0.02f;
	const float MAX_STEP_INTERVAL = 1.0f;
	const float STEP_ADJUST_INCREMENT = 0.02f;

	size_t updateHandle = -1;
	size_t increaseSpeedHandle = -1;
	size_t decreaseSpeedHandle = -1;
	float stepInterval = 0.2f;
	float tA = 0.0f;
	bool isActive = false;

	void OnFrameUpdate(const float& dT);
	void OnIncreaseStepSpeedPressed();
	void OnDecreaseStepSpeedPressed();

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

	std::cout << "STEP INTERVAL: "<< stepInterval <<"s\n";

	// TODO...step the current sort scene.
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
	isActive = value;
	if (isActive) { tA = 0.0f; }
}

SortSceneDrawData SortSceneManager::GetDrawData()
{
	if (!isActive) {
		return{
			"",
			stepInterval,
			{}
		};
	}

	// get all the relevant data from the current SortScene

	// We haven't created the SortScene class yet, so
	//   for now just return placeholder data
	return{
		// Placeholder...
		"PLACEHOLDER STRING",
		stepInterval,
		{}
	};
}

