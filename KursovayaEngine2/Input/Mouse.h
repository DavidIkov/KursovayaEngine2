#pragma once
#include"DLL.h"
#include"Tools/Event.h"
#include"Tools/BoolsTable.h"


class MouseClass {

public:
	enum class PressableKeysEnum :unsigned short int {
		Left, Right, Middle,

		ENUMEND
	};
private:

	BoolsTableClass<(size_t)PressableKeysEnum::ENUMEND> PressableKeysStates;

	//parameters: bool if it was pressed down or up:true=down,false=up
	EventClass PressableKeysEvents[(unsigned short int)PressableKeysEnum::ENUMEND];

public:

	DLLTREATMENT const EventClass& gMousePressableKeyEvent(PressableKeysEnum key) const;

	//not for user
	void _GLFW_KEYCALLBACK(int button, int action, int mods);


	DLLTREATMENT bool gMousePressableKeyState(PressableKeysEnum key) const;
};