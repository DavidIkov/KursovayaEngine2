#include"Renderer.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"

using namespace Graphics::Primitives;

void RendererNamespace::DrawArrays(PrimitivesEnum primitive, unsigned int startingInd, unsigned int amountOfVertexesToRender) {
	unsigned int glPrimitive = 0;
	switch (primitive) {
#define switchCaseForPrimitiveEnumMacro(prim,primName) case PrimitivesEnum::prim: {glPrimitive=primName; break; }
		switchCaseForPrimitiveEnumMacro(Points, GL_POINTS);
		switchCaseForPrimitiveEnumMacro(LineStrip, GL_LINE_STRIP);
		switchCaseForPrimitiveEnumMacro(LineLoop, GL_LINE_LOOP);
		switchCaseForPrimitiveEnumMacro(Lines, GL_LINES);
		switchCaseForPrimitiveEnumMacro(LineStripAdjacency, GL_LINE_STRIP_ADJACENCY);
		switchCaseForPrimitiveEnumMacro(LinesAdjacency, GL_LINES_ADJACENCY);
		switchCaseForPrimitiveEnumMacro(TriangleStrip, GL_TRIANGLE_STRIP);
		switchCaseForPrimitiveEnumMacro(TriangleFan, GL_TRIANGLE_FAN);
		switchCaseForPrimitiveEnumMacro(Triangles, GL_TRIANGLES);
		switchCaseForPrimitiveEnumMacro(TriangleStripAdjacency, GL_TRIANGLE_STRIP_ADJACENCY);
		switchCaseForPrimitiveEnumMacro(TrianglesAdjacency, GL_TRIANGLES_ADJACENCY);
		switchCaseForPrimitiveEnumMacro(Patches, GL_PATCHES);
#undef switchCaseForPrimitiveEnumMacro
	}
	glSC(glDrawArrays(glPrimitive, startingInd, amountOfVertexesToRender));
}