#pragma once
#include"Shader.h"
#include<string>
#include"Tools/DynArr.h"
#include"Tools/ClassFunctionsAccessController.h"

namespace Graphics::Primitives {

	class ShaderProgramClass {
	protected:
		unsigned int ID = 0u;
#ifdef KE2_Debug
		mutable bool Linked = false;
#endif
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
#ifdef KE2_Debug
				AlreadyLinked,
				NotLinked,
#endif
				FailedToLinkShaders,
				UniformNameIsTooLarge,
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

		DLLTREATMENT ShaderProgramClass();
		DLLTREATMENT ShaderProgramClass(const wchar_t* vsPath, const wchar_t* fsPath);
		DLLTREATMENT ShaderProgramClass(const wchar_t* vsPath, const wchar_t* gsPath, const wchar_t* fsPath);
		DLLTREATMENT ShaderProgramClass(ShaderProgramClass&& toCopy) noexcept;
		DLLTREATMENT ShaderProgramClass& operator=(ShaderProgramClass&& toCopy);
		DLLTREATMENT void AttachShader(unsigned int id) const;
		DLLTREATMENT void LinkShaders() const;
		DLLTREATMENT virtual ~ShaderProgramClass() noexcept(false);
		inline unsigned int gID() const noexcept { return ID; };

		DLLTREATMENT void Bind() const;
		DLLTREATMENT static void Unbind();

		struct UniformData {
			unsigned int Type;
			unsigned int Size;
			std::string Name;
			unsigned int ID;
		};


		DLLTREATMENT unsigned int GetUniformIDByName(const char* name) const;
		DLLTREATMENT UniformData GetUniformData(unsigned int index) const;
		DLLTREATMENT void GetUniformsData(DynArr<UniformData>* dynArr) const;

		DLLTREATMENT void SetUniform1f(unsigned int uniformID, float v0) const;
		DLLTREATMENT void SetUniform2f(unsigned int uniformID, float v0, float v1) const;
		DLLTREATMENT void SetUniform3f(unsigned int uniformID, float v0, float v1, float v2) const;
		DLLTREATMENT void SetUniform4f(unsigned int uniformID, float v0, float v1, float v2, float v3) const;
		DLLTREATMENT void SetUniform1i(unsigned int uniformID, int v0) const;
		DLLTREATMENT void SetUniform2i(unsigned int uniformID, int v0, int v1) const;
		DLLTREATMENT void SetUniform3i(unsigned int uniformID, int v0, int v1, int v2) const;
		DLLTREATMENT void SetUniform4i(unsigned int uniformID, int v0, int v1, int v2, int v3) const;
		DLLTREATMENT void SetUniform1ui(unsigned int uniformID, unsigned int v0) const;
		DLLTREATMENT void SetUniform2ui(unsigned int uniformID, unsigned int v0, unsigned int v1) const;
		DLLTREATMENT void SetUniform3ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2) const;
		DLLTREATMENT void SetUniform4ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const;
		DLLTREATMENT void SetUniform1fv(unsigned int uniformID, unsigned int count, const float* value) const;
		DLLTREATMENT void SetUniform2fv(unsigned int uniformID, unsigned int count, const float* value) const;
		DLLTREATMENT void SetUniform3fv(unsigned int uniformID, unsigned int count, const float* value) const;
		DLLTREATMENT void SetUniform4fv(unsigned int uniformID, unsigned int count, const float* value) const;
		DLLTREATMENT void SetUniform1iv(unsigned int uniformID, unsigned int count, const int* value) const;
		DLLTREATMENT void SetUniform2iv(unsigned int uniformID, unsigned int count, const int* value) const;
		DLLTREATMENT void SetUniform3iv(unsigned int uniformID, unsigned int count, const int* value) const;
		DLLTREATMENT void SetUniform4iv(unsigned int uniformID, unsigned int count, const int* value) const;
		DLLTREATMENT void SetUniform1uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const;
		DLLTREATMENT void SetUniform2uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const;
		DLLTREATMENT void SetUniform3uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const;
		DLLTREATMENT void SetUniform4uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const;
		DLLTREATMENT void SetUniformMatrix2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
		DLLTREATMENT void SetUniformMatrix3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
		DLLTREATMENT void SetUniformMatrix4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
		DLLTREATMENT void SetUniformMatrix2x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
		DLLTREATMENT void SetUniformMatrix3x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
		DLLTREATMENT void SetUniformMatrix2x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
		DLLTREATMENT void SetUniformMatrix4x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
		DLLTREATMENT void SetUniformMatrix3x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
		DLLTREATMENT void SetUniformMatrix4x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;

		inline void SetUniform1f(const char* uniformName, float v0) const { SetUniform1f(GetUniformIDByName(uniformName), v0); }
		inline void SetUniform2f(const char* uniformName, float v0, float v1) const { SetUniform2f(GetUniformIDByName(uniformName), v0, v1); }
		inline void SetUniform3f(const char* uniformName, float v0, float v1, float v2) const { SetUniform3f(GetUniformIDByName(uniformName), v0, v1, v2); }
		inline void SetUniform4f(const char* uniformName, float v0, float v1, float v2, float v3) const { SetUniform4f(GetUniformIDByName(uniformName), v0, v1, v2, v3); }
		inline void SetUniform1i(const char* uniformName, int v0) const { SetUniform1i(GetUniformIDByName(uniformName), v0); }
		inline void SetUniform2i(const char* uniformName, int v0, int v1) const { SetUniform2i(GetUniformIDByName(uniformName), v0, v1); }
		inline void SetUniform3i(const char* uniformName, int v0, int v1, int v2) const { SetUniform3i(GetUniformIDByName(uniformName), v0, v1, v2); }
		inline void SetUniform4i(const char* uniformName, int v0, int v1, int v2, int v3) const { SetUniform4i(GetUniformIDByName(uniformName), v0, v1, v2, v3); }
		inline void SetUniform1ui(const char* uniformName, unsigned int v0) const { SetUniform1ui(GetUniformIDByName(uniformName), v0); }
		inline void SetUniform2ui(const char* uniformName, unsigned int v0, unsigned int v1) const { SetUniform2ui(GetUniformIDByName(uniformName), v0, v1); }
		inline void SetUniform3ui(const char* uniformName, unsigned int v0, unsigned int v1, unsigned int v2) const { SetUniform3ui(GetUniformIDByName(uniformName), v0, v1, v2); }
		inline void SetUniform4ui(const char* uniformName, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const { SetUniform4ui(GetUniformIDByName(uniformName), v0, v1, v2, v3); }
		inline void SetUniform1fv(const char* uniformName, unsigned int count, const float* value) const { SetUniform1fv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniform2fv(const char* uniformName, unsigned int count, const float* value) const { SetUniform2fv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniform3fv(const char* uniformName, unsigned int count, const float* value) const { SetUniform3fv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniform4fv(const char* uniformName, unsigned int count, const float* value) const { SetUniform4fv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniform1iv(const char* uniformName, unsigned int count, const int* value) const { SetUniform1iv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniform2iv(const char* uniformName, unsigned int count, const int* value) const { SetUniform2iv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniform3iv(const char* uniformName, unsigned int count, const int* value) const { SetUniform3iv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniform4iv(const char* uniformName, unsigned int count, const int* value) const { SetUniform4iv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniform1uiv(const char* uniformName, unsigned int count, const unsigned int* value) const { SetUniform1uiv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniform2uiv(const char* uniformName, unsigned int count, const unsigned int* value) const { SetUniform2uiv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniform3uiv(const char* uniformName, unsigned int count, const unsigned int* value) const { SetUniform3uiv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniform4uiv(const char* uniformName, unsigned int count, const unsigned int* value) const { SetUniform4uiv(GetUniformIDByName(uniformName), count, value); }
		inline void SetUniformMatrix2fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix2fv(GetUniformIDByName(uniformName), count, transpose, value); }
		inline void SetUniformMatrix3fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix3fv(GetUniformIDByName(uniformName), count, transpose, value); }
		inline void SetUniformMatrix4fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix4fv(GetUniformIDByName(uniformName), count, transpose, value); }
		inline void SetUniformMatrix2x3fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix2x3fv(GetUniformIDByName(uniformName), count, transpose, value); }
		inline void SetUniformMatrix3x2fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix3x2fv(GetUniformIDByName(uniformName), count, transpose, value); }
		inline void SetUniformMatrix2x4fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix2x4fv(GetUniformIDByName(uniformName), count, transpose, value); }
		inline void SetUniformMatrix4x2fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix4x2fv(GetUniformIDByName(uniformName), count, transpose, value); }
		inline void SetUniformMatrix3x4fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix3x4fv(GetUniformIDByName(uniformName), count, transpose, value); }
		inline void SetUniformMatrix4x3fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix4x3fv(GetUniformIDByName(uniformName), count, transpose, value); }

#define CFAC_ClassName ShaderProgramClass
		CFAC_ClassConstructor(UniformFuncs,
			CFAC_FuncConstr(SetUniform1f) CFAC_FuncConstr(SetUniform2f) CFAC_FuncConstr(SetUniform3f) CFAC_FuncConstr(SetUniform4f)
			CFAC_FuncConstr(SetUniform1i) CFAC_FuncConstr(SetUniform2i) CFAC_FuncConstr(SetUniform3i) CFAC_FuncConstr(SetUniform4i)
			CFAC_FuncConstr(SetUniform1ui) CFAC_FuncConstr(SetUniform2ui) CFAC_FuncConstr(SetUniform3ui) CFAC_FuncConstr(SetUniform4ui)
			CFAC_FuncConstr(SetUniform1fv) CFAC_FuncConstr(SetUniform2fv) CFAC_FuncConstr(SetUniform3fv) CFAC_FuncConstr(SetUniform4fv)
			CFAC_FuncConstr(SetUniform1iv) CFAC_FuncConstr(SetUniform2iv) CFAC_FuncConstr(SetUniform3iv) CFAC_FuncConstr(SetUniform4iv)
			CFAC_FuncConstr(SetUniform1uiv) CFAC_FuncConstr(SetUniform2uiv) CFAC_FuncConstr(SetUniform3uiv) CFAC_FuncConstr(SetUniform4uiv)
			CFAC_FuncConstr(SetUniformMatrix2fv) CFAC_FuncConstr(SetUniformMatrix3fv) CFAC_FuncConstr(SetUniformMatrix4fv)
			CFAC_FuncConstr(SetUniformMatrix2x3fv) CFAC_FuncConstr(SetUniformMatrix3x2fv) CFAC_FuncConstr(SetUniformMatrix2x4fv)
			CFAC_FuncConstr(SetUniformMatrix4x2fv) CFAC_FuncConstr(SetUniformMatrix3x4fv) CFAC_FuncConstr(SetUniformMatrix4x3fv)
		);

#undef CFAC_ClassName
	};
}