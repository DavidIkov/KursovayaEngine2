#include"VertexArray.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"glad/glad.h"
#include"Tools/ErrorCodes.h"

VertexArray::VertexArray() {
    glSC(glGenVertexArrays(1, &ID));
    glSC(glBindVertexArray(ID));
}
VertexArray::VertexArray(const VertexArray* toCopy) {
    memcpy(this, toCopy, sizeof(VertexArray));
    toCopy->Deleted = true;
}
VertexArray::VertexArray(const VertexArray&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexArray));
    toCopy.Deleted = true;
}
void VertexArray::operator=(const VertexArray&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexArray));
    toCopy.Deleted = true;
}
VertexArray::~VertexArray() {
    if (not Deleted) {
        glSC(glDeleteVertexArrays(1, &ID));
        Deleted = true;
    }
}
unsigned int VertexArray::gID() const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"KILL YOURSELF",KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
    return ID;
}
void VertexArray::Delete() {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"ATTEMPING TO DELETE ALREADY DELETED VERTEX ARRAY",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else this->~VertexArray();
}
void VertexArray::Bind() const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX ARRAY IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {
        glSC(glBindVertexArray(ID));
    }
}
void VertexArray::Unbind() {
    glSC(glBindVertexArray(0));
}