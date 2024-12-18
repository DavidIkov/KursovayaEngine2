#include"RenderBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;


unsigned int RenderBufferClass::_BufferDataFormat_SwitchCase(BufferDataFormatEnum bufferDataFormat) noexcept {
	switch (bufferDataFormat) {
    case BufferDataFormatEnum::Depth: return GL_DEPTH_COMPONENT24;
    case BufferDataFormatEnum::DepthStencil: return GL_DEPTH24_STENCIL8;
    case BufferDataFormatEnum::Stencil: return GL_STENCIL_INDEX8;
    case BufferDataFormatEnum::Red: return GL_R8;
    case BufferDataFormatEnum::RG: return GL_RG8;
    case BufferDataFormatEnum::RGB: return GL_RGB8;
    case BufferDataFormatEnum::RGBA: return GL_RGBA8;
    default: return 0;
    }
}
RenderBufferClass::RenderBufferClass(unsigned int width, unsigned int height, BufferDataFormatEnum bufferDataFormat) {
    glSC(glGenRenderbuffers(1, &ID));
    Bind();
    glSC(glRenderbufferStorage(GL_RENDERBUFFER, _BufferDataFormat_SwitchCase(bufferDataFormat), width, height));
}
RenderBufferClass::RenderBufferClass(RenderBufferClass&& toCopy) noexcept :
    ID(toCopy.ID) {
    toCopy.ID = 0u;
}
RenderBufferClass& RenderBufferClass::operator=(RenderBufferClass&& toCopy) {
    this->~RenderBufferClass();
    new(this) RenderBufferClass(std::move(toCopy));
    return *this;
}
RenderBufferClass::~RenderBufferClass() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteRenderbuffers(1, &ID));
        ID = 0u;
    }
}
void RenderBufferClass::Bind() const {

#if defined KE2_Debug
    BindedInstances.sRenderBufferID(ID);
#endif
    glSC(glBindRenderbuffer(GL_RENDERBUFFER, ID));

}
void RenderBufferClass::Unbind() {
#if defined KE2_Debug
    BindedInstances.sRenderBufferID(0);
#endif
    glSC(glBindRenderbuffer(GL_RENDERBUFFER, 0));


}