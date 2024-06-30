#pragma once
#include"GLFW/glfw3.h"
#include<vector>
#include"Tools/Event.h"

enum class PressableKeys {
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

	ENUMEND
};

class Keyboard {

	bool PressableKeysStates[(unsigned int)PressableKeys::ENUMEND];

	//parameters: bool if it was pressed down or up:true=down,false=up
	Event<bool> PressableKeysEvents[(unsigned int)PressableKeys::ENUMEND];

public:

	Event<bool>& gPressableKeyEvent(PressableKeys key);

	//not for user
	void KeyCallback(int key, int scancode, int action, int mods);


	bool gPressableKeyState(PressableKeys key) const;

};
