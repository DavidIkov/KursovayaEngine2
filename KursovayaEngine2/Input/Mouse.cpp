#include"Mouse.h"
#include"GLFW/glfw3.h"
#include<iostream>



const EventClass& Mouse::gMouseInputKeyEvent(MouseInputKeys key) const {
	return MouseInputKeysEvents[(unsigned int)key];
}

//not for user
void Mouse::GLFW_KEYCALLBACK(int button, int action, int mods) {
	if (button >= GLFW_MOUSE_BUTTON_LEFT and button <= GLFW_MOUSE_BUTTON_MIDDLE) {
		unsigned int ind = button - GLFW_MOUSE_BUTTON_LEFT;
		MouseInputKeysStates[ind] = action != GLFW_RELEASE;
		if (action == GLFW_PRESS) { bool v = true; MouseInputKeysEvents[ind].FireEvent(&v); }
		else if (action == GLFW_RELEASE) { bool v = false; MouseInputKeysEvents[ind].FireEvent(&v); }
	}
}


bool Mouse::gMouseInputKeyState(MouseInputKeys key) const {
	return MouseInputKeysStates[(unsigned int)key];
}