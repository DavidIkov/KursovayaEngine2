#include"KE2_Manager.h"
#include"stb_image/stb_image.h"
#include"GLFW/glfw3.h"

using namespace KE2;
using namespace Manager;

constexpr bool KE2::Manager::_IsKE2_InDebugConfig() {
#if defined KE2_Debug
    return true;
#else
    return false;
#endif
}
constexpr bool KE2::Manager::_IsKE2_InEfficiencyConfig() {
#if defined KE2_Efficiency
    return true;
#else
    return false;
#endif
}
constexpr bool KE2::Manager::_IsKE2_In_x86_Architecture() {
#if defined KE2_x86
    return true;
#else 
    return false;
#endif
}
constexpr bool KE2::Manager::_IsKE2_In_x64_Architecture() {
#if defined KE2_x64
    return true;
#else
    return false;
#endif
}
constexpr bool KE2::Manager::_IsKE2_OnLinuxPlatform() {
#if defined KE2_Linux
    return true;
#else
    return false;
#endif
}
constexpr bool KE2::Manager::_IsKE2_OnWindowsPlatform() {
#if defined KE2_Windows
    return true;
#else
    return false;
#endif
}

static bool Initialized = false;

void KE2::Manager::_InitializeKE2() {

    if (Initialized) {
        ErrorsSystemNamespace::SendWarning << "Trying to initialize KE2 when its already initialized" >> ErrorsSystemNamespace::EndOfWarning;
        return;
    }

	stbi_set_flip_vertically_on_load(true);

	if (!glfwInit()) {
		ErrorsSystemNamespace::SendError << "Failed to initialize GLFW" >> ErrorsEnumWrapperStruct(ErrorsEnum::FailedToInitializeGLFW);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	Initialized = true;
    
}


void KE2::Manager::UninitializeKE2() {
    if (not Initialized) {
        glfwTerminate();
        Initialized = false;
    }
}