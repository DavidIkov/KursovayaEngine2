#pragma once
#include"DLL.h"
#include"Texture.h"
#include<vector>
#include"Tools/ClassFunctionsAccessController.h"

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
			CFAC_FuncPtrConstr(ClearColorBuffer)
			CFAC_FuncPtrConstr(ClearDepthBuffer)
			CFAC_FuncPtrConstr(ClearStencilBuffer)
			CFAC_FuncPtrConstr(ClearAllBuffers)
			CFAC_FuncPtrConstr(gID)
			CFAC_FuncPtrConstr(Finish)
			CFAC_FuncPtrConstr(Bind)
			CFAC_FuncPtrConstr(AttachRenderBuffer)
			CFAC_FuncPtrConstr(AttachTexture)
			CFAC_FuncPtrConstr(Unbind)
		);
#undef CFAC_ClassName
	};
}