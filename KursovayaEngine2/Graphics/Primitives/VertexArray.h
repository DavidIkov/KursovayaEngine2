#pragma once
#include"DLL.h"
#include"Tools/ClassFunctionsAccessController.h"

namespace Graphics::Primitives {

	class VertexBufferClass;

	class VertexArrayClass {
		unsigned int ID = 0;
		mutable bool Deleted = false;
	public:
		DLLTREATMENT VertexArrayClass();
		DLLTREATMENT VertexArrayClass(const VertexArrayClass&& toCopy);
		DLLTREATMENT void operator=(const VertexArrayClass&& toCopy);
		DLLTREATMENT ~VertexArrayClass();
		DLLTREATMENT unsigned int gID();
		DLLTREATMENT void Delete();
		DLLTREATMENT void Bind();
		DLLTREATMENT static void Unbind();

#define CFAC_ClassName VertexArrayClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncConstr(gID)
			CFAC_FuncConstr(Bind)
			CFAC_FuncConstr(Unbind)
		);
#undef CFAC_ClassName
	};
}