#include"FrameBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"


void FrameBuffer::ClearColorBuffer() {
	Bind();
	glSC(glClear(GL_COLOR_BUFFER_BIT));
}
void FrameBuffer::ClearDepthBuffer() {
	Bind();
	glSC(glClear(GL_DEPTH_BUFFER_BIT));
}
void FrameBuffer::ClearStencilBuffer() {
	Bind();
	glSC(glClear(GL_STENCIL_BUFFER_BIT));
}
void FrameBuffer::ClearAllBuffers() {
	Bind();
	glSC(glClear(GL_COLOR_BUFFER_BIT));
	glSC(glClear(GL_DEPTH_BUFFER_BIT));
	glSC(glClear(GL_STENCIL_BUFFER_BIT));
}

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) :Width(width), Height(height) {
	glSC(glGenFramebuffers(1, &ID));
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
}
FrameBuffer::FrameBuffer(FrameBuffer&& tempFB){
	memcpy(this, &tempFB, sizeof(tempFB));
	tempFB.Deleted = true;
}
FrameBuffer::~FrameBuffer() {
	if (not Deleted) {
		glSC(glDeleteFramebuffers(1, &ID));
		Deleted = true;
	}
}
unsigned int FrameBuffer::gID() const {
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "FRAME BUFFER IS DELETED, ACCESSING ITS ID MAY CAUSE PROBLEMS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
	return ID;
}
void FrameBuffer::AttachRenderBuffer(const RenderBuffer& rb) const {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
	bool db = rb.gIsDepthBufferEnabled();
	bool sb = rb.gIsStencilBufferEnabled();
	if (db and sb) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb.gID())); }
	else if (db and not sb) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb.gID())); }
	else if (not db and sb) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT ATTACH TO FRAMEBUFFER A RENDER BUFFER WHICH HAVE STENCIL BUFFER BUT HAVENT GOT DEPTH BUFFER", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
	else DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT ATTACH TO FRAMEBUFFER A RENDER BUFFER WHICH DOESNT HAVE ANY BUFFERS ENABLED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
}
void FrameBuffer::AttachTexture(Texture& tex) {
	AttachedTextures.push_back(&tex);

	int glAtt = 0;
	switch (tex.gStorageType()) {
	case TextureStorageType::RGB:
	{
		glAtt = GL_COLOR_ATTACHMENT0;
		break;
	}
	case TextureStorageType::RGBA:
	{
		glAtt = GL_COLOR_ATTACHMENT0;
		break;
	}
	case TextureStorageType::Depth:
	{
		glAtt = GL_DEPTH_ATTACHMENT;
		break;
	}
	case TextureStorageType::DepthStencil:
	{
		glAtt = GL_DEPTH_STENCIL_ATTACHMENT;
		break;
	}
	}
	glSC(glFramebufferTexture2D(GL_FRAMEBUFFER, glAtt, GL_TEXTURE_2D, tex.gID(), 0));
}
void FrameBuffer::Delete() {
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "FRAME BUFFER IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
	else this->~FrameBuffer();
}
void FrameBuffer::Finish() {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FRAMEBUFFER IS NOT COMPLETE", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
	}
	else {
		Finished = true;
	}
}
void FrameBuffer::Bind() const {
	if (not Finished) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT BIND BUFFER WHICH IS INCOMPLETE", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
	else if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT BIND ALREADY DELETED BUFFER", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
	else {
		glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
		glSC(glViewport(0, 0, Width, Height));
	}
}
void FrameBuffer::Unbind(unsigned int width, unsigned int height) {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	glSC(glViewport(0, 0, width, height));
}