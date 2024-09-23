#include"VertexArray.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"glad/glad.h"
#include"Tools/ErrorCodes.h"

using namespace GraphicsPrimitives;

VertexArrayClass::VertexArrayClass() {
    glSC(glGenVertexArrays(1, &ID));
    glSC(glBindVertexArray(ID));
}
VertexArrayClass::VertexArrayClass(RespConstrFlag, const VertexArrayClass& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexArrayClass));
    toCopy.Deleted = true;
}
VertexArrayClass::VertexArrayClass(const VertexArrayClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexArrayClass));
    toCopy.Deleted = true;
}
void VertexArrayClass::operator=(const VertexArrayClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexArrayClass));
    toCopy.Deleted = true;
}
VertexArrayClass::~VertexArrayClass() {
    if (not Deleted) {
        glSC(glDeleteVertexArrays(1, &ID));
        Deleted = true;
    }
}
unsigned int VertexArrayClass::gID() const {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"KILL YOURSELF",KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
#endif
    return ID;
}
void VertexArrayClass::Delete() {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"ATTEMPING TO DELETE ALREADY DELETED VERTEX ARRAY",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else 
#endif
        this->~VertexArrayClass();
}
void VertexArrayClass::Bind() const {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX ARRAY IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else 
#endif
    {
        glSC(glBindVertexArray(ID));
    }
}
void VertexArrayClass::Unbind() {
    glSC(glBindVertexArray(0));
}