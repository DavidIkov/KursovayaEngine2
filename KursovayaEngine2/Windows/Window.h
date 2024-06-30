#pragma once
#include"GLFW/glfw3.h"
#include"Keyboard/Keyboard.h"
#include"Maths/Vector2.h"

enum class CursorModes {
    Free,
    LockedAndInvisible,
};

/*
dosent support more than one monitors for now(maybe)(i cant fucking test it bruh)
need to make everything about "window context"
*/
class Window {

    GLFWwindow* GLFW_WindowPtr;

    mutable bool DontUpdateDeltaOnce = true;
    mutable Vector2 MousePosition;
    mutable Vector2 MouseDelta;
    static bool FirstWindow;//true
    CursorModes CursorMode = CursorModes::Free;

public:
    Keyboard _Keyboard;

    Window(unsigned int w, unsigned int h, const char* title, bool fullscreen, unsigned int swapInterval);
    bool WindowWaitingToClose() const;
    void StartUpdatingWindow();
    void EndUpdatingWindow();
    void gWindowSize(unsigned int* width, unsigned  int* height) const;
    void gCursorPosition(Vector2* pos) const;
    void gCursorDelta(Vector2* delta) const;
    void SetCursorMode(CursorModes mode);
    CursorModes gCursorMode() const;
};