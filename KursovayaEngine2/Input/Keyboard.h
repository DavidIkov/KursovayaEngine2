#pragma once
#include"DLL.h"
#include<vector>
#include"Tools/Event.h"

enum class PressableKeys :unsigned short int {
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

	ENUMEND
};

class Keyboard {

	bool PressableKeysStates[(unsigned short int)PressableKeys::ENUMEND];

	//parameters: bool if it was pressed down or up:true=down,false=up
	EventClass PressableKeysEvents[(unsigned short int)PressableKeys::ENUMEND];

public:

	DLLTREATMENT const EventClass& gPressableKeyEvent(PressableKeys key) const;

	//not for user
	void GLFW_KEYCALLBACK(int key, int scancode, int action, int mods);


	DLLTREATMENT bool gPressableKeyState(PressableKeys key) const;

};
