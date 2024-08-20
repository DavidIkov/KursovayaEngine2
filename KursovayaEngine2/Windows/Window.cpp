#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"Window.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"
#include"WindowsManager.h"
#include"Tools/GLDebug.h"
#include<iostream>

bool Window::FirstWindow = true;

void Window::ClearColorBuffer() {
    glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glSC(glClear(GL_COLOR_BUFFER_BIT));
}
void Window::ClearDepthBuffer() {
    glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glSC(glClear(GL_DEPTH_BUFFER_BIT));
}
void Window::ClearStencilBuffer() {
    glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glSC(glClear(GL_STENCIL_BUFFER_BIT));
}
void Window::ClearAllBuffers() {
    glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glSC(glClear(GL_COLOR_BUFFER_BIT));
    glSC(glClear(GL_DEPTH_BUFFER_BIT));
    glSC(glClear(GL_STENCIL_BUFFER_BIT));
}

const Keyboard& Window::gKeyboardHandle() const {
    return KeyboardHandle;
}
const Mouse& Window::gMouseHandle() const {
    return MouseHandle;
}

Window::Window(unsigned int w, unsigned int h, const char* title, bool fullscreen, unsigned int swapInterval) {

    WindowsManager::Windows.push_back(this);

    GLFW_WindowPtr = glfwCreateWindow(w, h, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (!GLFW_WindowPtr) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO CREATE WINDOW", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }

    //TODO this is temporary
    glfwSetWindowAttrib((GLFWwindow*)GLFW_WindowPtr, GLFW_RESIZABLE, false);

    glfwSetWindowUserPointer((GLFWwindow*)GLFW_WindowPtr, (void*)this);

    glfwMakeContextCurrent((GLFWwindow*)GLFW_WindowPtr);

    glfwSwapInterval(swapInterval);

    glfwSetMouseButtonCallback((GLFWwindow*)GLFW_WindowPtr, [](GLFWwindow* window, int button, int action, int mods) {
        ((Window*)glfwGetWindowUserPointer(window))->MouseHandle.GLFW_KEYCALLBACK(button, action, mods);
        });

    glfwSetKeyCallback((GLFWwindow*)GLFW_WindowPtr, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        ((Window*)glfwGetWindowUserPointer(window))->KeyboardHandle.GLFW_KEYCALLBACK(key, scancode, action, mods);
        });

    if (FirstWindow) {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO INITIALIZE GLAD", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_INITIALIZE_LIBRARY });
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
    glfwSetInputMode((GLFWwindow*)GLFW_WindowPtr, GLFW_CURSOR, glfwMode);
    CursorMode = mode;
}
CursorModes Window::gCursorMode() const {
    return CursorMode;
}
bool Window::WindowWaitingToClose() const {
    return glfwWindowShouldClose((GLFWwindow*)GLFW_WindowPtr);
}
void Window::gWindowSize(unsigned int* width, unsigned  int* height) const {
    int w, h;
    glfwGetWindowSize((GLFWwindow*)GLFW_WindowPtr, &w, &h);
    *width = w;
    *height = h;
}
Vector<2> Window::gWindowSize() const {
    int w, h;
    glfwGetWindowSize((GLFWwindow*)GLFW_WindowPtr, &w, &h);
    return Vector<2>((float)w,(float)h);
}
void Window::UpdateMouseData() {

    unsigned int width, height;
    gWindowSize(&width, &height);

    double tx, ty;
    glfwGetCursorPos((GLFWwindow*)GLFW_WindowPtr, &tx, &ty);
    
    Vector<2> recordedPrevPos = MousePosition;

    MousePosition[0] = (float)tx - (float)width / 2;
    MousePosition[1] = (float)height / 2 - (float)ty;

    if (DontUpdateDeltaOnce) {
        MouseDelta = Vector<2>(0.f);
        DontUpdateDeltaOnce = false;
    } else MouseDelta = MousePosition - recordedPrevPos;
}
void Window::gCursorPosition(Vector<2>* pos) const {
    *pos = MousePosition;
}
const Vector<2>& Window::gCursorPosition() const {
    return MousePosition;
}
void Window::gCursorDelta(Vector<2>* delta) const {
    *delta = MouseDelta;
}
const Vector<2>& Window::gCursorDelta() const {
    return MouseDelta;
}
void Window::SwapScreenBuffers() {
    glfwSwapBuffers((GLFWwindow*)GLFW_WindowPtr);
}
void Window::ProcessEvents() {
    glfwPollEvents();
}
void Window::WaitTillEvent() {
    glfwWaitEvents();
}