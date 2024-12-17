#pragma once
#include"Graphics/Primitives/RenderBuffer.h"

namespace KE2::Graphics::Abstractions {
	class RenderBufferClass :protected Primitives::RenderBufferClass {

	protected:
		const bool DepthBufferEnabled = false; const bool StencilBufferEnabled = false;
	public:

		DLLTREATMENT RenderBufferClass(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer);
		DLLTREATMENT RenderBufferClass(RenderBufferClass&& toCopy) noexcept;
		DLLTREATMENT RenderBufferClass(const RenderBufferClass& toCopy) noexcept;
	private:
		inline virtual Primitives::RenderBufferClass& operator=(Primitives::RenderBufferClass&& toCopy) override final { return operator=(dynamic_cast<RenderBufferClass&&>(toCopy)); }
	public:
		DLLTREATMENT virtual RenderBufferClass& operator=(RenderBufferClass&& toCopy);
		DLLTREATMENT virtual RenderBufferClass& operator=(const RenderBufferClass& toCopy);
		DLLTREATMENT virtual ~RenderBufferClass() noexcept(false) override = default;

		inline bool gDepthBufferEnabled() const noexcept { return DepthBufferEnabled; }
		inline bool gStencilBufferEnabled() const noexcept { return StencilBufferEnabled; }

		using Primitives::RenderBufferClass::Bind;
		using Primitives::RenderBufferClass::Unbind;

		using RenderBufferID_Type = Primitives::RenderBufferClass::RenderBufferID_Type;
		using Primitives::RenderBufferClass::gID;
		using Primitives::RenderBufferClass::operator RenderBufferID_Type;

	};
}
