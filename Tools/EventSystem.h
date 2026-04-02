#pragma once
#include "Event.h"

// Define payload structs here

class EventSystem
{
private:
	// private constructor -- can't create from outside
	EventSystem() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	EventSystem(const EventSystem&) = delete;
	EventSystem& operator=(const EventSystem&) = delete;

public:

	// the ONLY way to get the instance
	static EventSystem& Instance() {
		static EventSystem instance; // created once, lives forever
		return instance;
	}


	// Input Events
	SimpleEvent QuitPressed;

};