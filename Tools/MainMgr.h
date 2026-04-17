#pragma once

class MainMgr
{
private:
	// private constructor -- can't create from outside
	MainMgr() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	MainMgr(const MainMgr&) = delete;
	MainMgr& operator=(const MainMgr&) = delete;

	

public:

	// the ONLY way to get the instance
	static MainMgr& Instance() {
		static MainMgr instance; // created once, lives forever


		// subscribe to the mode-switching events


		return instance;
	}

	enum VizMode {
		SORTING,
		PATHFINDING
	};

	VizMode currentVizMode = VizMode::SORTING;

	void SetVizMode(const VizMode& mode)
	{
		if (mode == currentVizMode) { return; }
		currentVizMode = mode;
	}

};