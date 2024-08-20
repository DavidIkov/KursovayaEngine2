#pragma once
#include"DLL.h"
#include"Input/Keyboard.h"
#include"Input/Mouse.h"
#include"Maths/Vector.h"

enum class CursorModes {
    Free,
    LockedAndInvisible,
};

/*
TODO: dosent support more than one monitors for now(maybe)(i cant fucking test it bruh)
TODO: need to make everything about "window context"
*/

/*
recommended order is:
UpdateMouseData();
render....;
SwapScreenBuffers();
ProcessEvents();
*/
class Window {

    //GLFWwindow*
    void* GLFW_WindowPtr;

    mutable bool DontUpdateDeltaOnce = true;
    mutable Vector<2> MousePosition;
    mutable Vector<2> MouseDelta;
    static bool FirstWindow;//true
    CursorModes CursorMode = CursorModes::Free;

    Keyboard KeyboardHandle;
    Mouse MouseHandle;

public:
    
    DLLTREATMENT const Keyboard& gKeyboardHandle() const;
    DLLTREATMENT const Mouse& gMouseHandle() const;

    DLLTREATMENT void ClearColorBuffer();
    DLLTREATMENT void ClearDepthBuffer();
    DLLTREATMENT void ClearStencilBuffer();
    //color,depth,stencil
    DLLTREATMENT void ClearAllBuffers();

    DLLTREATMENT Window(unsigned int w, unsigned int h, const char* title, bool fullscreen, unsigned int swapInterval);
    DLLTREATMENT bool WindowWaitingToClose() const;
    DLLTREATMENT void UpdateMouseData();
    DLLTREATMENT void SwapScreenBuffers();
    //will proccess events that havent been processed
    DLLTREATMENT void ProcessEvents();
    //will stop thread until some event will occur, usefull to not use 100% of cpu in while loop
    DLLTREATMENT void WaitTillEvent();
    DLLTREATMENT void gWindowSize(unsigned int* width, unsigned  int* height) const;
    DLLTREATMENT Vector<2> gWindowSize() const;
    DLLTREATMENT void gCursorPosition(Vector<2>* pos) const;
    DLLTREATMENT const Vector<2>& gCursorPosition() const;
    DLLTREATMENT void gCursorDelta(Vector<2>* delta) const;
    DLLTREATMENT const Vector<2>& gCursorDelta() const;
    DLLTREATMENT void SetCursorMode(CursorModes mode);
    DLLTREATMENT CursorModes gCursorMode() const;
};