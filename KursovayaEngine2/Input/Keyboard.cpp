#include"Keyboard.h"
#include"GLFW/glfw3.h"
#include<iostream>

bool KeyboardClass::gPressableKeyState(PressableKeysEnum key) const {
	return PressableKeysStates[(unsigned int)key];
}

const EventClass& KeyboardClass::gPressableKeyEvent(PressableKeysEnum key) const {
	return PressableKeysEvents[(unsigned int)key];
}

void KeyboardClass::GLFW_KEYCALLBACK(int key, int scancode, int action, int mods)
{
	if (key >= GLFW_KEY_A and key <= GLFW_KEY_Z) {
		unsigned int ind = key - GLFW_KEY_A;
		PressableKeysStates[ind] = action != GLFW_RELEASE;
		if (action == GLFW_PRESS) { bool v = true; PressableKeysEvents[ind].FireEvent(&v); }
		else if (action == GLFW_RELEASE) { bool v = false; PressableKeysEvents[ind].FireEvent(&v); }
	}
}

