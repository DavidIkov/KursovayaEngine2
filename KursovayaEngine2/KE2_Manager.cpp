#include"KE2_Manager.h"
#include"Tools/DebuggingTools.h"
#include"stb_image/stb_image.h"
#include"GLFW/glfw3.h"


constexpr bool _IsKE2_InDebugConfig() {
#if defined KE2_Debug
    return true;
#else
    return false;
#endif
}
constexpr bool _IsKE2_InEfficiencyConfig() {
#if defined KE2_Efficiency
    return true;
#else
    return false;
#endif
}
constexpr bool _IsKE2_In_x86_Platform() {
#if defined KE2_x86
    return true;
#else 
    return false;
#endif
}
constexpr bool _IsKE2_In_x64_Platform() {
#if defined KE2_x64
    return true;
#else
    return false;
#endif
}

bool Initialized = false;


KURSAVAYAENGINE2_CORE_ERRORS _InitializeKE2() {

    if (Initialized) return KURSAVAYAENGINE2_CORE_ERRORS::NONE;

    static_assert(sizeof(unsigned char) == 1, "cant initialize KursovayaEngine2 since sizeof(unsigned char) is not 1");
	
    try {
        stbi_set_flip_vertically_on_load(true);

        if (!glfwInit()) {
            DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO INITIALIZE GLFW", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_INITIALIZE_LIBRARY });
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        Initialized = true;
        return KURSAVAYAENGINE2_CORE_ERRORS::NONE;
    }
    catch (KURSAVAYAENGINE2_CORE_ERRORS& err) { return err; }
    catch (...) { return KURSAVAYAENGINE2_CORE_ERRORS::UNKNOWN; }
    
}


void UninitializeKE2() {
    if (not Initialized) {
        glfwTerminate();
        Initialized = false;
    }
}