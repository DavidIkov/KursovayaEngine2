#include"Renderer.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"

using namespace Graphics::Primitives;

void RendererNamespace::DrawArrays(PrimitivesEnum primitive, unsigned int startingInd, unsigned int amountOfVertexesToRender) {
	unsigned int glPrimitive = 0;
	
	switch (primitive) {
	case PrimitivesEnum::Points: glPrimitive = GL_POINTS; break;
	case PrimitivesEnum::LineStrip: glPrimitive = GL_LINE_STRIP; break;
	case PrimitivesEnum::LineLoop: glPrimitive = GL_LINE_LOOP; break;
	case PrimitivesEnum::Lines: glPrimitive = GL_LINES; break;
	case PrimitivesEnum::LineStripAdjacency: glPrimitive = GL_LINE_STRIP_ADJACENCY; break;
	case PrimitivesEnum::LinesAdjacency: glPrimitive = GL_LINES_ADJACENCY; break;
	case PrimitivesEnum::TriangleStrip: glPrimitive = GL_TRIANGLE_STRIP; break;
	case PrimitivesEnum::TriangleFan: glPrimitive = GL_TRIANGLE_FAN; break;
	case PrimitivesEnum::Triangles: glPrimitive = GL_TRIANGLES; break;
	case PrimitivesEnum::TriangleStripAdjacency: glPrimitive = GL_TRIANGLE_STRIP_ADJACENCY; break;
	case PrimitivesEnum::TrianglesAdjacency: glPrimitive = GL_TRIANGLES_ADJACENCY; break;
	case PrimitivesEnum::Patches: glPrimitive = GL_PATCHES; break;
	}
	glSC(glDrawArrays(glPrimitive, startingInd, amountOfVertexesToRender));
}