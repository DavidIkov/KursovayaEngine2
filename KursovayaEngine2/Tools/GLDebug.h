#pragma once
#include<iostream>
#include"glad/glad.h"
#include"Tools/ErrorsSystem.h"

struct OpenGL_AnyErrorWrapperStruct : KE2::ErrorsSystemNamespace::ErrorBase {
    unsigned int ErrorGL_Enum;
    inline OpenGL_AnyErrorWrapperStruct(unsigned int errorGL_Enum) :ErrorGL_Enum(errorGL_Enum) {};
}; using OpenGL_AnyError = OpenGL_AnyErrorWrapperStruct;

#if defined KE2_Debug
#define glSC(func)  while (glGetError() != GL_NO_ERROR);\
func;\
{unsigned int error = glGetError();\
if (error != GL_NO_ERROR) KE2::ErrorsSystemNamespace::SendError<<"OpenGL error, code: ["<<std::to_string(error)<<"]">>OpenGL_AnyErrorWrapperStruct(error);\
}
#else 
#define glSC(func) func
#endif