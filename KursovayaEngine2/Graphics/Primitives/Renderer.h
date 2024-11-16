#pragma once
#include"DLL.h"

namespace Graphics::Primitives {

	namespace RendererNamespace {
		enum class PrimitivesEnum {
			Points, LineStrip, LineLoop, Lines, LineStripAdjacency, LinesAdjacency, TriangleStrip,
			TriangleFan, Triangles, TriangleStripAdjacency, TrianglesAdjacency, Patches
		};
		DLLTREATMENT void DrawArrays(PrimitivesEnum primitive, unsigned int firstVertexInd, unsigned int amountOfVertexesToRender);
		DLLTREATMENT void DrawArraysInstanced(PrimitivesEnum primitive, unsigned int firstVertexInd, unsigned int amountOfVertexesToRender, unsigned int instancesAmount);
	}
}