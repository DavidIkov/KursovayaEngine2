#pragma once
#include"DLL.h"
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

	DLLTREATMENT void ClearColorBuffer();
	DLLTREATMENT void ClearDepthBuffer();
	DLLTREATMENT void ClearStencilBuffer();
	//color,depth,stencil
	DLLTREATMENT void ClearAllBuffers();

	DLLTREATMENT FrameBuffer(unsigned int width, unsigned int height);
	DLLTREATMENT FrameBuffer(FrameBuffer&& tempFB);
	DLLTREATMENT ~FrameBuffer();
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT void Delete();
	DLLTREATMENT void Finish();
	DLLTREATMENT void Bind() const;
	DLLTREATMENT void AttachRenderBuffer(const RenderBuffer& rb) const;
	DLLTREATMENT void AttachTexture(Texture& tex);
	DLLTREATMENT static void Unbind(unsigned int width, unsigned int height);
};