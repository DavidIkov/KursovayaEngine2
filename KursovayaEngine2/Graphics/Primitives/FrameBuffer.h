#pragma once
#include"DLL.h"
#include"Texture.h"
#include"RenderBuffer.h"
#include<vector>
#include"Tools/ErrorsSystem.h"

namespace KE2::Graphics::Primitives {

	class FrameBufferClass {
	protected:
		unsigned int ID = 0u;
#if defined KE2_Debug
		mutable bool Finished = false;
#endif

		static unsigned int _BufferDataFormat_SwitchCase(RenderBufferClass::BufferDataFormatEnum bufferDataFormat) noexcept;
		static unsigned int _DataFormatOnGPU_SwitchCase(TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU) noexcept;
	public:

		enum class AttachmentTypesEnum : unsigned char {
			Depth,Stencil,DepthStencil,Color
		};
	protected:
		static unsigned int _AttachmentType_SwitchCase(AttachmentTypesEnum attachmentType) noexcept;
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

		DLLTREATMENT FrameBufferClass();
		DLLTREATMENT FrameBufferClass(FrameBufferClass&& toCopy) noexcept;
		DLLTREATMENT virtual FrameBufferClass& operator=(FrameBufferClass&& toCopy);
		DLLTREATMENT virtual ~FrameBufferClass() noexcept(false);

		typedef unsigned int FrameBufferID_Type;
		inline FrameBufferID_Type gID() const noexcept { return ID; }
		inline operator FrameBufferID_Type() const noexcept { return ID; }

		DLLTREATMENT void Finish() const;
		DLLTREATMENT void Bind() const;
		//use colorAttachmentInd only if bufferDataFormat have somnething to do with colors, not depth/stencil
		DLLTREATMENT virtual void AttachRenderBuffer(RenderBufferClass& renderBuffer, RenderBufferClass::BufferDataFormatEnum bufferDataFormat, unsigned int colorAttachmentInd = 0);
		DLLTREATMENT virtual void UnattachRenderBuffer(RenderBufferClass::BufferDataFormatEnum bufferDataFormat, unsigned int colorAttachmentInd = 0);
		DLLTREATMENT virtual void UnattachRenderBuffer(AttachmentTypesEnum attachmentType, unsigned int colorAttachmentInd = 0);
		//use colorAttachmentInd only if dataFormatOnGPU have somnething to do with colors, not depth/stencil
		DLLTREATMENT virtual void AttachTexture(TextureClass& texture, TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, unsigned int colorAttachmentInd = 0);
		DLLTREATMENT virtual void UnattachTexture(TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, unsigned int colorAttachmentInd = 0);
		DLLTREATMENT virtual void UnattachTexture(AttachmentTypesEnum attachmentType, unsigned int colorAttachmentInd = 0);
		DLLTREATMENT static void Unbind();
		//will update current viewport size
		DLLTREATMENT static void SetViewportSize(Vector2U viewportSize);

	};
}