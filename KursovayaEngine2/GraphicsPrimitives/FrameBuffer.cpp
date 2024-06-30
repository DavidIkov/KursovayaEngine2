#include"FrameBuffer.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height):Tex(width,height) {
	glSC(glGenFramebuffers(1, &ID));
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
	glSC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Tex.gID(), 0));
}
FrameBuffer::~FrameBuffer() {
	if (not Deleted) {
		glSC(glDeleteFramebuffers(1, &ID));
		Deleted = true;
	}
}
unsigned int FrameBuffer::gID() const {
	return ID;
}
void FrameBuffer::AttachRenderBuffer(const RenderBuffer& rb) const {
	bool db = rb.gIsDepthBufferEnabled();
	bool sb = rb.gIsStencilBufferEnabled();
	if (db and sb) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb.gID())) }
	else if (db and not sb) { glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb.gID())) }
	else if (not db and sb) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT ATTACH TO FRAMEBUFFER A RENDER BUFFER WHICH HAVE STENCIL BUFFER BUT HAVENT GOT DEPTH BUFFER", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_ATTACH_TO_FRAMEBUFFER_A_RENDERBUFFER_WHICH_HAVE_STENCIL_BUFFER_BUT_HAVENT_GOT_DEPTH_BUFFER });
	else DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT ATTACH TO FRAMEBUFFER A RENDER BUFFER WHICH DOESNT HAVE ANY BUFFERS ENABLED", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_ATTACH_TO_FRAMEBUFFER_A_RENDERBUFFER_WHICH_DOSENT_HAVE_ANY_BUFFERS_ENABLED });
}
void FrameBuffer::Delete() {
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "FRAME BUFFER IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_DELETE_ALREADY_DELETED_FRAMEBUFFER });
	else this->~FrameBuffer();
}
const Texture& FrameBuffer::gTexture() const {
	return Tex;
}
void FrameBuffer::Finish() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FRAMEBUFFER IS NOT COMPLETE", KURSAVAYAENGINE2_CORE_ERRORS::FRAMEBUFFER_IS_NOT_COMPLETE });
	}
	else {
		Finished = true;
	}
}
void FrameBuffer::Bind() const {
	if (not Finished) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT BIND BUFFER WHICH IS INCOMPLETE", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_BIND_INCOMPLETE_FRAMEBUFFER });
	else if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "YOU CANT BIND ALREADY DELETED BUFFER", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_BIND_ALREADY_DELETED_FRAMEBUFFER });
	else {
		glSC(glBindFramebuffer(GL_FRAMEBUFFER, ID));
	}
}
void FrameBuffer::Unbind() {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}