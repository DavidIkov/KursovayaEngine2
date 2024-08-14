#pragma once
#include"DLL.h"

enum class ShaderTypesEnum {
	Fragment,Vertex,Geometry
};

/*note that there are 2 similar constructors, if you want to read from file, 
then firstly specify direction of file and then shader type,
but if you want to type source code yourself then firstly specify shader type and then actual code*/
class Shader {
	unsigned int ID;
	mutable bool Deleted = false;
	mutable bool Compiled = false;
	ShaderTypesEnum ShaderType;
public:
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT Shader(const char* filePath, ShaderTypesEnum typ);
	DLLTREATMENT Shader(ShaderTypesEnum typ, const char* code);
	DLLTREATMENT Shader(Shader&& tempS);
	DLLTREATMENT void Compile();
	DLLTREATMENT ~Shader();
	DLLTREATMENT void Delete();
};