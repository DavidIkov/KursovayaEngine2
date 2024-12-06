#include"KE2_Manager.h"
#include"stb_image/stb_image.h"
#include"GLFW/glfw3.h"

using namespace KE2;
using namespace Manager;

constexpr bool KE2::Manager::KE2_LibraryStruct::_IsKE2_InDebugConfig() noexcept {
#if defined KE2_Debug
    return true;
#else
    return false;
#endif
}
constexpr bool KE2::Manager::KE2_LibraryStruct::_IsKE2_InEfficiencyConfig() noexcept {
#if defined KE2_Efficiency
    return true;
#else
    return false;
#endif
}
constexpr bool KE2::Manager::KE2_LibraryStruct::_IsKE2_In_x86_Architecture() noexcept {
#if defined KE2_x86
    return true;
#else 
    return false;
#endif
}
constexpr bool KE2::Manager::KE2_LibraryStruct::_IsKE2_In_x64_Architecture() noexcept {
#if defined KE2_x64
    return true;
#else
    return false;
#endif
}
constexpr bool KE2::Manager::KE2_LibraryStruct::_IsKE2_OnLinuxPlatform() noexcept {
#if defined KE2_Linux
    return true;
#else
    return false;
#endif
}
constexpr bool KE2::Manager::KE2_LibraryStruct::_IsKE2_OnWindowsPlatform() noexcept {
#if defined KE2_Windows
    return true;
#else
    return false;
#endif
}

static bool Initialized = false;

void KE2::Manager::KE2_LibraryStruct::_InitializeKE2() {

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


void KE2::Manager::KE2_LibraryStruct::_UninitializeKE2() {
    if (Initialized) {
        glfwTerminate();
        Initialized = false;
    }
    else {
        ErrorsSystemNamespace::SendWarning << "Trying to uninitialize KE2 when its already uninitialize" >> ErrorsSystemNamespace::EndOfWarning;
        return;
    }
}