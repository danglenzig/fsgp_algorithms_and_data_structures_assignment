#pragma once

#include "../Sorting/SortSceneManager.h"
#include "raylib.h";
#include <string>
#include <iostream>

class RenderSystem
{
private:
	// private constructor -- can't create from outside
	RenderSystem() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;

public:

	// the ONLY way to get the instance
	static RenderSystem& Instance() {
		static RenderSystem instance; // created once, lives forever
		return instance;
	}


	void RenderSortScene(const SortSceneDrawData& drawData)
	{
		std::string titleString = drawData.algorithmString;
		DrawText(
			titleString.c_str(),
			10, 10, 30, BLACK
		);

		// TODO: everything else

	}


};