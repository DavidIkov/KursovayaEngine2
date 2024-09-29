#pragma once
#include"DLL.h"
#include"Tools/DynArr.h"
#include"Tools/ClassFunctionsAccessController.h"

namespace Graphics::Primitives {

	class ShaderClass {
		unsigned int ID;
		mutable bool Deleted = false;
#if defined Debug
		mutable bool Compiled = false;
#endif
	public:
		enum class TypesEnum :unsigned short int {
			Fragment, Vertex, Geometry
		};
	private:
#if defined Debug
		TypesEnum ShaderType;
#endif
	public:
		DLLTREATMENT unsigned int gID();
		DLLTREATMENT ShaderClass(const wchar_t* filePath, TypesEnum typ);
		DLLTREATMENT ShaderClass(TypesEnum typ, const char* code);
		DLLTREATMENT ShaderClass(RespConstrFlag, const ShaderClass& toCopy);
		DLLTREATMENT ShaderClass(const ShaderClass&& toCopy);
		DLLTREATMENT void operator=(const ShaderClass&& toCopy);
		DLLTREATMENT void Compile();
		DLLTREATMENT ~ShaderClass();
		DLLTREATMENT void Delete();
		
#define CFAC_ClassName ShaderClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncPtrConstr(gID)
			CFAC_FuncPtrConstr(Compile)
		);
#undef CFAC_ClassName
	};
}