#pragma once
#include"Shader.h"
#include<string>
#include"Tools/DynArr.h"

namespace Graphics::Primitives {

	class ShaderProgramClass {
		unsigned int ID;
#if defined KE2_Debug
		mutable bool Linked = false;
#endif
		mutable bool Deleted = false;
	public:
		DLLTREATMENT ShaderProgramClass();
		DLLTREATMENT ShaderProgramClass(const ShaderProgramClass&& toCopy);
		DLLTREATMENT void operator=(const ShaderProgramClass&& toCopy);
		DLLTREATMENT void AttachShader(unsigned int id);
		DLLTREATMENT void LinkShaders();
		DLLTREATMENT ~ShaderProgramClass();
		DLLTREATMENT unsigned int gID();
		DLLTREATMENT void Delete();
		DLLTREATMENT void Bind();
		DLLTREATMENT static void Unbind();

		struct UniformData {
			unsigned int Type;
			unsigned int Size;
			std::string Name;
			unsigned int ID;
		};

		DLLTREATMENT unsigned int GetUniformIDByName(const char* name);
		DLLTREATMENT UniformData GetUniformData(unsigned int index);
		DLLTREATMENT void GetUniformsData(DynArr<UniformData>* dynArr);

		DLLTREATMENT void SetUniform1f(unsigned int uniformID, float v0);
		DLLTREATMENT void SetUniform2f(unsigned int uniformID, float v0, float v1);
		DLLTREATMENT void SetUniform3f(unsigned int uniformID, float v0, float v1, float v2);
		DLLTREATMENT void SetUniform4f(unsigned int uniformID, float v0, float v1, float v2, float v3);
		DLLTREATMENT void SetUniform1i(unsigned int uniformID, int v0);
		DLLTREATMENT void SetUniform2i(unsigned int uniformID, int v0, int v1);
		DLLTREATMENT void SetUniform3i(unsigned int uniformID, int v0, int v1, int v2);
		DLLTREATMENT void SetUniform4i(unsigned int uniformID, int v0, int v1, int v2, int v3);
		DLLTREATMENT void SetUniform1ui(unsigned int uniformID, unsigned int v0);
		DLLTREATMENT void SetUniform2ui(unsigned int uniformID, unsigned int v0, unsigned int v1);
		DLLTREATMENT void SetUniform3ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2);
		DLLTREATMENT void SetUniform4ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);
		DLLTREATMENT void SetUniform1fv(unsigned int uniformID, unsigned int count, const float* value);
		DLLTREATMENT void SetUniform2fv(unsigned int uniformID, unsigned int count, const float* value);
		DLLTREATMENT void SetUniform3fv(unsigned int uniformID, unsigned int count, const float* value);
		DLLTREATMENT void SetUniform4fv(unsigned int uniformID, unsigned int count, const float* value);
		DLLTREATMENT void SetUniform1iv(unsigned int uniformID, unsigned int count, const int* value);
		DLLTREATMENT void SetUniform2iv(unsigned int uniformID, unsigned int count, const int* value);
		DLLTREATMENT void SetUniform3iv(unsigned int uniformID, unsigned int count, const int* value);
		DLLTREATMENT void SetUniform4iv(unsigned int uniformID, unsigned int count, const int* value);
		DLLTREATMENT void SetUniform1uiv(unsigned int uniformID, unsigned int count, const unsigned int* value);
		DLLTREATMENT void SetUniform2uiv(unsigned int uniformID, unsigned int count, const unsigned int* value);
		DLLTREATMENT void SetUniform3uiv(unsigned int uniformID, unsigned int count, const unsigned int* value);
		DLLTREATMENT void SetUniform4uiv(unsigned int uniformID, unsigned int count, const unsigned int* value);
		DLLTREATMENT void SetUniformMatrix2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix2x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix3x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix2x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix4x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix3x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix4x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value);

#define CFAC_ClassName ShaderProgramClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncPtrConstr(Bind)
			CFAC_FuncPtrConstr(Unbind)
			CFAC_FuncPtrConstr(AttachShader)
			CFAC_FuncPtrConstr(LinkShaders)
			CFAC_FuncPtrConstr(gID)
			CFAC_FuncPtrConstr(SetUniform1f) CFAC_FuncPtrConstr(SetUniform2f) CFAC_FuncPtrConstr(SetUniform3f) CFAC_FuncPtrConstr(SetUniform4f)
			CFAC_FuncPtrConstr(SetUniform1i) CFAC_FuncPtrConstr(SetUniform2i) CFAC_FuncPtrConstr(SetUniform3i) CFAC_FuncPtrConstr(SetUniform4i)
			CFAC_FuncPtrConstr(SetUniform1ui) CFAC_FuncPtrConstr(SetUniform2ui) CFAC_FuncPtrConstr(SetUniform3ui) CFAC_FuncPtrConstr(SetUniform4ui)
			CFAC_FuncPtrConstr(SetUniform1fv) CFAC_FuncPtrConstr(SetUniform2fv) CFAC_FuncPtrConstr(SetUniform3fv) CFAC_FuncPtrConstr(SetUniform4fv)
			CFAC_FuncPtrConstr(SetUniform1iv) CFAC_FuncPtrConstr(SetUniform2iv) CFAC_FuncPtrConstr(SetUniform3iv) CFAC_FuncPtrConstr(SetUniform4iv)
			CFAC_FuncPtrConstr(SetUniform1uiv) CFAC_FuncPtrConstr(SetUniform2uiv) CFAC_FuncPtrConstr(SetUniform3uiv) CFAC_FuncPtrConstr(SetUniform4uiv)
			CFAC_FuncPtrConstr(SetUniformMatrix2fv) CFAC_FuncPtrConstr(SetUniformMatrix3fv) CFAC_FuncPtrConstr(SetUniformMatrix4fv)
			CFAC_FuncPtrConstr(SetUniformMatrix2x3fv) CFAC_FuncPtrConstr(SetUniformMatrix3x2fv) CFAC_FuncPtrConstr(SetUniformMatrix2x4fv)
			CFAC_FuncPtrConstr(SetUniformMatrix4x2fv) CFAC_FuncPtrConstr(SetUniformMatrix3x4fv) CFAC_FuncPtrConstr(SetUniformMatrix4x3fv)
		);
		CFAC_ClassConstructor(UniformFuncsAccess,
			CFAC_FuncPtrConstr(SetUniform1f) CFAC_FuncPtrConstr(SetUniform2f) CFAC_FuncPtrConstr(SetUniform3f) CFAC_FuncPtrConstr(SetUniform4f)
			CFAC_FuncPtrConstr(SetUniform1i) CFAC_FuncPtrConstr(SetUniform2i) CFAC_FuncPtrConstr(SetUniform3i) CFAC_FuncPtrConstr(SetUniform4i)
			CFAC_FuncPtrConstr(SetUniform1ui) CFAC_FuncPtrConstr(SetUniform2ui) CFAC_FuncPtrConstr(SetUniform3ui) CFAC_FuncPtrConstr(SetUniform4ui)
			CFAC_FuncPtrConstr(SetUniform1fv) CFAC_FuncPtrConstr(SetUniform2fv) CFAC_FuncPtrConstr(SetUniform3fv) CFAC_FuncPtrConstr(SetUniform4fv)
			CFAC_FuncPtrConstr(SetUniform1iv) CFAC_FuncPtrConstr(SetUniform2iv) CFAC_FuncPtrConstr(SetUniform3iv) CFAC_FuncPtrConstr(SetUniform4iv)
			CFAC_FuncPtrConstr(SetUniform1uiv) CFAC_FuncPtrConstr(SetUniform2uiv) CFAC_FuncPtrConstr(SetUniform3uiv) CFAC_FuncPtrConstr(SetUniform4uiv)
			CFAC_FuncPtrConstr(SetUniformMatrix2fv) CFAC_FuncPtrConstr(SetUniformMatrix3fv) CFAC_FuncPtrConstr(SetUniformMatrix4fv)
			CFAC_FuncPtrConstr(SetUniformMatrix2x3fv) CFAC_FuncPtrConstr(SetUniformMatrix3x2fv) CFAC_FuncPtrConstr(SetUniformMatrix2x4fv)
			CFAC_FuncPtrConstr(SetUniformMatrix4x2fv) CFAC_FuncPtrConstr(SetUniformMatrix3x4fv) CFAC_FuncPtrConstr(SetUniformMatrix4x3fv)
		);

#undef CFAC_ClassName
	};
}