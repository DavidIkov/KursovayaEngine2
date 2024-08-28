#pragma once
#include"DLL.h"


class RenderBuffer {
	unsigned int ID = 0;
	mutable bool Deleted = false;
public:
	DLLTREATMENT RenderBuffer(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer);
	DLLTREATMENT RenderBuffer(const RenderBuffer* toCopy);
	DLLTREATMENT RenderBuffer(const RenderBuffer&& toCopy);
	DLLTREATMENT void operator=(const RenderBuffer&& toCopy);
	DLLTREATMENT ~RenderBuffer();
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT void Delete();
};