#include"FrameBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"

using namespace GraphicsPrimitives;

void FrameBufferClass::ClearColorBuffer() const {
	Bind();
	glSC(glClear(GL_COLOR_BUFFER_BIT));
}
void FrameBufferClass::ClearDepthBuffer() const {
	Bind();
	glSC(glClear(GL_DEPTH_BUFFER_BIT));
}
void FrameBufferClass::ClearStencilBuffer() const {
	Bind();
	glSC(glClear(GL_STENCIL_BUFFER_BIT));
}
void FrameBufferClass::ClearAllBuffers() const {
	Bind();
	glSC(glClear(GL_COLOR_BUFFER_BIT));
	glSC(glClear(GL_DEPTH_BUFFER_BIT));
	glSC(glClear(GL_STENCIL_BUFFER_BIT));
}

FrameBufferClass::FrameBufferClass(unsigned int width, unsigned int height) :Width(width), Height(height) {
	glSC(glGenFramebuffers(1, &ID));
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
}
FrameBufferClass::FrameBufferClass(RespConstrFlag, const FrameBufferClass& toCopy) {
	memcpy(this, &toCopy, sizeof(FrameBufferClass));
	toCopy.Deleted = true;
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
unsigned int FrameBufferClass::gID() const {
#if defined Debug
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "FRAME BUFFER IS DELETED, ACCESSING ITS ID MAY CAUSE PROBLEMS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
#endif
	return ID;
}
void FrameBufferClass::AttachRenderBuffer(unsigned int renderBufferID, bool depthBufferEnabled, bool stencilBufferEnabled) {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
	if (depthBufferEnabled and stencilBufferEnabled) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID)); }
	else if (depthBufferEnabled and not stencilBufferEnabled) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferID)); }
#if defined Debug
	else if (not depthBufferEnabled and stencilBufferEnabled) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT ATTACH TO FRAMEBUFFER A RENDER BUFFER WHICH HAVE STENCIL BUFFER BUT HAVENT GOT DEPTH BUFFER", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
	else DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT ATTACH TO FRAMEBUFFER A RENDER BUFFER WHICH DOESNT HAVE ANY BUFFERS ENABLED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
#endif
}
void FrameBufferClass::AttachTexture(unsigned int texID, TextureDataSettingsClass::DataFormatOnGPU_Enum dataFormat) {

	int glAtt = 0;
	switch (dataFormat) {
	case TextureDataSettingsClass::DataFormatOnGPU_Enum::DepthComponent: glAtt = GL_DEPTH_ATTACHMENT; break;
	case TextureDataSettingsClass::DataFormatOnGPU_Enum::DepthStencil: glAtt = GL_DEPTH_STENCIL_ATTACHMENT; break;
	case TextureDataSettingsClass::DataFormatOnGPU_Enum::Red: glAtt = GL_COLOR_ATTACHMENT0; break;
	case TextureDataSettingsClass::DataFormatOnGPU_Enum::RG: glAtt = GL_COLOR_ATTACHMENT0; break;
	case TextureDataSettingsClass::DataFormatOnGPU_Enum::RGB: glAtt = GL_COLOR_ATTACHMENT0; break;
	case TextureDataSettingsClass::DataFormatOnGPU_Enum::RGBA: glAtt = GL_COLOR_ATTACHMENT0; break;
	}
	glSC(glFramebufferTexture2D(GL_FRAMEBUFFER, glAtt, GL_TEXTURE_2D, texID, 0));
}
void FrameBufferClass::Delete() {
#if defined Debug
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "FRAME BUFFER IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
	else 
#endif
		this->~FrameBufferClass();
}
void FrameBufferClass::Finish() {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
#if defined Debug
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FRAMEBUFFER IS NOT COMPLETE", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
	}
	else {
		Finished = true;
	}
#endif
}
void FrameBufferClass::Bind() const {
#if defined Debug
	if (not Finished) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT BIND BUFFER WHICH IS INCOMPLETE", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
	else if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT BIND ALREADY DELETED BUFFER", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
	else 
#endif
	{
		glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
		glSC(glViewport(0, 0, Width, Height));
	}
}
void FrameBufferClass::Unbind(unsigned int width, unsigned int height) {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	glSC(glViewport(0, 0, width, height));
}