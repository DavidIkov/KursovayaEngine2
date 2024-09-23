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
		DLLTREATMENT unsigned int gID() const;
		DLLTREATMENT ShaderProgramClass();
		DLLTREATMENT ShaderProgramClass(const ShaderProgramClass* toCopy);
		DLLTREATMENT ShaderProgramClass(const ShaderProgramClass&& toCopy);
		DLLTREATMENT void operator=(const ShaderProgramClass&& toCopy);
		DLLTREATMENT void AttachShader(const ShaderClass& SH);
		DLLTREATMENT void LinkShaders();
		DLLTREATMENT ~ShaderProgramClass();
		DLLTREATMENT void Delete();
		DLLTREATMENT void Bind() const;
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

	};
}