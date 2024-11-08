#pragma once
#include"DLL.h"
#include"Tools/DynArr.h"
#include"Graphics/Primitives/ShaderProgram.h"
#include"Tools/ClassFunctionsAccessController.h"
#include"Graphics/Primitives/Shader.h"

namespace Graphics::Abstractions {
	class ShaderClass {
	public:
		typedef Primitives::ShaderProgramClass::CFAC_UniformFuncsAccess_Class CFAC_UniformFuncs_Class;
	private:
		//dosent actually hold shaders data, it is storing StalkerClass pointing to actual instance
		class ShaderDataClass: protected Primitives::ShaderProgramClass {
			struct UniformDataStruct {
				std::string Name;
				unsigned int ID;
			};
			DynArr<UniformDataStruct> ShaderUniformsData;
			void _SaveShaderDataNames();
		public:
			ShaderDataClass(const wchar_t* vsPath, const wchar_t* fsPath);
			ShaderDataClass(const wchar_t* vsPath, const wchar_t* gsPath, const wchar_t* fsPath);
			ShaderDataClass(const ShaderDataClass&& toCopy);
			unsigned int GetUniformIDByName(const char* name);
			void Bind();
			CFAC_UniformFuncs_Class gCFAC_UniformFuncs();
			const Primitives::ShaderProgramClass& gPrimitiveShader() const;
			~ShaderDataClass();
		};

		static DynArr<ShaderDataClass> ShadersStorage;
		mutable bool Deleted = false;
		StalkerClass ShaderDataStalker;

		void(*UpdShaderDataFunc)(void* ptrToCustomStorage, ShaderClass* ptrToShader);
		void* PtrToCustomStorageOfShaderDataUpdaterFunc;
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				AlreadyDeleted,
			}; ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

		//if updShaderDataFuncPtr is nullptr then function wont be called
		DLLTREATMENT ShaderClass(const wchar_t* vertexShaderPath, const wchar_t* fragmentShaderPath, void* ptrToCustomStorageOfShaderDataUpdaterFunc, void(*updShaderDataFuncPtr)(void*, ShaderClass*));
		//if updShaderDataFuncPtr is nullptr then function wont be called
		DLLTREATMENT ShaderClass(const wchar_t* vertexShaderPath, const wchar_t* geometryShaderPath, const wchar_t* fragmentShaderPath, void* ptrToCustomStorageOfShaderDataUpdaterFunc, void(*updShaderDataFuncPtr)(void*, ShaderClass*));
		//if updShaderDataFuncPtr is nullptr then function wont be called
		DLLTREATMENT ShaderClass(const ShaderClass& toCopy, void* ptrToCustomStorageOfShaderDataUpdaterFunc, void(*updShaderDataFuncPtr)(void*, ShaderClass*));
		DLLTREATMENT ShaderClass(const ShaderClass&& toCopy);
		DLLTREATMENT ~ShaderClass();
		DLLTREATMENT void operator=(const ShaderClass&& toCopy);

		DLLTREATMENT unsigned int GetUniformIDByName(const char* name);
		
		DLLTREATMENT const Primitives::ShaderProgramClass& gPrimitiveShader() const;
		DLLTREATMENT CFAC_UniformFuncs_Class gCFAC_UniformFuncs() const;

		DLLTREATMENT void Bind();
		DLLTREATMENT void Unbind();
		DLLTREATMENT void Delete();

	};
}
