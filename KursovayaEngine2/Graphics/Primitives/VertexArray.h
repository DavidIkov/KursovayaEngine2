#pragma once
#include"DLL.h"
#include"Tools/DynArr.h"
#include"Tools/ClassFunctionsAccessController.h"

namespace Graphics::Primitives {

	class VertexArrayClass {
		unsigned int ID = 0;
		mutable bool Deleted = false;
	public:
		DLLTREATMENT VertexArrayClass();
		DLLTREATMENT VertexArrayClass(RespConstrFlag, const VertexArrayClass& toCopy);
		DLLTREATMENT VertexArrayClass(const VertexArrayClass&& toCopy);
		DLLTREATMENT void operator=(const VertexArrayClass&& toCopy);
		DLLTREATMENT ~VertexArrayClass();
		DLLTREATMENT unsigned int gID();
		DLLTREATMENT void Delete();
		DLLTREATMENT void Bind();
		DLLTREATMENT static void Unbind();

#define CFAC_ClassName VertexArrayClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncPtrConstr(gID)
			CFAC_FuncPtrConstr(Bind)
			CFAC_FuncPtrConstr(Unbind)
		);
#undef CFAC_ClassName
	};
}