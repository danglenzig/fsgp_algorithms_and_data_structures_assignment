#pragma once

#include "../Sorting/SortSceneManager.h"
#include "raylib.h";
#include <string>
#include <iostream>
#include <iomanip>
#include <format>
#include <algorithm>
#include "../Data/structs.h"

class RenderSystem
{
private:
	// private constructor -- can't create from outside
	RenderSystem() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;

	std::string sortModeControlsStr = "Up/Down Arrows : step time      R: reset      N : next sorting algorithm      P : pathfinding      ESC : quit";
	std::string pfModeControlsStr   = "                  R: reset      N : next sorting algorithm      S : sorting          ESC : quit";

	const Color UI_RED = { 224,0,0,255 };

	void DrawMaze(const PathfindingSceneDrawData&);

public:

	// the ONLY way to get the instance
	static RenderSystem& Instance() {
		static RenderSystem instance; // created once, lives forever
		return instance;
	}


	void RenderSortScene(const SortSceneDrawData& drawData)
	{
		DrawText(
			sortModeControlsStr.c_str(),
			100, 670, 20, UI_RED
		);

		std::string titleString = drawData.algorithmString;
		DrawText(
			titleString.c_str(),
			10, 10, 30, BLACK
		);

		float stepInverval = drawData.stepInterval;
		int stepCt = drawData.stepCount;
		int comparisons = drawData.comparisons;
		int swaps = drawData.swaps;
		std::string stateStr = std::format(
			"step time: {:.2f}s, list size: {}, comparisons: {}, swaps: {}",
			stepInverval, drawData.barsList.size(), comparisons, swaps
		);
		DrawText(
			stateStr.c_str(),
			10, 50, 20, BLACK
		);

		DrawText(
			drawData.additionalInfoStr.c_str(),
			10, 80, 20, BLACK
		); 

		const int START_X = 40;
		const int SPACING = 10;
		//const int POS_Y = 100;
		int count = drawData.barsList.size();

		for (int i = 0; i < count; i++) {
			SortBarData barData = drawData.barsList[i];
			
			int width = barData.width;
			int height = barData.height;

			int posX = START_X + (i * SPACING);
			int posY = 600 - height;

			Color color = barData.color;
			DrawRectangle(posX, posY, width, height, color);
		}
	}

	void RenderPathfindingScene(const PathfindingSceneDrawData& data)
	{
		//std::cout << "TODO: Pathfinding Scene Rendering\n";
		DrawText(
			pfModeControlsStr.c_str(),
			100, 670, 20, GOLD
		);
		DrawMaze(data);
	}

};

inline void RenderSystem::DrawMaze(const PathfindingSceneDrawData& data)
{
	NodePosDict posDict = data.nodePosDict;
	MazeGraph graph = data.graph;
	size_t nodeCount = data.mazeSize * data.mazeSize;
	int startTextposX = data.startTextPosX - 100;
	int startTextPosY = data.startTextPosY - 15;

	std::vector<MazeNodeId> visited = {};

	


	for (size_t i = 0; i < nodeCount; ++i) {
		visited.push_back(i);

		float thisNodeX = posDict[i].x;
		float thisNodeY = posDict[i].y;
		Vector2 thisNodePos = { thisNodeX, thisNodeY };

		std::vector<MazeNodeId> neighbors = graph[i];
		for (MazeNodeId neighbor : neighbors) {
			// don't draw lines to already-visited neighbors
			if (std::find(visited.begin(), visited.end(), neighbor) != visited.end()) {
				continue;
			}
			float neighborX = posDict[neighbor].x;
			float neighborY = posDict[neighbor].y;
			Vector2 neighborPos = { neighborX, neighborY };
			DrawLineEx(thisNodePos, neighborPos, 20, DARKGRAY);
		}
	}

	int startX = posDict[0].x;
	int startY = posDict[0].y;
	int goalX = posDict[nodeCount - 1].x;
	int goalY = posDict[nodeCount - 1].y;
	DrawCircle(startX, startY, 12, { 128,255,0,255 });
	DrawCircle(goalX, goalY, 12, { 128,255,0,255 });

	

	/*DrawText(
		"START",
		startTextposX, startTextPosY, 30, {128,128,0,255}
	);*/

}