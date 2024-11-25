#pragma once
#include"DLL.h"
#include"Tools/ClassFunctionsAccessController.h"
#include"Tools/ErrorsSystem.h"

namespace Graphics::Primitives {

	class ShaderClass {
		unsigned int ID;
		mutable bool Deleted = false;
#if defined KE2_Debug
		mutable bool Compiled = false;
#endif
	public:
		enum class TypesEnum :unsigned short int {
			Fragment, Vertex, Geometry
		};
	private:
#if defined KE2_Debug
		TypesEnum ShaderType;
#endif
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				AlreadyDeleted,
				AlreadyCompiled,
				FailedToCompile,
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

		DLLTREATMENT ShaderClass(const wchar_t* filePath, TypesEnum typ);
		DLLTREATMENT ShaderClass(TypesEnum typ, const char* code);
		DLLTREATMENT ShaderClass(const ShaderClass&& toCopy);
		DLLTREATMENT void operator=(const ShaderClass&& toCopy);
		DLLTREATMENT void Compile();
		DLLTREATMENT unsigned int gID();
		DLLTREATMENT virtual ~ShaderClass();
		DLLTREATMENT void Delete();
		
#define CFAC_ClassName ShaderClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncConstr(Compile)
			CFAC_FuncConstr(gID)
		);
#undef CFAC_ClassName
	};
}