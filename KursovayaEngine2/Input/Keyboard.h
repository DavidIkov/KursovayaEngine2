#pragma once
#include"DLL.h"
#include"Tools/Event.h"
#include"Tools/BoolsTable.h"


class KeyboardClass {

public:
	enum class PressableKeysEnum :unsigned short int {
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

		LeftShift, LeftControl, LeftAlt, LeftSuper, RightShift, RightControl, RightAlt, RightSuper,

		ENUMEND
	};
private:

	BoolsTableClass<(size_t)PressableKeysEnum::ENUMEND> PressableKeysStates;

	//parameters: bool if it was pressed down or up:true=down,false=up
	EventClass PressableKeysEvents[(unsigned short int)PressableKeysEnum::ENUMEND];

public:

	DLLTREATMENT const EventClass& gPressableKeyEvent(PressableKeysEnum key) const;

	//not for user
	void _GLFW_KEYCALLBACK(int key, int scancode, int action, int mods);


	DLLTREATMENT bool gPressableKeyState(PressableKeysEnum key) const;

};
