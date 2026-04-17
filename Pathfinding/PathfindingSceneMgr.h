#pragma once
#include "../Data/structs.h"
#include "../Tools/EventSystem.h"
#include "MazeTools.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
//#include <cstdio>


class PathfindingSceneMgr
{
private:

	const size_t MAZE_SIZE = 20;
	const int X_OFFSET = 300;
	const int Y_OFFSET = 40;
	const int NODE_SPACING = 30;
	const float RAT_SPEED = 150.0f;
	const float CLOSE_ENOUGH = 0.05f;


	bool isActive = false;
	bool ratAtGoal = false;
	float tA = 0.0f;
	float stepInterval = 0.2f;
	size_t updateHandle = -1;
	size_t increaseSpeedHandle = -1;
	size_t decreaseSpeedHandle = -1;
	size_t resetHandle = -1;
	size_t nextHandle = -1;
	//MazeNodeId ratCurr = -1;
	//MazeNodeId ratNext = -1;
	size_t ratIdx = 0;

	PathfindingSceneDrawData currentDrawData;

	void OnFrameUpdate(const float& dT);
	void OnIncreaseSpeedPressed();
	void OnDecreaseSpeedPressed();
	void OnResetPressed();
	void OnNextPressed();
	void UpdateDrawData();

	NodePosDict GetNodePosDict();

	PathResult GetShortestPath();

	

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
	currentDrawData.solution = GetShortestPath();

	currentDrawData.ratPosX = currentDrawData.nodePosDict[0].x;
	currentDrawData.ratPosY = currentDrawData.nodePosDict[0].y;

	ratIdx = 0;
	//ratCurr = currentDrawData.solution[0];
	//ratNext = currentDrawData.solution[1];
	ratAtGoal = false;


}

inline void PathfindingSceneMgr::SetIsActive(const bool& value)
{
	if (isActive == value) { return; }
	isActive = value;
	if (isActive) { tA = 0.0f; }
}

inline void PathfindingSceneMgr::OnFrameUpdate(const float& dT)
{
	if (ratAtGoal) { return; }

	MazeNodeId ratCurr = currentDrawData.solution[ratIdx];
	MazeNodeId ratNext = currentDrawData.solution[ratIdx + 1];

	Vector2 ratPos  = { currentDrawData.ratPosX, currentDrawData.ratPosY };
	Vector2 nextPos = { currentDrawData.nodePosDict[ratNext].x, currentDrawData.nodePosDict[ratNext].y };

	
	float distanceToNext = Vector2DistanceSqr(ratPos, nextPos);
	if (distanceToNext <= CLOSE_ENOUGH) {
		if (ratNext == (MAZE_SIZE * MAZE_SIZE) - 1) {
			ratAtGoal = true;
			return;
		}

		++ratIdx;

	}
	Vector2 dir = Vector2Normalize(Vector2Subtract(nextPos, ratPos));
	Vector2 moveVector = dir * dT * RAT_SPEED;

	Vector2 newPos = Vector2Add(ratPos, moveVector);

	currentDrawData.ratPosX = (int)newPos.x;
	currentDrawData.ratPosY = (int)newPos.y;





	
}
inline void PathfindingSceneMgr::OnIncreaseSpeedPressed() {}
inline void PathfindingSceneMgr::OnDecreaseSpeedPressed() {}

inline void PathfindingSceneMgr::OnResetPressed()
{
	InitializeSceneData();
}

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

inline PathResult PathfindingSceneMgr::GetShortestPath()
{
	// BFS path calculation (fewest edges)
	MazeNodeId start = 0;
	MazeNodeId goal = (MAZE_SIZE * MAZE_SIZE) - 1;

	PathResult result = {};
	MazeGraph maze = currentDrawData.graph;

	if (maze.find(start) == maze.end() || maze.find(start) == maze.end()) {
		std::cerr << "Start or Goal missing from maze\n";
	}

	std::queue<MazeNodeId> q;
	std::unordered_map<MazeNodeId, bool> visited;
	std::unordered_map<MazeNodeId, MazeNodeId> cameFrom;

	visited[start] = true;
	cameFrom[start] = start;
	q.push(start);

	bool found = false;

	
	while (!q.empty() && !found) {

		MazeNodeId current = q.front();
		q.pop();

		auto itAdj = maze.find(current);
		if (itAdj == maze.end()) {
			continue;
		}

		for (const auto& next : itAdj->second) {
			if (visited[next]) {
				continue;
			}
			visited[next] = true;
			cameFrom[next] = current;
			if (next == goal) {
				found = true;
				break;
			}
			q.push(next);
		}
	}

	if (!found) { return result; }

	

	// Reconstruct path: goal -> ... -> start
	std::vector<MazeNodeId> reversed;
	for (MazeNodeId cur = goal;; cur = cameFrom[cur]) {
		reversed.push_back(cur);
		if (cur == start) break;
	}

	std::reverse(reversed.begin(), reversed.end());
	result = std::move(reversed);

	return result;
}