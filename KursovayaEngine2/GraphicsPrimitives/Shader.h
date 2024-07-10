#pragma once

enum class ShaderTypesEnum {
	Fragment,Vertex,Geometry
};
class Shader {
	unsigned int ID;
	mutable bool Deleted = false;
	mutable bool Compiled = false;
	ShaderTypesEnum ShaderType;
public:
	unsigned int gID() const;
	Shader(ShaderTypesEnum typ, const char* filePath);
	void Compile();
	~Shader();
	void Delete();
};