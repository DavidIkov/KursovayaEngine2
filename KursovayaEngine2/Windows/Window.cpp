#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"Window.h"
#include"WindowsManager.h"
#include"Tools/GLDebug.h"
#include<iostream>

using namespace KE2;

bool WindowClass::FirstWindow = true;

void WindowClass::ClearColorBuffer() {
    glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glSC(glClear(GL_COLOR_BUFFER_BIT));
}
void WindowClass::ClearDepthBuffer() {
    glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glSC(glClear(GL_DEPTH_BUFFER_BIT));
}
void WindowClass::ClearStencilBuffer() {
    glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glSC(glClear(GL_STENCIL_BUFFER_BIT));
}
void WindowClass::ClearAllBuffers() {
    glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glSC(glClear(GL_COLOR_BUFFER_BIT));
    glSC(glClear(GL_DEPTH_BUFFER_BIT));
    glSC(glClear(GL_STENCIL_BUFFER_BIT));
}

const KeyboardClass& WindowClass::gKeyboardHandle() const {
    return KeyboardHandle;
}
const MouseClass& WindowClass::gMouseHandle() const {
    return MouseHandle;
}

WindowClass::WindowClass(unsigned int* w, unsigned int* h, const char* title, bool fullscreen, unsigned int swapInterval) {
    
    WindowsManager::Windows.push_back(this);

    glfwSetErrorCallback(+[](int code, const char* msg) {
        ErrorsSystemNamespace::SendError << "GLFW error with code [" << std::to_string(code) << "] and message: [" << msg << "]" >> GLFW_ErrorWrapperStruct(code);
        });

    GLFW_WindowPtr = glfwCreateWindow(*w, *h, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

    
    gWindowSize(w, h);

    //TODO this is temporary
    glfwSetWindowAttrib((GLFWwindow*)GLFW_WindowPtr, GLFW_RESIZABLE, false);

    glfwSetWindowUserPointer((GLFWwindow*)GLFW_WindowPtr, (void*)this);

    glfwMakeContextCurrent((GLFWwindow*)GLFW_WindowPtr);

    glfwSwapInterval(swapInterval);

    glfwSetMouseButtonCallback((GLFWwindow*)GLFW_WindowPtr, [](GLFWwindow* window, int button, int action, int mods) {
        ((WindowClass*)glfwGetWindowUserPointer(window))->MouseHandle._GLFW_KEYCALLBACK(button, action, mods);
        });

    glfwSetKeyCallback((GLFWwindow*)GLFW_WindowPtr, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        ((WindowClass*)glfwGetWindowUserPointer(window))->KeyboardHandle._GLFW_KEYCALLBACK(key, scancode, action, mods);
        });

    if (FirstWindow) {
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        std::cout << "OPENGL VERSION: " << glGetString(GL_VERSION) << std::endl;
    }

    FirstWindow = false;
}
void WindowClass::SetCursorMode(CursorModes mode) {
    unsigned int glfwMode = GLFW_CURSOR_NORMAL;
    switch (mode) {
    case CursorModes::Free: glfwMode = GLFW_CURSOR_NORMAL; break;
    case CursorModes::LockedAndInvisible: glfwMode = GLFW_CURSOR_DISABLED; break;
    }
    DontUpdateDeltaOnce = true;
    glfwSetInputMode((GLFWwindow*)GLFW_WindowPtr, GLFW_CURSOR, glfwMode);
    CursorMode = mode;
}
CursorModes WindowClass::gCursorMode() const {
    return CursorMode;
}
bool WindowClass::WindowWaitingToClose() const {
    return glfwWindowShouldClose((GLFWwindow*)GLFW_WindowPtr);
}
void WindowClass::gWindowSize(unsigned int* width, unsigned  int* height) const {
    int w, h;
    glfwGetWindowSize((GLFWwindow*)GLFW_WindowPtr, &w, &h);
    *width = w;
    *height = h;
}
Vector2U WindowClass::gWindowSize() const {
    int w, h;
    glfwGetWindowSize((GLFWwindow*)GLFW_WindowPtr, &w, &h);
    return Vector2U(w, h);
}
void WindowClass::UpdateMouseData() {

    unsigned int width, height;
    gWindowSize(&width, &height);

    double tx, ty;
    glfwGetCursorPos((GLFWwindow*)GLFW_WindowPtr, &tx, &ty);
    
    Vector2I recordedPrevPos = MousePosition;

    MousePosition[0] = (int)tx - width / 2;
    MousePosition[1] = height / 2 - (int)ty;

    if (DontUpdateDeltaOnce) {
        MouseDelta = Vector2I(0);
        DontUpdateDeltaOnce = false;
    } else MouseDelta = MousePosition - recordedPrevPos;
}
void WindowClass::gCursorPosition(Vector2I* pos) const {
    *pos = MousePosition;
}
const Vector2I& WindowClass::gCursorPosition() const {
    return MousePosition;
}
void WindowClass::gCursorDelta(Vector2I* delta) const {
    *delta = MouseDelta;
}
const Vector2I& WindowClass::gCursorDelta() const {
    return MouseDelta;
}
void WindowClass::SwapScreenBuffers() {
    glfwSwapBuffers((GLFWwindow*)GLFW_WindowPtr);
}
void WindowClass::ProcessEvents() {
    glfwPollEvents();
}
void WindowClass::WaitTillEvent() {
    glfwWaitEvents();
}

void WindowClass::Destroy() {
    this->~WindowClass();
}
WindowClass::~WindowClass() {
    if (not Deleted) {
        glfwDestroyWindow((GLFWwindow*)GLFW_WindowPtr);
        Deleted = true;
    }
}

WindowClass::MonitorDataStruct WindowClass::gPrimaryMonitorData() {
    const GLFWvidmode* data = glfwGetVideoMode(glfwGetPrimaryMonitor());
    return MonitorDataStruct{ Vector2U((unsigned int)data->width,(unsigned int)data->height),
        Vector3U((unsigned int)data->redBits,(unsigned int)data->greenBits,(unsigned int)data->blueBits),(unsigned int)data->refreshRate };
}