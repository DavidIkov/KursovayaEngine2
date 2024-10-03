#include"RenderBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"
#include"Graphics/Globals.h"
#include"Tools/DebugRuntimeAssert.h"

using namespace Graphics::Primitives;

#define Assert_NotDeleted_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Deleted, "RenderBuffer is deleted", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);

RenderBufferClass::RenderBufferClass(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer) {
    glSC(glGenRenderbuffers(1, &ID));
    Bind();

    if (createDepthBuffer and createStencilBuffer) { glSC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height)); }
    else if (createDepthBuffer and not createStencilBuffer) { glSC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height)); }
#if defined KE2_Debug
    else if (not createDepthBuffer and createStencilBuffer) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"cant create RenderBuffer with stencil buffer but without depth buffer",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
    else DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"cant create RenderBuffer without any buffers",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
#endif
}
RenderBufferClass::RenderBufferClass(RespConstrFlag, const RenderBufferClass& toCopy) {
    memcpy(this, &toCopy, sizeof(RenderBufferClass));
    toCopy.Deleted = true;
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
unsigned int RenderBufferClass::gID() {
    Assert_NotDeleted_Macro;
    return ID;
}