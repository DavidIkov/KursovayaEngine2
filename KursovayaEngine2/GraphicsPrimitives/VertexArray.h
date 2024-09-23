#pragma once
#include"DLL.h"
class VertexArrayClass {
	unsigned int ID = 0;
	mutable bool Deleted = false;
public:
	DLLTREATMENT VertexArrayClass();
	DLLTREATMENT VertexArrayClass(const VertexArrayClass* toCopy);
	DLLTREATMENT VertexArrayClass(const VertexArrayClass&& toCopy);
	DLLTREATMENT void operator=(const VertexArrayClass&& toCopy);
	DLLTREATMENT ~VertexArrayClass();
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT void Delete();
	DLLTREATMENT void Bind() const;
	DLLTREATMENT static void Unbind();
};