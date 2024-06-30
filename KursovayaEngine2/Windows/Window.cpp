#include"glad/glad.h"
#include"Window.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"
#include"WindowsManager.h"
#include<iostream>

bool Window::FirstWindow = true;


Window::Window(unsigned int w, unsigned int h, const char* title, bool fullscreen, unsigned int swapInterval) {

    WindowsManager::Windows.push_back(this);

    GLFW_WindowPtr = glfwCreateWindow(w, h, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (!GLFW_WindowPtr) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO CREATE WINDOW", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_CREATE_WINDOW });
    }

    glfwSetWindowUserPointer(GLFW_WindowPtr, (void*)this);

    glfwMakeContextCurrent(GLFW_WindowPtr);

    glfwSwapInterval(swapInterval);

    glfwSetKeyCallback(GLFW_WindowPtr, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Window* wind = (Window*)glfwGetWindowUserPointer(window);
        wind->_Keyboard.KeyCallback(key, scancode, action, mods);
        });

    if (FirstWindow) {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO INITIALIZE GLAD", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_INITIALIZE_GLAD });
        std::cout << "OPENGL VERSION: " << glGetString(GL_VERSION) << std::endl;
    }

    FirstWindow = false;
}
void Window::SetCursorMode(CursorModes mode) {
    unsigned int glfwMode = GLFW_CURSOR_NORMAL;
    switch (mode) {
    case CursorModes::Free: glfwMode = GLFW_CURSOR_NORMAL; break;
    case CursorModes::LockedAndInvisible: glfwMode = GLFW_CURSOR_DISABLED; break;
    }
    DontUpdateDeltaOnce = true;
    glfwSetInputMode(GLFW_WindowPtr, GLFW_CURSOR, glfwMode);
    CursorMode = mode;
}
CursorModes Window::gCursorMode() const {
    return CursorMode;
}
bool Window::WindowWaitingToClose() const {
    return glfwWindowShouldClose(GLFW_WindowPtr);
}
void Window::gWindowSize(unsigned int* width, unsigned  int* height) const {
    int w, h;
    glfwGetWindowSize(GLFW_WindowPtr, &w, &h);
    *width = w;
    *height = h;
}
void Window::StartUpdatingWindow() {

    //update mouse data

    unsigned int width, height;
    gWindowSize(&width, &height);

    double tx, ty;
    glfwGetCursorPos(GLFW_WindowPtr, &tx, &ty);
    
    Vector2 recordedPrevPos = MousePosition;

    MousePosition.sX((float)tx - (float)width / 2);
    MousePosition.sY((float)height / 2 - (float)ty);

    if (DontUpdateDeltaOnce) {
        MouseDelta = Vector2(0, 0);
        DontUpdateDeltaOnce = false;
    } else MouseDelta = MousePosition - recordedPrevPos;
}
void Window::gCursorPosition(Vector2* pos) const {
    *pos = MousePosition;
}
void Window::gCursorDelta(Vector2* delta) const {
    *delta = MouseDelta;
}
void Window::EndUpdatingWindow() {
    glfwSwapBuffers(GLFW_WindowPtr);

    glfwPollEvents();
}