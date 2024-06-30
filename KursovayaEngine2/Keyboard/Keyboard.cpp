#include"Keyboard.h"
#include<iostream>

bool Keyboard::gPressableKeyState(PressableKeys key) const {
	return PressableKeysStates[(unsigned int)key];
}

Event<bool>& Keyboard::gPressableKeyEvent(PressableKeys key) {
	return PressableKeysEvents[(unsigned int)key];
}

void Keyboard::KeyCallback(int key, int scancode, int action, int mods)
{
	if (key >= GLFW_KEY_A and key <= GLFW_KEY_Z) {
		unsigned int ind = key - GLFW_KEY_A;
		PressableKeysStates[ind] = action != GLFW_RELEASE;
		if (action == GLFW_PRESS) PressableKeysEvents[ind].FireEvent(true);
		else if (action == GLFW_RELEASE) PressableKeysEvents[ind].FireEvent(false);
	}
}

