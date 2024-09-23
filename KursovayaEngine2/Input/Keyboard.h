#pragma once
#include"DLL.h"
#include<vector>
#include"Tools/Event.h"


class KeyboardClass {

public:
	enum class PressableKeysEnum :unsigned short int {
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

		ENUMEND
	};
private:

	bool PressableKeysStates[(unsigned short int)PressableKeysEnum::ENUMEND];

	//parameters: bool if it was pressed down or up:true=down,false=up
	EventClass PressableKeysEvents[(unsigned short int)PressableKeysEnum::ENUMEND];

public:

	DLLTREATMENT const EventClass& gPressableKeyEvent(PressableKeysEnum key) const;

	//not for user
	void GLFW_KEYCALLBACK(int key, int scancode, int action, int mods);


	DLLTREATMENT bool gPressableKeyState(PressableKeysEnum key) const;

};
