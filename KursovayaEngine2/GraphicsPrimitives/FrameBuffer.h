#pragma once
#include"Texture.h"
#include"RenderBuffer.h"

class FrameBuffer {
	unsigned int ID = 0;
	mutable bool Finished = false;
	mutable bool Deleted = false;
	Texture Tex;

public:
	FrameBuffer(unsigned int width, unsigned int height);
	~FrameBuffer();
	unsigned int gID() const;
	void Delete();
	void Finish();
	void Bind() const;
	const Texture& gTexture() const;
	void AttachRenderBuffer(const RenderBuffer& rb) const;
	static void Unbind();
};