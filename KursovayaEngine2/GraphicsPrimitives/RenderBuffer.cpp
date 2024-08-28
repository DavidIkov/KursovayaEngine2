#include"RenderBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"

RenderBuffer::RenderBuffer(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer) {
    glSC(glGenRenderbuffers(1, &ID));
    glSC(glBindRenderbuffer(GL_RENDERBUFFER, ID));
    if (createDepthBuffer and createStencilBuffer) { glSC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height)); }
    else if (createDepthBuffer and not createStencilBuffer) { glSC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height)); }
    else if (not createDepthBuffer and createStencilBuffer) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"YOU CANT CREATE RENDER BUFFER WITH STENCIL BUFFER BUT WITHOUT DEPTH BUFFER",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
    else DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"YOU CANT CREATE RENDER BUFFER WITHOUT ANY BUFFERS",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
}
RenderBuffer::RenderBuffer(const RenderBuffer* toCopy) {
    memcpy(this, toCopy, sizeof(RenderBuffer));
    toCopy->Deleted = true;
}
RenderBuffer::RenderBuffer(const RenderBuffer&& toCopy) {
    memcpy(this, &toCopy, sizeof(RenderBuffer));
    toCopy.Deleted = true;
}
void RenderBuffer::operator=(const RenderBuffer&& toCopy) {
    this->~RenderBuffer();
    memcpy(this, &toCopy, sizeof(RenderBuffer));
    toCopy.Deleted = true;
}
RenderBuffer::~RenderBuffer() {
    if (not Deleted) {
        glSC(glDeleteRenderbuffers(1, &ID));
        Deleted = true;
    }
}
void RenderBuffer::Delete() {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"TRIED TO DELETE RENDERBUFFER WHEN ITS ALREADY DELETED",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else this->~RenderBuffer();
}
unsigned int RenderBuffer::gID() const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"RENDER BUFFER IS ALREADY DELETED, ACCESSING ITS ID MAY CAUSE PROBLEMS",KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
    return ID;
}