#pragma once
#include"DLL.h"
#include"Texture.h"
#include"Tools/DynArr.h"
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
		unsigned int Width, Height;

	public:

		DLLTREATMENT void ClearColorBuffer();
		DLLTREATMENT void ClearDepthBuffer();
		DLLTREATMENT void ClearStencilBuffer();
		//color,depth,stencil
		DLLTREATMENT void ClearAllBuffers();

		DLLTREATMENT FrameBufferClass(unsigned int width, unsigned int height);
		DLLTREATMENT FrameBufferClass(RespConstrFlag, const FrameBufferClass& toCopy);
		DLLTREATMENT FrameBufferClass(const FrameBufferClass&& toCopy);
		DLLTREATMENT void operator=(const FrameBufferClass&& toCopy);
		DLLTREATMENT ~FrameBufferClass();
		DLLTREATMENT unsigned int gID();
		DLLTREATMENT void Delete();
		DLLTREATMENT void Finish();
		DLLTREATMENT void Bind();
		DLLTREATMENT void AttachRenderBuffer(unsigned int renderBufferID, bool depthBufferEnabled, bool stencilBufferEnabled);
		DLLTREATMENT void AttachTexture(unsigned int texID, TextureDataSettingsClass::DataFormatOnGPU_Enum dataFormat);
		DLLTREATMENT static void Unbind(unsigned int width, unsigned int height);

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