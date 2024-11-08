#include"RenderBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;

#define Assert_NotDeleted_Macro if (Deleted) ErrorsSystemNamespace::SendError<<"RenderBuffer is already deleted">>ErrorsEnumWrapperStruct(ErrorsEnum::AlreadyDeleted);

RenderBufferClass::RenderBufferClass(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer) {
    glSC(glGenRenderbuffers(1, &ID));
    Bind();

    if (createDepthBuffer and createStencilBuffer) { glSC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height)); }
    else if (createDepthBuffer and not createStencilBuffer) { glSC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height)); }
#if defined KE2_Debug
    else if (not createDepthBuffer and createStencilBuffer) ErrorsSystemNamespace::SendError << "cant create RenderBuffer with stencil buffer but without depth buffer" >> ErrorsEnumWrapperStruct(ErrorsEnum::StencilBufferWithoutDepthBuffer);
    else ErrorsSystemNamespace::SendError << "cant create RenderBuffer without any buffers" >> ErrorsEnumWrapperStruct(ErrorsEnum::NoBuffers);
#endif
}
RenderBufferClass::RenderBufferClass(const RenderBufferClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(RenderBufferClass));
    toCopy.Deleted = true;
}
void RenderBufferClass::operator=(const RenderBufferClass&& toCopy) {
    this->~RenderBufferClass();
    memcpy(this, &toCopy, sizeof(RenderBufferClass));
    toCopy.Deleted = true;
}
RenderBufferClass::~RenderBufferClass() {
    if (not Deleted) {
        glSC(glDeleteRenderbuffers(1, &ID));
        Deleted = true;
    }
}
void RenderBufferClass::Delete() {
    Assert_NotDeleted_Macro;
    this->~RenderBufferClass();
}
void RenderBufferClass::Bind() {

    Assert_NotDeleted_Macro;

#if defined KE2_Debug
    BindedInstances.sRenderBuffer_ID(ID);
#endif
    glSC(glBindRenderbuffer(GL_RENDERBUFFER, ID));

}
void RenderBufferClass::Unbind() {
#if defined KE2_Debug
    BindedInstances.sRenderBuffer_ID(0);
#endif
    glSC(glBindRenderbuffer(GL_RENDERBUFFER, 0));


}
unsigned int RenderBufferClass::gID() {
    Assert_NotDeleted_Macro;
    return ID;
}