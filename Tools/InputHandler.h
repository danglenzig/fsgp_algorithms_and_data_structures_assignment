#pragma once
#include "EventSystem.h"


class InputHandler
{
private:



	// private constructor -- can't create from outside
	InputHandler() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;

public:

	// the ONLY way to get the instance
	static InputHandler& Instance() {
		static InputHandler instance; // created once, lives forever
		return instance;
	}

	void GetInputEvents()
	{
		// if some button is pressed
		//     EventSystem::Instance().NameOfSimpleEvent.Invoke()
	}

};