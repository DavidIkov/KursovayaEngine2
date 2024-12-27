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
"FrameBuffer is not binded">>ErrorsSystemNamespace::EndOfWarning; Bind(); }
#else
#define Assert_Finished_Macro
#define Assert_NotFinished_Macro
#define Assert_Binded_Macro
#endif

unsigned int FrameBufferClass::_BufferDataFormat_SwitchCase(RenderBufferClass::BufferDataFormatEnum bufferDataFormat) noexcept {
	switch (bufferDataFormat) {
	case RenderBufferClass::BufferDataFormatEnum::Depth: return GL_DEPTH_ATTACHMENT;
    case RenderBufferClass::BufferDataFormatEnum::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
    case RenderBufferClass::BufferDataFormatEnum::Stencil: return GL_STENCIL_ATTACHMENT;
    case RenderBufferClass::BufferDataFormatEnum::Red: return GL_COLOR_ATTACHMENT0;
    case RenderBufferClass::BufferDataFormatEnum::RG: return GL_COLOR_ATTACHMENT0;
    case RenderBufferClass::BufferDataFormatEnum::RGB: return GL_COLOR_ATTACHMENT0;
    case RenderBufferClass::BufferDataFormatEnum::RGBA: return GL_COLOR_ATTACHMENT0;
	default: return 0;
	}
}
unsigned int FrameBufferClass::_DataFormatOnGPU_SwitchCase(TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU) noexcept {
	switch (dataFormatOnGPU) {
	case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Depth: return GL_DEPTH_ATTACHMENT;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Stencil: return GL_STENCIL_ATTACHMENT;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Red: return GL_COLOR_ATTACHMENT0;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RG: return GL_COLOR_ATTACHMENT0;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGB: return GL_COLOR_ATTACHMENT0;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA: return GL_COLOR_ATTACHMENT0;
	default: return 0;
	}
}
unsigned int FrameBufferClass::_AttachmentType_SwitchCase(AttachmentTypesEnum attachmentType) noexcept {
	switch (attachmentType) {
	case AttachmentTypesEnum::Depth: return GL_DEPTH_ATTACHMENT;
	case AttachmentTypesEnum::Stencil: return GL_STENCIL_ATTACHMENT;
	case AttachmentTypesEnum::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
	case AttachmentTypesEnum::Color: return GL_COLOR_ATTACHMENT0;
	}
	return 0;
}
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

FrameBufferClass::FrameBufferClass() {
	glSC(glGenFramebuffers(1, &ID));
	Bind();
}
FrameBufferClass::FrameBufferClass(FrameBufferClass&& toCopy) noexcept:
	ID(toCopy.ID)
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
void FrameBufferClass::AttachRenderBuffer(RenderBufferClass& renderBuffer, RenderBufferClass::BufferDataFormatEnum bufferDataFormat, unsigned int colorAttachmentInd) {
	Assert_Binded_Macro;
	Assert_NotFinished_Macro;

	glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, _BufferDataFormat_SwitchCase(bufferDataFormat) + colorAttachmentInd, GL_RENDERBUFFER, renderBuffer.gID()));
}
void FrameBufferClass::UnattachRenderBuffer(RenderBufferClass::BufferDataFormatEnum bufferDataFormat, unsigned int colorAttachmentInd) {
	Assert_Binded_Macro;
	Assert_NotFinished_Macro;

	glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, _BufferDataFormat_SwitchCase(bufferDataFormat) + colorAttachmentInd, GL_RENDERBUFFER, 0));
}
void FrameBufferClass::UnattachRenderBuffer(AttachmentTypesEnum attachmentType, unsigned int colorAttachmentInd) {
	Assert_Binded_Macro; Assert_NotFinished_Macro;

	glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, _AttachmentType_SwitchCase(attachmentType) + colorAttachmentInd, GL_RENDERBUFFER, 0));
}
void FrameBufferClass::AttachTexture(TextureClass& texture, TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, unsigned int colorAttachmentInd) {
	Assert_Binded_Macro;
	Assert_NotFinished_Macro;

	glSC(glFramebufferTexture2D(GL_FRAMEBUFFER, _DataFormatOnGPU_SwitchCase(dataFormatOnGPU) + colorAttachmentInd, GL_TEXTURE_2D, texture.gID(), 0));
}
void FrameBufferClass::UnattachTexture(TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, unsigned int colorAttachmentInd) {
	Assert_Binded_Macro;
	Assert_NotFinished_Macro;

	glSC(glFramebufferTexture2D(GL_FRAMEBUFFER, _DataFormatOnGPU_SwitchCase(dataFormatOnGPU) + colorAttachmentInd, GL_TEXTURE_2D, 0, 0));
}
void FrameBufferClass::UnattachTexture(AttachmentTypesEnum attachmentType, unsigned int colorAttachmentInd) {
	Assert_Binded_Macro; Assert_NotFinished_Macro;

	glSC(glFramebufferTexture2D(GL_FRAMEBUFFER, _AttachmentType_SwitchCase(attachmentType) + colorAttachmentInd, GL_TEXTURE_2D, 0, 0));
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
void FrameBufferClass::Bind() const {

#if defined KE2_Debug
	BindedInstances.sFrameBufferID(ID);
#endif
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
}
void FrameBufferClass::Unbind() {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
#if defined KE2_Debug
	BindedInstances.sFrameBufferID(0);
#endif
}
void FrameBufferClass::SetViewportSize(Vector2U viewportSize) {
	glSC(glViewport(0, 0, viewportSize[0], viewportSize[1]));
}
