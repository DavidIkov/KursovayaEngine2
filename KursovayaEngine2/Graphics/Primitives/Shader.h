#pragma once
#include"DLL.h"
#include"Tools/ErrorsSystem.h"

namespace Graphics::Primitives {

	class ShaderClass {
		unsigned int ID = 0u;
#ifdef KE2_Debug
		mutable bool Compiled = false;
#endif
	public:
		enum class TypesEnum :unsigned short int {
			Fragment, Vertex, Geometry
		};
	private:
#ifdef KE2_Debug
		const TypesEnum ShaderType;
#endif
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
#ifdef KE2_Debug
				AlreadyCompiled,
#endif
				FailedToCompile,
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

		DLLTREATMENT ShaderClass(const wchar_t* filePath, TypesEnum typ);
		DLLTREATMENT ShaderClass(TypesEnum typ, const char* code);
		DLLTREATMENT ShaderClass(ShaderClass&& toCopy) noexcept;
		DLLTREATMENT ShaderClass& operator=(ShaderClass&& toCopy);
		DLLTREATMENT void Compile() const;
		inline unsigned int gID() const noexcept { return ID; }
		DLLTREATMENT virtual ~ShaderClass() noexcept(false);
	};
}