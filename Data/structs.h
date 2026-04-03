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
	std::vector<SortBarData> barsList;
	
	SortSceneDrawData(
		std::string _algStr,
		float _stepInterval,
		int _stepCount,
		int _comparisons,
		std::vector<SortBarData> _bars
	) {
		algorithmString = _algStr;
		stepInterval = _stepInterval;
		stepCount = _stepCount;
		comparisons = _comparisons;
		barsList = _bars;
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