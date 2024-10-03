#pragma once
#include<iostream>
#include"glad/glad.h"
#if defined KE2_Debug
#define glSC(func)  while (glGetError() != GL_NO_ERROR);\
func;\
{int error = glGetError();\
if (error != GL_NO_ERROR) {\
    std::cout << "Error: " << error << " in file: " << __FILE__ << " in call: " << #func << " at " << __LINE__ << " line" << "\n";\
    __debugbreak();\
}}
#else 
#define glSC(func) func
#endif