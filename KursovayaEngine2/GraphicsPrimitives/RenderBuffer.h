#pragma once
#include"DLL.h"


class RenderBufferClass {
	unsigned int ID = 0;
	mutable bool Deleted = false;
public:
	DLLTREATMENT RenderBufferClass(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer);
	DLLTREATMENT RenderBufferClass(const RenderBufferClass* toCopy);
	DLLTREATMENT RenderBufferClass(const RenderBufferClass&& toCopy);
	DLLTREATMENT void operator=(const RenderBufferClass&& toCopy);
	DLLTREATMENT ~RenderBufferClass();
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT void Delete();
};