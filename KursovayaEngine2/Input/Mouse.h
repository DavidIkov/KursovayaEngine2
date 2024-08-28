#pragma once
#include"DLL.h"
#include"Tools/Event.h"

enum class MouseInputKeys :unsigned short int {
    Left, Right, Middle,

	ENUMEND
};

class Mouse {

	bool MouseInputKeysStates[(unsigned short int)MouseInputKeys::ENUMEND];

	//parameters: bool if it was pressed down or up:true=down,false=up
	EventClass MouseInputKeysEvents[(unsigned short int)MouseInputKeys::ENUMEND];

public:

	DLLTREATMENT const EventClass& gMouseInputKeyEvent(MouseInputKeys key) const;

	//not for user
	void GLFW_KEYCALLBACK(int button, int action, int mods);


	DLLTREATMENT bool gMouseInputKeyState(MouseInputKeys key) const;
};