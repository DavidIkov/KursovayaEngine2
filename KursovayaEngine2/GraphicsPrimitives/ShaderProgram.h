#pragma once
#include"Shader.h"

namespace GraphicsPrimitives {

	class ShaderProgramClass {
		unsigned int ID;
#if defined Debug
		mutable bool Linked = false;
#endif
		mutable bool Deleted = false;
	public:
		DLLTREATMENT unsigned int gID();
		DLLTREATMENT ShaderProgramClass();
		DLLTREATMENT ShaderProgramClass(RespConstrFlag, const ShaderProgramClass& toCopy);
		DLLTREATMENT ShaderProgramClass(const ShaderProgramClass&& toCopy);
		DLLTREATMENT void operator=(const ShaderProgramClass&& toCopy);
		DLLTREATMENT void AttachShader(ShaderClass::CFAC_FullAccess_Class shaderCFAC);
		DLLTREATMENT void LinkShaders();
		DLLTREATMENT ~ShaderProgramClass();
		DLLTREATMENT void Delete();
		DLLTREATMENT void Bind();
		DLLTREATMENT static void Unbind();

		DLLTREATMENT void SetUniform1f(const char* name, float v0);
		DLLTREATMENT void SetUniform2f(const char* name, float v0, float v1);
		DLLTREATMENT void SetUniform3f(const char* name, float v0, float v1, float v2);
		DLLTREATMENT void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);
		DLLTREATMENT void SetUniform1i(const char* name, int v0);
		DLLTREATMENT void SetUniform2i(const char* name, int v0, int v1);
		DLLTREATMENT void SetUniform3i(const char* name, int v0, int v1, int v2);
		DLLTREATMENT void SetUniform4i(const char* name, int v0, int v1, int v2, int v3);
		DLLTREATMENT void SetUniform1ui(const char* name, unsigned int v0);
		DLLTREATMENT void SetUniform2ui(const char* name, unsigned int v0, unsigned int v1);
		DLLTREATMENT void SetUniform3ui(const char* name, unsigned int v0, unsigned int v1, unsigned int v2);
		DLLTREATMENT void SetUniform4ui(const char* name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);
		DLLTREATMENT void SetUniform1fv(const char* name, unsigned int count, const float* value);
		DLLTREATMENT void SetUniform2fv(const char* name, unsigned int count, const float* value);
		DLLTREATMENT void SetUniform3fv(const char* name, unsigned int count, const float* value);
		DLLTREATMENT void SetUniform4fv(const char* name, unsigned int count, const float* value);
		DLLTREATMENT void SetUniform1iv(const char* name, unsigned int count, const int* value);
		DLLTREATMENT void SetUniform2iv(const char* name, unsigned int count, const int* value);
		DLLTREATMENT void SetUniform3iv(const char* name, unsigned int count, const int* value);
		DLLTREATMENT void SetUniform4iv(const char* name, unsigned int count, const int* value);
		DLLTREATMENT void SetUniform1uiv(const char* name, unsigned int count, const unsigned int* value);
		DLLTREATMENT void SetUniform2uiv(const char* name, unsigned int count, const unsigned int* value);
		DLLTREATMENT void SetUniform3uiv(const char* name, unsigned int count, const unsigned int* value);
		DLLTREATMENT void SetUniform4uiv(const char* name, unsigned int count, const unsigned int* value);
		DLLTREATMENT void SetUniformMatrix2fv(const char* name, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix3fv(const char* name, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix4fv(const char* name, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix2x3fv(const char* name, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix3x2fv(const char* name, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix2x4fv(const char* name, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix4x2fv(const char* name, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix3x4fv(const char* name, unsigned int count, bool transpose, const float* value);
		DLLTREATMENT void SetUniformMatrix4x3fv(const char* name, unsigned int count, bool transpose, const float* value);

#define CFAC_ClassName ShaderProgramClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncPtrConstr(gID)
			CFAC_FuncPtrConstr(Bind)
			CFAC_FuncPtrConstr(Unbind)
			CFAC_FuncPtrConstr(AttachShader)
			CFAC_FuncPtrConstr(LinkShaders)
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