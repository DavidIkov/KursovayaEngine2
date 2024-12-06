#pragma once
#include"DLL.h"
#include"Tools/ErrorsSystem.h"

namespace Graphics::Primitives {
	class RenderBufferClass {
		unsigned int ID = 0u;
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				StencilBufferWithoutDepthBuffer,
				NoBuffers,
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

		DLLTREATMENT RenderBufferClass(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer);
		DLLTREATMENT RenderBufferClass(RenderBufferClass&& toCopy) noexcept;
		DLLTREATMENT RenderBufferClass& operator=(RenderBufferClass&& toCopy);
		DLLTREATMENT virtual ~RenderBufferClass() noexcept(false);
		DLLTREATMENT void Bind() const;
		DLLTREATMENT static void Unbind();
		inline unsigned int gID() const noexcept { return ID; }

	};
}