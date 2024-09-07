#pragma once
#include"DLL.h"

class Shader {
	unsigned int ID;
	mutable bool Deleted = false;
#if defined Debug
	mutable bool Compiled = false;
#endif
public:
	enum class TypesEnum :unsigned short int {
		Fragment, Vertex, Geometry
	};
private:
#if defined Debug
	TypesEnum ShaderType;
#endif
public:
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT Shader(const wchar_t* filePath, TypesEnum typ);
	DLLTREATMENT Shader(TypesEnum typ, const char* code);
	DLLTREATMENT Shader(const Shader* toCopy);
	DLLTREATMENT Shader(const Shader&& toCopy);
	DLLTREATMENT void operator=(const Shader&& toCopy);
	DLLTREATMENT void Compile();
	DLLTREATMENT ~Shader();
	DLLTREATMENT void Delete();
};