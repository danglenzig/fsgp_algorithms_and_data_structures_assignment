#pragma once
#include "EventSystem.h"
#include "raylib.h"


class InputHandler
{
private:



	// private constructor -- can't create from outside
	InputHandler() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;


	bool quitJustPressed = false;

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
		if (IsKeyPressed(KEY_ESCAPE)) {
			quitJustPressed = true;
			//EventSystem::Instance().QuitPressed.Invoke();
			return;
		}
		if (IsKeyPressed(KEY_UP)) {
			EventSystem::Instance().IncreaseStepSpeedPressed.Invoke();
			return;
		}
		if (IsKeyPressed(KEY_DOWN)) {
			EventSystem::Instance().DecreaseStepSpeedPressed.Invoke();
			return;
		}
		if (IsKeyPressed(KEY_R)) {
			EventSystem::Instance().ResetPressed.Invoke();
		}
		if (IsKeyPressed(KEY_N)) {
			EventSystem::Instance().NextPressed.Invoke();
		}
	}
	bool GetQuitJustPressed() { return quitJustPressed; }

};