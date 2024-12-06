#include"Renderer.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"

using namespace Graphics::Primitives;

static unsigned int _GetPrimitive_SwitchCase(RendererNamespace::PrimitivesEnum primitive) {
	using namespace RendererNamespace;
	switch (primitive) {
	case PrimitivesEnum::Points: return GL_POINTS;
	case PrimitivesEnum::LineStrip: return GL_LINE_STRIP;
	case PrimitivesEnum::LineLoop: return GL_LINE_LOOP;
	case PrimitivesEnum::Lines: return GL_LINES;
	case PrimitivesEnum::LineStripAdjacency: return GL_LINE_STRIP_ADJACENCY;
	case PrimitivesEnum::LinesAdjacency: return GL_LINES_ADJACENCY;
	case PrimitivesEnum::TriangleStrip: return GL_TRIANGLE_STRIP;
	case PrimitivesEnum::TriangleFan: return GL_TRIANGLE_FAN;
	case PrimitivesEnum::Triangles: return GL_TRIANGLES;
	case PrimitivesEnum::TriangleStripAdjacency: return GL_TRIANGLE_STRIP_ADJACENCY;
	case PrimitivesEnum::TrianglesAdjacency: return GL_TRIANGLES_ADJACENCY;
	case PrimitivesEnum::Patches: return GL_PATCHES;
	} return 0;
}
void RendererNamespace::DrawArrays(PrimitivesEnum primitive, unsigned int startingInd, unsigned int amountOfVertexesToRender) {
	glSC(glDrawArrays(_GetPrimitive_SwitchCase(primitive), startingInd, amountOfVertexesToRender));
}
void RendererNamespace::DrawArraysInstanced(PrimitivesEnum primitive, unsigned int firstVertexInd, unsigned int amountOfVertexesToRender, unsigned int instancesAmount) {
	glSC(glDrawArraysInstanced(_GetPrimitive_SwitchCase(primitive), firstVertexInd, amountOfVertexesToRender, instancesAmount));
}