#pragma once
#include"DLL.h"
#include"Tools/Event.h"

enum class MouseInputKeys {
    Left, Right, Middle,

	ENUMEND
};

class Mouse {

	bool MouseInputKeysStates[(unsigned int)MouseInputKeys::ENUMEND];

	//parameters: bool if it was pressed down or up:true=down,false=up
	EventClass MouseInputKeysEvents[(unsigned int)MouseInputKeys::ENUMEND];

public:

	DLLTREATMENT const EventClass& gMouseInputKeyEvent(MouseInputKeys key) const;

	//not for user
	void GLFW_KEYCALLBACK(int button, int action, int mods);


	DLLTREATMENT bool gMouseInputKeyState(MouseInputKeys key) const;
};