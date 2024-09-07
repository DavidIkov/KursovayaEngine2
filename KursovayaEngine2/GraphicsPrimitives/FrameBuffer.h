#pragma once
#include"DLL.h"
#include"Texture.h"
#include<vector>

enum class FrameBufferTextureAttachment {
	Color,Depth,Stencil
};

class FrameBuffer {
	unsigned int ID = 0;
#if defined Debug
	mutable bool Finished = false;
#endif
	mutable bool Deleted = false;
	unsigned int Width, Height;


public:

	DLLTREATMENT void ClearColorBuffer() const;
	DLLTREATMENT void ClearDepthBuffer() const;
	DLLTREATMENT void ClearStencilBuffer() const;
	//color,depth,stencil
	DLLTREATMENT void ClearAllBuffers() const;

	DLLTREATMENT FrameBuffer(unsigned int width, unsigned int height);
	DLLTREATMENT FrameBuffer(const FrameBuffer* toCopy);
	DLLTREATMENT FrameBuffer(const FrameBuffer&& toCopy);
	DLLTREATMENT void operator=(const FrameBuffer&& toCopy);
	DLLTREATMENT ~FrameBuffer();
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT void Delete();
	DLLTREATMENT void Finish();
	DLLTREATMENT void Bind() const;
	DLLTREATMENT void AttachRenderBuffer(unsigned int renderBufferID, bool depthBufferEnabled, bool stencilBufferEnabled);
	DLLTREATMENT void AttachTexture(unsigned int texID, TextureDataSettingsClass::DataFormatOnGPU_Enum dataFormat);
	DLLTREATMENT static void Unbind(unsigned int width, unsigned int height);
};