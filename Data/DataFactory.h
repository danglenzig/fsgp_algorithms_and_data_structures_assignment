#pragma once

#include <vector>
#include "structs.h"
#include "raylib.h"

class DataFactory
{
private:
	// private constructor -- can't create from outside
	DataFactory() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	DataFactory(const DataFactory&) = delete;
	DataFactory& operator=(const DataFactory&) = delete;

public:

	// the ONLY way to get the instance
	static DataFactory& Instance() {
		static DataFactory instance; // created once, lives forever
		return instance;
	}


	std::vector<SortBarData> GetSortedBarsList(const int& count = 100) {
		std::vector<SortBarData> barsList = {};

		const int WIDTH = 8;
		const float HEIGHT_MULTIPLIER = 4.0f;

		for (int i = 0; i < count; i++) {
			Color color = GOLD;
			int height = (int)((float)i * HEIGHT_MULTIPLIER);
			SortBarData data = SortBarData(color, height, WIDTH);
			barsList.push_back(data);
		}

		return barsList;
	}

};