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
};