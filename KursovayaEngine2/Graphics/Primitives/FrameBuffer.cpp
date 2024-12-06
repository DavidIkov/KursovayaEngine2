#include"FrameBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;

#if defined KE2_Debug
#define Assert_Finished_Macro if (not Finished) ErrorsSystemNamespace::SendError<<"FrameBuffer is not finished">>\
ErrorsEnumWrapperStruct(ErrorsEnum::NotFinished);
#define Assert_NotFinished_Macro if (Finished) ErrorsSystemNamespace::SendError<<"FrameBuffer is already finished">>\
ErrorsEnumWrapperStruct(ErrorsEnum::AlreadyFinished);
#define Assert_Binded_Macro if (BindedInstances.gFrameBufferID() != ID) { ErrorsSystemNamespace::SendWarning<<\
"FrameBuffer is not binded">>ErrorsSystemNamespace::EndOfWarning; Bind(false); }
#else
#define Assert_Finished_Macro
#define Assert_NotFinished_Macro
#define Assert_Binded_Macro
#endif

void FrameBufferClass::ClearColorBuffer() const {
	Assert_Finished_Macro;
	Assert_Binded_Macro;
	glSC(glClear(GL_COLOR_BUFFER_BIT));
}
void FrameBufferClass::ClearDepthBuffer() const {
	Assert_Finished_Macro;
	Assert_Binded_Macro;
	glSC(glClear(GL_DEPTH_BUFFER_BIT));
}
void FrameBufferClass::ClearStencilBuffer() const {
	Assert_Finished_Macro;
	Assert_Binded_Macro;
	glSC(glClear(GL_STENCIL_BUFFER_BIT));
}
void FrameBufferClass::ClearAllBuffers() const {
	ClearColorBuffer();
	ClearDepthBuffer();
	ClearStencilBuffer();
}

FrameBufferClass::FrameBufferClass(Vector2U viewportSize) :ViewportSize(viewportSize) {
	glSC(glGenFramebuffers(1, &ID));
	Bind(false);
}
FrameBufferClass::FrameBufferClass() {
	glSC(glGenFramebuffers(1, &ID));
	Bind(false);
}
FrameBufferClass::FrameBufferClass(FrameBufferClass&& toCopy) noexcept:
	ID(toCopy.ID), ViewportSize(toCopy.ViewportSize) 
#ifdef KE2_Debug
	,Finished(toCopy.Finished)
#endif
{
	toCopy.ID = 0u;
}
FrameBufferClass& FrameBufferClass::operator=(FrameBufferClass&& toCopy) {
	this->~FrameBufferClass();
	new(this) FrameBufferClass(std::move(toCopy));
	return *this;
}
FrameBufferClass::~FrameBufferClass() noexcept(false) {
	if (ID != 0u) {
		Unbind();
		glSC(glDeleteFramebuffers(1, &ID));
		ID = 0u;
	}
}
void FrameBufferClass::AttachRenderBuffer(unsigned int renderBufferID, bool depthBufferEnabled, bool stencilBufferEnabled) const {
	Assert_Binded_Macro;
	Assert_NotFinished_Macro;

	if (depthBufferEnabled and stencilBufferEnabled) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID)); }
	else if (depthBufferEnabled and not stencilBufferEnabled) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferID)); }
	else if (not depthBufferEnabled and stencilBufferEnabled) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID)); }
#if defined KE2_Debug
	else ErrorsSystemNamespace::SendWarning << "no buffers selected to bind from RenderBuffer to FrameBuffer" >> ErrorsSystemNamespace::EndOfWarning;
#endif
}
void FrameBufferClass::AttachTexture(unsigned int texID, TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormat) const {
	Assert_Binded_Macro;
	Assert_NotFinished_Macro;

	int glAtt = 0;
	switch (dataFormat) {
	case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::DepthComponent: glAtt = GL_DEPTH_ATTACHMENT; break;
	case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::DepthStencil: glAtt = GL_DEPTH_STENCIL_ATTACHMENT; break;
	case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Red: glAtt = GL_COLOR_ATTACHMENT0; break;
	case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RG: glAtt = GL_COLOR_ATTACHMENT0; break;
	case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGB: glAtt = GL_COLOR_ATTACHMENT0; break;
	case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA: glAtt = GL_COLOR_ATTACHMENT0; break;
	}
	glSC(glFramebufferTexture2D(GL_FRAMEBUFFER, glAtt, GL_TEXTURE_2D, texID, 0));
}
void FrameBufferClass::Finish() const {
	Assert_Binded_Macro;
	Assert_NotFinished_Macro;

#if defined KE2_Debug
	Finished = true;
	unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		ErrorsSystemNamespace::SendError << "FrameBuffer is not complete, OpenGL's error: [" << std::to_string(status) << "]" >> ErrorsEnumWrapperStruct(ErrorsEnum::NotComplete);
#endif
}
void FrameBufferClass::Bind(bool updViewportSize) const {

#if defined KE2_Debug
	BindedInstances.sFrameBuffer_ID(ID);
#endif
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
	if (updViewportSize) { glSC(glViewport(0, 0, ViewportSize[0], ViewportSize[1])); }
}
void FrameBufferClass::Unbind() {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
#if defined KE2_Debug
	BindedInstances.sFrameBuffer_ID(0);
#endif
}
void FrameBufferClass::sViewportSize(Vector2U viewportSize, bool updViewportSize) {
	ViewportSize = viewportSize;
	if (updViewportSize) { glSC(glViewport(0, 0, viewportSize[0], viewportSize[1])); }
}
void FrameBufferClass::SetViewportSize_Static(Vector2U viewportSize) {
	glSC(glViewport(0, 0, viewportSize[0], viewportSize[1]));
}
