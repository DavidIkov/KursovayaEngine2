#pragma once


class RenderBuffer {
	unsigned int ID = 0;
	bool DepthBufferEnabled = false;
	bool StencilBufferEnabled = false;
	mutable bool Deleted = false;
public:
	RenderBuffer(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer);
	~RenderBuffer();
	unsigned int gID() const;
	void Delete();
	bool gIsDepthBufferEnabled() const;
	bool gIsStencilBufferEnabled() const;
};