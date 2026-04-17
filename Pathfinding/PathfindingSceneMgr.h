#pragma once
#include "../Data/structs.h"
#include "../Tools/EventSystem.h"
#include "MazeTools.h"
#include <iostream>
#include <vector>

class PathfindingSceneMgr
{
private:

	const size_t MAZE_SIZE = 20;
	const int X_OFFSET = 300;
	const int Y_OFFSET = 40;
	const int NODE_SPACING = 30;


	bool isActive = false;
	float tA = 0.0f;
	float stepInterval = 0.2f;
	size_t updateHandle = -1;
	size_t increaseSpeedHandle = -1;
	size_t decreaseSpeedHandle = -1;
	size_t resetHandle = -1;
	size_t nextHandle = -1;

	PathfindingSceneDrawData currentDrawData;

	void OnFrameUpdate(const float& dT);
	void OnIncreaseSpeedPressed();
	void OnDecreaseSpeedPressed();
	void OnResetPressed();
	void OnNextPressed();
	void UpdateDrawData();

	NodePosDict GetNodePosDict();

	

public:
	PathfindingSceneMgr(const float& stepInterval);
	~PathfindingSceneMgr();

	void InitializeSceneData();
	void SetIsActive(const bool& value);

	void Test() {
		std::cout << "BAR\n";
	}

	PathfindingSceneDrawData GetDrawData();

};

inline PathfindingSceneMgr::PathfindingSceneMgr(const float& _stepInterval)
{
	stepInterval = _stepInterval;
	updateHandle = EventSystem::Instance().FrameUpdate.Subscribe(
		[this](const float& dT) {
			OnFrameUpdate(dT);
		}
	);
	increaseSpeedHandle = EventSystem::Instance().IncreaseStepSpeedPressed.Subscribe(
		[this]() {
			OnIncreaseSpeedPressed();
		}
	);
	decreaseSpeedHandle = EventSystem::Instance().DecreaseStepSpeedPressed.Subscribe(
		[this]() {
			OnDecreaseSpeedPressed();
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
}

inline PathfindingSceneMgr::~PathfindingSceneMgr()
{
	EventSystem::Instance().FrameUpdate.Unsubscribe(updateHandle);
	EventSystem::Instance().IncreaseStepSpeedPressed.Unsubscribe(increaseSpeedHandle);
	EventSystem::Instance().DecreaseStepSpeedPressed.Unsubscribe(decreaseSpeedHandle);
	EventSystem::Instance().ResetPressed.Unsubscribe(resetHandle);
	EventSystem::Instance().NextPressed.Unsubscribe(nextHandle);
}

inline void PathfindingSceneMgr::InitializeSceneData()
{
	SquareMaze maze = SquareMaze(MAZE_SIZE);
	currentDrawData.graph =maze.AsGraph().graph;
	currentDrawData.mazeSize = MAZE_SIZE;
	currentDrawData.xOffset = X_OFFSET;
	currentDrawData.yOffset = Y_OFFSET;
	currentDrawData.nodeSpacing = NODE_SPACING;

	currentDrawData.startTextPosX = X_OFFSET - NODE_SPACING;
	currentDrawData.startTextPosY = Y_OFFSET;

	currentDrawData.nodePosDict = GetNodePosDict();
}

inline void PathfindingSceneMgr::SetIsActive(const bool& value)
{
	if (isActive == value) { return; }
	isActive = value;
	if (isActive) { tA = 0.0f; }
}

inline void PathfindingSceneMgr::OnFrameUpdate(const float& dT)
{
	if (!isActive) { return; }
	tA += dT;
	if (tA < stepInterval) { return; }
	tA = 0.0f;
	UpdateDrawData();
}
inline void PathfindingSceneMgr::OnIncreaseSpeedPressed() {}
inline void PathfindingSceneMgr::OnDecreaseSpeedPressed() {}
inline void PathfindingSceneMgr::OnResetPressed() {}
inline void PathfindingSceneMgr::OnNextPressed() {}
inline void PathfindingSceneMgr::UpdateDrawData() {}

inline PathfindingSceneDrawData PathfindingSceneMgr::GetDrawData()
{
	return currentDrawData; // NOTE: struct still undefined
}

inline NodePosDict PathfindingSceneMgr::GetNodePosDict()
{

	size_t mazeSize = currentDrawData.mazeSize;
	int xOffset = currentDrawData.xOffset;
	int yOffset = currentDrawData.yOffset;
	int spacing = currentDrawData.nodeSpacing;

	NodePosDict posDict = {};
	
	size_t count = -1;

	for (int col = 0; col < mazeSize; ++col) {
		for (int row = 0; row < mazeSize; ++row) {
			++count;
			int xPos = (row * spacing) + xOffset;
			int yPos = (col * spacing) + yOffset;
			posDict[count] = { xPos, yPos };

		}
	}

	return posDict;
}