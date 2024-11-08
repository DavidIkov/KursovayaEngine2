#pragma once
#include"DLL.h"
#include"Tools/ClassFunctionsAccessController.h"
#include"Tools/ErrorsSystem.h"

namespace Graphics::Primitives {
	class RenderBufferClass {
		unsigned int ID = 0;
		mutable bool Deleted = false;
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				AlreadyDeleted,
				StencilBufferWithoutDepthBuffer,
				NoBuffers,
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

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
			CFAC_FuncConstr(Bind)
			CFAC_FuncConstr(gID)
		);
#undef CFAC_ClassName
	};
}