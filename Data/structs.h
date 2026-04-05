#pragma once

struct SortBarData {
	Color color;
	int height;
	int width;
	SortBarData(
		Color _color,
		int _height,
		int _width
	) {
		color = _color;
		height = _height;
		width = _width;
	}
};

struct SortSceneDrawData {
	std::string algorithmString;
	float stepInterval;
	int stepCount;
	int comparisons;
	int swaps;
	std::vector<SortBarData> barsList;
	std::string additionalInfoStr;
	
	SortSceneDrawData(
		std::string _algStr,
		float _stepInterval,
		int _stepCount,
		int _comparisons,
		int _swaps,
		std::vector<SortBarData> _bars,
		std::string _additionalInfoStr
	) {
		algorithmString = _algStr;
		stepInterval = _stepInterval;
		stepCount = _stepCount;
		comparisons = _comparisons;
		swaps = _swaps;
		barsList = _bars;
		additionalInfoStr = _additionalInfoStr;
	}

	// default constructor with blank data
	SortSceneDrawData() {
		algorithmString = "";
		stepInterval = -1.0f;
		stepCount = -1;
		comparisons = -1;
		barsList = {};
	}
};