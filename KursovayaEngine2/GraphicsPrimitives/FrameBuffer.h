#pragma once
#include"Texture.h"
#include"RenderBuffer.h"
#include<vector>

enum class FrameBufferTextureAttachment {
	Color,Depth,Stencil
};

class FrameBuffer {
	unsigned int ID = 0;
	mutable bool Finished = false;
	mutable bool Deleted = false;
	unsigned int Width, Height;
	std::vector<Texture*> AttachedTextures;

public:

	FrameBuffer(unsigned int width, unsigned int height);
	~FrameBuffer();
	unsigned int gID() const;
	void Delete();
	void Finish();
	void Bind() const;
	void AttachRenderBuffer(const RenderBuffer& rb) const;
	void AttachTexture(Texture& tex);
	static void Unbind(unsigned int width, unsigned int height);
};