#include"RenderBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"

using namespace Graphics::Primitives;

RenderBufferClass::RenderBufferClass(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer) {
    glSC(glGenRenderbuffers(1, &ID));
    glSC(glBindRenderbuffer(GL_RENDERBUFFER, ID));
    if (createDepthBuffer and createStencilBuffer) { glSC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height)); }
    else if (createDepthBuffer and not createStencilBuffer) { glSC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height)); }
#if defined Debug
    else if (not createDepthBuffer and createStencilBuffer) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"YOU CANT CREATE RENDER BUFFER WITH STENCIL BUFFER BUT WITHOUT DEPTH BUFFER",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
    else DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"YOU CANT CREATE RENDER BUFFER WITHOUT ANY BUFFERS",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
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
#ifdef Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"TRIED TO DELETE RENDERBUFFER WHEN ITS ALREADY DELETED",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else
#endif
        this->~RenderBufferClass();
}
unsigned int RenderBufferClass::gID() {
#ifdef Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"RENDER BUFFER IS ALREADY DELETED, ACCESSING ITS ID MAY CAUSE PROBLEMS",KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
#endif
    return ID;
}