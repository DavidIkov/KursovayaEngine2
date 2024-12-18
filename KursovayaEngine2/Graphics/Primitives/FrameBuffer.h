#pragma once
#include"DLL.h"
#include"Texture.h"
#include"RenderBuffer.h"
#include<vector>
#include"Tools/ErrorsSystem.h"

namespace KE2::Graphics::Primitives {

	enum class FrameBufferClassTextureAttachment {
		Color, Depth, Stencil
	};

	class FrameBufferClass {
		unsigned int ID = 0u;
		Vector2U ViewportSize = Vector2U(0u);
#if defined KE2_Debug
		mutable bool Finished = false;
#endif

		static unsigned int _BufferDataFormat_SwitchCase(RenderBufferClass::BufferDataFormatEnum bufferDataFormat) noexcept;
		static unsigned int _DataFormatOnGPU_SwitchCase(TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU) noexcept;
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				AlreadyFinished,
#ifdef KE2_Debug
				NotFinished,
#endif
				NotComplete
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;
		

		DLLTREATMENT void ClearColorBuffer() const;
		DLLTREATMENT void ClearDepthBuffer() const;
		DLLTREATMENT void ClearStencilBuffer() const;
		//color,depth,stencil
		DLLTREATMENT void ClearAllBuffers() const;

		DLLTREATMENT FrameBufferClass(Vector2U viewportSize);
		//viewportSize will be (0,0)
		DLLTREATMENT FrameBufferClass();
		DLLTREATMENT FrameBufferClass(FrameBufferClass&& toCopy) noexcept;
		DLLTREATMENT FrameBufferClass& operator=(FrameBufferClass&& toCopy);
		DLLTREATMENT virtual ~FrameBufferClass() noexcept(false);

		typedef unsigned int FrameBufferID_Type;
		inline FrameBufferID_Type gID() const noexcept { return ID; }
		inline operator FrameBufferID_Type() const noexcept { return ID; }

		DLLTREATMENT void Finish() const;
		//if updViewportSize is true then it will update opengl's viewport size
		DLLTREATMENT void Bind(bool updViewportSize = true) const;
		//use colorAttachmentInd only if bufferDataFormat have somnething to do with colors, not depth/stencil
		DLLTREATMENT void AttachRenderBuffer(const RenderBufferClass& renderBuffer, RenderBufferClass::BufferDataFormatEnum bufferDataFormat, unsigned int colorAttachmentInd = 0);
		//use colorAttachmentInd only if dataFormatOnGPU have somnething to do with colors, not depth/stencil
		DLLTREATMENT void AttachTexture(const TextureClass& texture, TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, unsigned int colorAttachmentInd = 0);
		DLLTREATMENT static void Unbind();
		//if updViewportSize is true then it will update opengl's viewport size
		inline Vector2U gViewportSize() const noexcept { return ViewportSize; }
		DLLTREATMENT void SetViewportSize(Vector2U viewportSize, bool updViewportSize = true);
		//will update opengl's viewport size
		DLLTREATMENT static void SetViewportSize_Static(Vector2U viewportSize);

	};
}