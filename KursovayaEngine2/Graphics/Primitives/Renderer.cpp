#include"Renderer.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"

using namespace Graphics::Primitives;

static unsigned int constexpr _GetPrimitive_SwitchCase(RendererNamespace::PrimitivesEnum primitive) noexcept {
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
static unsigned int constexpr _GetIndexBufferDataType_SwitchCase(RendererNamespace::IndexBufferDataTypeEnum indexBufferDataType) noexcept {
	using namespace RendererNamespace;
	switch (indexBufferDataType)
	{
	case Graphics::Primitives::RendererNamespace::IndexBufferDataTypeEnum::UnsignedByte: return GL_UNSIGNED_BYTE;
	case Graphics::Primitives::RendererNamespace::IndexBufferDataTypeEnum::UnsignedShort: return GL_UNSIGNED_SHORT;
	case Graphics::Primitives::RendererNamespace::IndexBufferDataTypeEnum::UnsignedInt: return GL_UNSIGNED_INT;
	default: return 0;
	}
}
void RendererNamespace::DrawArrays(PrimitivesEnum primitive, unsigned int startingInd, unsigned int amountOfVertexesToRender) {
	glSC(glDrawArrays(_GetPrimitive_SwitchCase(primitive), startingInd, amountOfVertexesToRender));
}
void RendererNamespace::DrawArraysInstanced(PrimitivesEnum primitive, unsigned int firstVertexInd, unsigned int amountOfVertexesToRender, unsigned int instancesAmount) {
	glSC(glDrawArraysInstanced(_GetPrimitive_SwitchCase(primitive), firstVertexInd, amountOfVertexesToRender, instancesAmount));
}
void RendererNamespace::DrawElements(PrimitivesEnum primitive, unsigned int firstIndexInd, unsigned int amountOfIndexesToRender, IndexBufferDataTypeEnum indexBufferDataType) {
	glSC(glDrawElements(_GetPrimitive_SwitchCase(primitive), amountOfIndexesToRender, _GetIndexBufferDataType_SwitchCase(indexBufferDataType), (const void*)(long long)firstIndexInd));
}
void RendererNamespace::DrawElementsInstanced(PrimitivesEnum primitive, unsigned int firstIndexInd, unsigned int amountOfIndexesToRender, IndexBufferDataTypeEnum indexBufferDataType, unsigned int instancesAmount) {
	glSC(glDrawElementsInstanced(_GetPrimitive_SwitchCase(primitive), amountOfIndexesToRender, _GetIndexBufferDataType_SwitchCase(indexBufferDataType), (const void*)(long long)firstIndexInd, instancesAmount));
}
