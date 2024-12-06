#include"RenderBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;

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