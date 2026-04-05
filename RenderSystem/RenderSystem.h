#pragma once

#include "../Sorting/SortSceneManager.h"
#include "raylib.h";
#include <string>
#include <iostream>
#include <iomanip>
#include <format>
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

	const Color UI_RED = { 224,0,0,255 };

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


};