#pragma once
#include"Shader.h"

class ShaderProgram {
	unsigned int ID;
	mutable bool Linked = false;
	mutable bool Deleted = false;
public:
	unsigned int gID() const;
	ShaderProgram();
	void AttachShader(const Shader& SH);
	void LinkShaders();
	~ShaderProgram();
	void Delete();
	void Bind() const;
	static void Unbind();

	void SetUniform1f(const char* name, float v0);
	void SetUniform2f(const char* name, float v0, float v1);
	void SetUniform3f(const char* name, float v0, float v1, float v2);
	void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const char* name, int v0);
	void SetUniform2i(const char* name, int v0, int v1);
	void SetUniform3i(const char* name, int v0, int v1, int v2);
	void SetUniform4i(const char* name, int v0, int v1, int v2, int v3);
	void SetUniform1ui(const char* name, unsigned int v0);
	void SetUniform2ui(const char* name, unsigned int v0, unsigned int v1);
	void SetUniform3ui(const char* name, unsigned int v0, unsigned int v1, unsigned int v2);
	void SetUniform4ui(const char* name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);
	void SetUniform1fv(const char* name, unsigned int count, const float* value);
	void SetUniform2fv(const char* name, unsigned int count, const float* value);
	void SetUniform3fv(const char* name, unsigned int count, const float* value);
	void SetUniform4fv(const char* name, unsigned int count, const float* value);
	void SetUniform1iv(const char* name, unsigned int count, const int* value);
	void SetUniform2iv(const char* name, unsigned int count, const int* value);
	void SetUniform3iv(const char* name, unsigned int count, const int* value);
	void SetUniform4iv(const char* name, unsigned int count, const int* value);
	void SetUniform1uiv(const char* name, unsigned int count, const unsigned int* value);
	void SetUniform2uiv(const char* name, unsigned int count, const unsigned int* value);
	void SetUniform3uiv(const char* name, unsigned int count, const unsigned int* value);
	void SetUniform4uiv(const char* name, unsigned int count, const unsigned int* value);
	void SetUniformMatrix2fv(const char* name, unsigned int count, bool transpose, const float* value);
	void SetUniformMatrix3fv(const char* name, unsigned int count, bool transpose, const float* value);
	void SetUniformMatrix4fv(const char* name, unsigned int count, bool transpose, const float* value);
	void SetUniformMatrix2x3fv(const char* name, unsigned int count, bool transpose, const float* value);
	void SetUniformMatrix3x2fv(const char* name, unsigned int count, bool transpose, const float* value);
	void SetUniformMatrix2x4fv(const char* name, unsigned int count, bool transpose, const float* value);
	void SetUniformMatrix4x2fv(const char* name, unsigned int count, bool transpose, const float* value);
	void SetUniformMatrix3x4fv(const char* name, unsigned int count, bool transpose, const float* value);
	void SetUniformMatrix4x3fv(const char* name, unsigned int count, bool transpose, const float* value);

};