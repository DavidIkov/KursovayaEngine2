#pragma once
#include"DLL.h"
class VertexArray {
	unsigned int ID = 0;
	mutable bool Deleted = false;
public:
	DLLTREATMENT VertexArray();
	DLLTREATMENT VertexArray(VertexArray&& tempVA);
	DLLTREATMENT ~VertexArray();
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT void Delete();
	DLLTREATMENT void Bind();
	DLLTREATMENT static void Unbind();
};