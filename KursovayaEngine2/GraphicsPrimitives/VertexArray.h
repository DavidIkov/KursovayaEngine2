#pragma once
#include"DLL.h"
class VertexArray {
	unsigned int ID = 0;
	mutable bool Deleted = false;
public:
	DLLTREATMENT VertexArray();
	DLLTREATMENT VertexArray(const VertexArray* toCopy);
	DLLTREATMENT VertexArray(const VertexArray&& toCopy);
	DLLTREATMENT void operator=(const VertexArray&& toCopy);
	DLLTREATMENT ~VertexArray();
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT void Delete();
	DLLTREATMENT void Bind() const;
	DLLTREATMENT static void Unbind();
};