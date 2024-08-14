#pragma once
#include"DLL.h"


class RenderBuffer {
	unsigned int ID = 0;
	bool DepthBufferEnabled = false;
	bool StencilBufferEnabled = false;
	mutable bool Deleted = false;
public:
	DLLTREATMENT RenderBuffer(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer);
	DLLTREATMENT RenderBuffer(RenderBuffer&& tempRB);
	DLLTREATMENT ~RenderBuffer();
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT void Delete();
	DLLTREATMENT bool gIsDepthBufferEnabled() const;
	DLLTREATMENT bool gIsStencilBufferEnabled() const;
};