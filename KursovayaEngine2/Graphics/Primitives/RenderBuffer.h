#pragma once
#include"DLL.h"
#include"Tools/ErrorsSystem.h"

namespace KE2::Graphics::Primitives {
	class RenderBufferClass {
	protected:
		unsigned int ID = 0u;
	public:
		enum class BufferDataFormatEnum {
			Depth, DepthStencil, Stencil, Red, RG, RGB, RGBA, None
		};
	protected:
		static unsigned int _BufferDataFormat_SwitchCase(BufferDataFormatEnum bufferDataFormat) noexcept;
	public:

		DLLTREATMENT RenderBufferClass(unsigned int width, unsigned int height, BufferDataFormatEnum bufferDataFormat);
		DLLTREATMENT RenderBufferClass(RenderBufferClass&& toCopy) noexcept;
		DLLTREATMENT virtual RenderBufferClass& operator=(RenderBufferClass&& toCopy);
		DLLTREATMENT virtual ~RenderBufferClass() noexcept(false);
		DLLTREATMENT void Bind() const;
		DLLTREATMENT static void Unbind();

		typedef unsigned int RenderBufferID_Type;
		inline RenderBufferID_Type gID() const noexcept { return ID; }
		inline operator RenderBufferID_Type() const noexcept { return ID; }

	};
}