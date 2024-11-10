#include"VertexArray.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;

#define Assert_NotDeleted_Macro if (Deleted) ErrorsSystemNamespace::SendError<<"VertexArray is already deleted">>ErrorsEnumWrapperStruct(ErrorsEnum::AlreadyDeleted);

VertexArrayClass::VertexArrayClass() {
    glSC(glGenVertexArrays(1, &ID));
    Bind();
}
VertexArrayClass::VertexArrayClass(const VertexArrayClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexArrayClass));
    toCopy.Deleted = true;
}
void VertexArrayClass::operator=(const VertexArrayClass&& toCopy) {
    Delete();
    memcpy(this, &toCopy, sizeof(VertexArrayClass));
    toCopy.Deleted = true;
}
VertexArrayClass::~VertexArrayClass() {
    if (not Deleted) {
        Unbind();
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
    BindedInstances.sVertexArray_ID(ID);
#endif
	glSC(glBindVertexArray(ID));
}
void VertexArrayClass::Unbind() {
    glSC(glBindVertexArray(0));
#if defined KE2_Debug
    BindedInstances.sVertexArray_ID(0);
#endif
}