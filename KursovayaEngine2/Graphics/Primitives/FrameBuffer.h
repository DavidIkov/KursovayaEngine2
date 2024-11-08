#pragma once
#include"DLL.h"
#include"Texture.h"
#include<vector>
#include"Tools/ClassFunctionsAccessController.h"
#include"Tools/ErrorsSystem.h"

namespace Graphics::Primitives {

	enum class FrameBufferClassTextureAttachment {
		Color, Depth, Stencil
	};

	class FrameBufferClass {
		unsigned int ID = 0;
#if defined KE2_Debug
		mutable bool Finished = false;
#endif
		mutable bool Deleted = false;
		Vector2U ViewportSize;

	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				AlreadyDeleted,
				AlreadyFinished,
				NotFinished,
				NotComplete
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;
		

		DLLTREATMENT void ClearColorBuffer();
		DLLTREATMENT void ClearDepthBuffer();
		DLLTREATMENT void ClearStencilBuffer();
		//color,depth,stencil
		DLLTREATMENT void ClearAllBuffers();

		DLLTREATMENT FrameBufferClass(Vector2U viewportSize);
		//viewportSize will be (0,0)
		DLLTREATMENT FrameBufferClass();
		DLLTREATMENT FrameBufferClass(const FrameBufferClass&& toCopy);
		DLLTREATMENT void operator=(const FrameBufferClass&& toCopy);
		DLLTREATMENT ~FrameBufferClass();
		DLLTREATMENT unsigned int gID();
		DLLTREATMENT void Delete();
		DLLTREATMENT void Finish();
		//if updViewportSize is true then it will update opengl's viewport size
		DLLTREATMENT void Bind(bool updViewportSize);
		DLLTREATMENT void AttachRenderBuffer(unsigned int renderBufferID, bool depthBufferEnabled, bool stencilBufferEnabled);
		DLLTREATMENT void AttachTexture(unsigned int texID, TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormat);
		DLLTREATMENT static void Unbind();
		DLLTREATMENT static void SetViewportSize(Vector2U viewportSize);

#define CFAC_ClassName FrameBufferClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncConstr(ClearColorBuffer)
			CFAC_FuncConstr(ClearDepthBuffer)
			CFAC_FuncConstr(ClearStencilBuffer)
			CFAC_FuncConstr(ClearAllBuffers)
			CFAC_FuncConstr(gID)
			CFAC_FuncConstr(Finish)
			CFAC_FuncConstr(Bind)
			CFAC_FuncConstr(AttachRenderBuffer)
			CFAC_FuncConstr(AttachTexture)
			CFAC_FuncConstr(Unbind)
		);
#undef CFAC_ClassName
	};
}