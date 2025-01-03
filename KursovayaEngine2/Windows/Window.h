#pragma once
#include"DLL.h"
#include"Input/Keyboard.h"
#include"Input/Mouse.h"
#include"Maths/Vector.h"
#include"Tools/ErrorsSystem.h"

namespace KE2::Window {

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
    class WindowClass {

    public:
        void* GLFW_WindowPtr;//GLFWwindow*

		enum class CursorModes {
			Free,
			LockedAndInvisible,
		};
    private:

        bool Deleted = false;

        mutable bool DontUpdateDeltaOnce = true;
        mutable Vector2I MousePosition;
        mutable Vector2I MouseDelta;
        static bool FirstWindow;//true
        CursorModes CursorMode = CursorModes::Free;

        KeyboardClass KeyboardHandle;
        MouseClass MouseHandle;

    public:

        struct GLFW_ErrorWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
            unsigned int ErrCode;
            inline GLFW_ErrorWrapperStruct(unsigned int errCode) :ErrCode(errCode) {};
        }; using GLFW_AnyError = GLFW_ErrorWrapperStruct;

        DLLTREATMENT const KeyboardClass& gKeyboardHandle() const;
        DLLTREATMENT const MouseClass& gMouseHandle() const;

        DLLTREATMENT void ClearColorBuffer();
        DLLTREATMENT void ClearDepthBuffer();
        DLLTREATMENT void ClearStencilBuffer();
        //color,depth,stencil
        DLLTREATMENT void ClearAllBuffers();

        //will change width and height to actual window width and height
        DLLTREATMENT WindowClass(unsigned int* w, unsigned int* h, const char* title, bool fullscreen, int vsyncValue);
        DLLTREATMENT bool WindowWaitingToClose() const;
        DLLTREATMENT void UpdateMouseData();
        DLLTREATMENT void SwapScreenBuffers();
        //will proccess events that havent been processed
        DLLTREATMENT void ProcessEvents();
        //will stop thread until some event will occur, usefull to not use 100% of cpu in while loop
        DLLTREATMENT void WaitTillEvent();
        DLLTREATMENT void gWindowSize(unsigned int* width, unsigned  int* height) const;
        DLLTREATMENT Vector2U gWindowSize() const;
        DLLTREATMENT void gCursorPosition(Vector2I* pos) const;
        DLLTREATMENT const Vector2I& gCursorPosition() const;
        DLLTREATMENT void gCursorDelta(Vector2I* delta) const;
        DLLTREATMENT const Vector2I& gCursorDelta() const;
        DLLTREATMENT void SetCursorMode(CursorModes mode);
        DLLTREATMENT CursorModes gCursorMode() const;

        DLLTREATMENT void Destroy();
        DLLTREATMENT ~WindowClass();

        struct MonitorDataStruct {
            Vector2U Size;
            Vector3U ColorDepth;
            unsigned int RefreshRate;//in Hz
        };
        static DLLTREATMENT MonitorDataStruct gPrimaryMonitorData();
    };
}