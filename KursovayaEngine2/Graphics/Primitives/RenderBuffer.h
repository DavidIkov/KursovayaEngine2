#pragma once
#include"DLL.h"
#include"Tools/ClassFunctionsAccessController.h"

namespace Graphics::Primitives {
	class RenderBufferClass {
		unsigned int ID = 0;
		mutable bool Deleted = false;
	public:
		DLLTREATMENT RenderBufferClass(unsigned int width, unsigned int height, bool createDepthBuffer, bool createStencilBuffer);
		DLLTREATMENT RenderBufferClass(const RenderBufferClass&& toCopy);
		DLLTREATMENT void operator=(const RenderBufferClass&& toCopy);
		DLLTREATMENT ~RenderBufferClass();
		DLLTREATMENT void Bind();
		DLLTREATMENT static void Unbind();
		DLLTREATMENT unsigned int gID();
		DLLTREATMENT void Delete();

#define CFAC_ClassName RenderBufferClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncPtrConstr(Bind)
			CFAC_FuncPtrConstr(gID)
		);
#undef CFAC_ClassName
	};
}