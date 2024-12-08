#pragma once
#include"DLL.h"
#include"Texture.h"
#include<vector>
#include"Tools/ErrorsSystem.h"

namespace Graphics::Primitives {

	enum class FrameBufferClassTextureAttachment {
		Color, Depth, Stencil
	};

	class FrameBufferClass {
		unsigned int ID = 0u;
#if defined KE2_Debug
		mutable bool Finished = false;
#endif
		Vector2U ViewportSize = Vector2U(0u);

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
		inline unsigned int gID() const noexcept { return ID; }
		DLLTREATMENT void Finish() const;
		//if updViewportSize is true then it will update opengl's viewport size
		DLLTREATMENT void Bind(bool updViewportSize = true) const;
		DLLTREATMENT void AttachRenderBuffer(unsigned int renderBufferID, bool depthBufferEnabled, bool stencilBufferEnabled) const;
		DLLTREATMENT void AttachTexture(unsigned int texID, TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormat) const;
		DLLTREATMENT static void Unbind();
		//if updViewportSize is true then it will update opengl's viewport size
		inline Vector2U gViewportSize() const noexcept { return ViewportSize; }
		DLLTREATMENT void SetViewportSize(Vector2U viewportSize, bool updViewportSize = true);
		//will update opengl's viewport size
		DLLTREATMENT static void SetViewportSize_Static(Vector2U viewportSize);

	};
}