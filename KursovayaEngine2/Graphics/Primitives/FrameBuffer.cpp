#include"FrameBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;
#define Assert_NotDeleted_Macro if (Deleted) ErrorsSystemNamespace::SendError<<"FrameBuffer is already deleted">>\
ErrorsEnumWrapperStruct(ErrorsEnum::AlreadyDeleted);
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

void FrameBufferClass::ClearColorBuffer() {
	Assert_NotDeleted_Macro;
	Assert_Finished_Macro;
	Assert_Binded_Macro;
	glSC(glClear(GL_COLOR_BUFFER_BIT));
}
void FrameBufferClass::ClearDepthBuffer() {
	Assert_NotDeleted_Macro;
	Assert_Finished_Macro;
	Assert_Binded_Macro;
	glSC(glClear(GL_DEPTH_BUFFER_BIT));
}
void FrameBufferClass::ClearStencilBuffer() {
	Assert_NotDeleted_Macro;
	Assert_Finished_Macro;
	Assert_Binded_Macro;
	glSC(glClear(GL_STENCIL_BUFFER_BIT));
}
void FrameBufferClass::ClearAllBuffers() {
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
FrameBufferClass::FrameBufferClass(const FrameBufferClass&& toCopy) {
	memcpy(this, &toCopy, sizeof(FrameBufferClass));
	toCopy.Deleted = true;
}
void FrameBufferClass::operator=(const FrameBufferClass&& toCopy) {
    Delete();
	memcpy(this, &toCopy, sizeof(FrameBufferClass));
	toCopy.Deleted = true;
}
FrameBufferClass::~FrameBufferClass() {
	if (not Deleted) {
		Unbind();
		glSC(glDeleteFramebuffers(1, &ID));
		Deleted = true;
	}
}
unsigned int FrameBufferClass::gID() {
	Assert_NotDeleted_Macro;
	return ID;
}
void FrameBufferClass::AttachRenderBuffer(unsigned int renderBufferID, bool depthBufferEnabled, bool stencilBufferEnabled) {
	Assert_NotDeleted_Macro;
	Assert_Binded_Macro;
	Assert_NotFinished_Macro;

	if (depthBufferEnabled and stencilBufferEnabled) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID)); }
	else if (depthBufferEnabled and not stencilBufferEnabled) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferID)); }
	else if (not depthBufferEnabled and stencilBufferEnabled) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID)); }
#if defined KE2_Debug
	else ErrorsSystemNamespace::SendWarning << "no buffers selected to bind from RenderBuffer to FrameBuffer" >> ErrorsSystemNamespace::EndOfWarning;
#endif
}
void FrameBufferClass::AttachTexture(unsigned int texID, TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormat) {
	Assert_NotDeleted_Macro;
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
void FrameBufferClass::Delete() {
	Assert_NotDeleted_Macro;
	this->~FrameBufferClass();
}
void FrameBufferClass::Finish() {
	Assert_NotDeleted_Macro;
	Assert_Binded_Macro;
	Assert_NotFinished_Macro;

#if defined KE2_Debug
	Finished = true;
	unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		ErrorsSystemNamespace::SendError << "FrameBuffer is not complete, OpenGL's error: [" << std::to_string(status) << "]" >> ErrorsEnumWrapperStruct(ErrorsEnum::NotComplete);
#endif
}
void FrameBufferClass::Bind(bool updViewportSize) {
	Assert_NotDeleted_Macro;

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
void FrameBufferClass::SetViewportSize(Vector2U viewportSize) {
	glSC(glViewport(0, 0, viewportSize[0], viewportSize[1]));
}
