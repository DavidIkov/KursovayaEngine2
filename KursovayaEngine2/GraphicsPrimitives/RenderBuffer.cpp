#include"RenderBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"

RenderBuffer::RenderBuffer(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer):DepthBufferEnabled(createDepthBuffer),StencilBufferEnabled(createStencilBuffer) {
    glSC(glGenRenderbuffers(1, &ID));
    glSC(glBindRenderbuffer(GL_RENDERBUFFER, ID));
    if (createDepthBuffer and createStencilBuffer) { glSC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height)) }
    else if (createDepthBuffer and not createStencilBuffer) { glSC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height)) }
    else if (not createDepthBuffer and createStencilBuffer) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"YOU CANT CREATE RENDER BUFFER WITH STENCIL BUFFER BUT WITHOUT DEPTH BUFFER",KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_CREATE_RENDERBUFFER_WITHOUT_ANY_BUFFERS });
    else DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"YOU CANT CREATE RENDER BUFFER WITHOUT ANY BUFFERS",KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_CREATE_RENDERBUFFER_WITHOUT_ANY_BUFFERS });
    glSC(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}
RenderBuffer::~RenderBuffer() {
    if (not Deleted) {
        glSC(glDeleteRenderbuffers(1, &ID));
        Deleted = true;
    }
}
void RenderBuffer::Delete() {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable,"TRIED TO DELETE RENDERBUFFER WHEN ITS ALREADY DELETED",KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_DELETE_ALREADY_DELETED_RENDERBUFFER });
    else this->~RenderBuffer();
}
unsigned int RenderBuffer::gID() const {
    return ID;
}
bool RenderBuffer::gIsDepthBufferEnabled() const {
    return DepthBufferEnabled;
}
bool RenderBuffer::gIsStencilBufferEnabled() const {
    return StencilBufferEnabled;
}