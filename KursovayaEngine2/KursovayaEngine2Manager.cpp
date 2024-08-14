#include"KursovayaEngine2Manager.h"
#include"Tools/DebuggingTools.h"
#include"stb_image/stb_image.h"
#include"GLFW/glfw3.h"


KURSAVAYAENGINE2_CORE_ERRORS InitializeKursovayaEngine2() {
	
    try {
        stbi_set_flip_vertically_on_load(true);

        if (!glfwInit()) {
            DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO INITIALIZE GLFW", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_INITIALIZE_LIBRARY });
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        return KURSAVAYAENGINE2_CORE_ERRORS::NONE;
    }
    catch (KURSAVAYAENGINE2_CORE_ERRORS& err) { return err; }
    catch (...) { return KURSAVAYAENGINE2_CORE_ERRORS::UNKNOWN; }
    
}


void UninitializeKursovayaEngine2() {

    glfwTerminate();
}