#include"VertexArray.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"glad/glad.h"
#include"Tools/ErrorCodes.h"
#include"Tools/DebugRuntimeAssert.h"
#include"Graphics/Globals.h"

using namespace Graphics::Primitives;

#define Assert_NotDeleted_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Deleted, "VertexArray is deleted", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);

VertexArrayClass::VertexArrayClass() {
    glSC(glGenVertexArrays(1, &ID));
    Bind();
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
unsigned int VertexArrayClass::gID() {
    Assert_NotDeleted_Macro;
    return ID;
}
void VertexArrayClass::Delete() {
    Assert_NotDeleted_Macro;
	this->~VertexArrayClass();
}
void VertexArrayClass::Bind() {
    Assert_NotDeleted_Macro;
#if defined KE2_Debug
    BindedInstances.sVertexArray_ID_And_Ptr(ID, this);
#endif
	glSC(glBindVertexArray(ID));
}
void VertexArrayClass::Unbind() {
    glSC(glBindVertexArray(0));
}