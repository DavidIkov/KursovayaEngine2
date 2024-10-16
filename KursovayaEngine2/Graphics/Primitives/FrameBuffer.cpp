#include"FrameBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"
#include"Graphics/Globals.h"
#include"Tools/DebugRuntimeAssert.h"

using namespace Graphics::Primitives;
#define Assert_NotDeleted_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Deleted, "FrameBuffer is deleted", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#if defined KE2_Debug
#define Assert_Finished_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, Finished, "FrameBuffer is not finished", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#define Assert_NotFinished_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Finished, "FrameBuffer is not finished", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#define Assert_Binded_Macro if (DebugRuntimeAssert(DebuggingTools::ErrorTypes::Warning, BindedInstances.gFrameBufferID() == ID, "FrameBuffer is not binded", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION)) Bind();
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

FrameBufferClass::FrameBufferClass(unsigned int width, unsigned int height) :Width(width), Height(height) {
	glSC(glGenFramebuffers(1, &ID));
	Bind();
}
FrameBufferClass::FrameBufferClass(const FrameBufferClass&& toCopy) {
	memcpy(this, &toCopy, sizeof(FrameBufferClass));
	toCopy.Deleted = true;
}
void FrameBufferClass::operator=(const FrameBufferClass&& toCopy) {
	this->~FrameBufferClass();
	memcpy(this, &toCopy, sizeof(FrameBufferClass));
	toCopy.Deleted = true;
}
FrameBufferClass::~FrameBufferClass() {
	if (not Deleted) {
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
#if defined KE2_Debug
	else if (not depthBufferEnabled and stencilBufferEnabled) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "cant attach RenderBuffer to a FrameBuffer, RenderBuffer have stencil buffer and no depth buffer", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
	else DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "cant attach RenderBuffer to a FrameBuffer, RenderBuffer have no buffers", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
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
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FrameBuffer is not complete", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
#endif
}
void FrameBufferClass::Bind() {
	Assert_NotDeleted_Macro;

#if defined KE2_Debug
	BindedInstances.sFrameBuffer_ID(ID);
#endif
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
	glSC(glViewport(0, 0, Width, Height));
}
void FrameBufferClass::Unbind(unsigned int width, unsigned int height) {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	glSC(glViewport(0, 0, width, height));
}