#pragma once
#include"DLL.h"

namespace Graphics::Primitives {

	namespace RendererNamespace {
		enum class PrimitivesEnum :unsigned char {
			Points, LineStrip, LineLoop, Lines, LineStripAdjacency, LinesAdjacency, TriangleStrip,
			TriangleFan, Triangles, TriangleStripAdjacency, TrianglesAdjacency, Patches
		};
		enum class IndexBufferDataTypeEnum :unsigned char {
			UnsignedByte, UnsignedShort, UnsignedInt
		};
		DLLTREATMENT void DrawArrays(PrimitivesEnum primitive, unsigned int firstVertexInd, unsigned int amountOfVertexesToRender);
		DLLTREATMENT void DrawArraysInstanced(PrimitivesEnum primitive, unsigned int firstVertexInd, unsigned int amountOfVertexesToRender, unsigned int instancesAmount);
		DLLTREATMENT void DrawElements(PrimitivesEnum primitive, unsigned int firstIndexInd, unsigned int amountOfIndexesToRender, IndexBufferDataTypeEnum indexBufferDataType);
		DLLTREATMENT void DrawElementsInstanced(PrimitivesEnum primitive, unsigned int firstIndexInd, unsigned int amountOfIndexesToRender, IndexBufferDataTypeEnum indexBufferDataType, unsigned int instancesAmount);

	}
}