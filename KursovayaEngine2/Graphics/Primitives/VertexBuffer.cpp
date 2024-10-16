#include"VertexBuffer.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"glad/glad.h"
#include"Tools/ErrorCodes.h"
#include"Graphics/Globals.h"
#include"Tools/DebugRuntimeAssert.h"

using namespace Graphics::Primitives;
#define Assert_NotDeleted_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Deleted, "VertexBuffer is deleted", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#if defined KE2_Debug
#define Assert_Binded_Macro if (DebugRuntimeAssert(DebuggingTools::ErrorTypes::Warning, BindedInstances.gVertexBufferID() == ID, "VertexBuffer is not binded", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION)) Bind();
#else
#define Assert_Binded_Macro
#endif

VertexBufferClass::VertexBufferClass() {
    glSC(glGenBuffers(1, &ID));
    Bind();
}
VertexBufferClass::VertexBufferClass(const VertexBufferClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexBufferClass));
    toCopy.Deleted = true;
}
void VertexBufferClass::operator=(const VertexBufferClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexBufferClass));
    toCopy.Deleted = true;
}
VertexBufferClass::~VertexBufferClass() {
    if (not Deleted) {
        glSC(glDeleteBuffers(1, &ID));
        Deleted = true;
    }
}
void VertexBufferClass::Delete() {
    Assert_NotDeleted_Macro;
    this->~VertexBufferClass();
}
unsigned int VertexBufferClass::gID() {
    Assert_NotDeleted_Macro;
    return ID;
}
void VertexBufferClass::SetLayout(BufferDataTypeEnum dataType, const std::vector<unsigned int>& layout) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
#if defined KE2_Debug
    DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, BindedInstances.gVertexArrayID() != 0, "changing layout of VertexBuffer without any VertexArray binded dosent make any sence", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION);
#endif

	for (unsigned int i = 0; i < EnabledAttributesAmount; i++) { glSC(glDisableVertexAttribArray(i)); }
	EnabledAttributesAmount = (unsigned short int)layout.size();

	unsigned int sumOfLayout = 0; for (unsigned int i = 0; i < layout.size(); i++) sumOfLayout += layout[i];

	unsigned int gl_dataType = 0;
	switch (dataType) {
	case BufferDataTypeEnum::Byte: gl_dataType = GL_BYTE; break;
	case BufferDataTypeEnum::UnsignedByte: gl_dataType = GL_UNSIGNED_BYTE; break;
	case BufferDataTypeEnum::Float: gl_dataType = GL_FLOAT; break;
	case BufferDataTypeEnum::Int: gl_dataType = GL_INT; break;
	case BufferDataTypeEnum::UnsignedInt: gl_dataType = GL_UNSIGNED_INT; break;
	}

	float* off = 0;
	for (unsigned int i = 0; i < layout.size(); i++) {
		glSC(glVertexAttribPointer(i, layout[i], GL_FLOAT, GL_FALSE, sumOfLayout * sizeof(float), (void*)off));
		glSC(glEnableVertexAttribArray(i));
		off += layout[i];
	}
}

static unsigned int _GetVBUsageForGL(VertexBufferClass::BufferDataUsageEnum usage) {
    switch (usage) {
    case VertexBufferClass::BufferDataUsageEnum::StreamDraw: return GL_STREAM_DRAW;
    case VertexBufferClass::BufferDataUsageEnum::StreamRead: return GL_STREAM_READ;
    case VertexBufferClass::BufferDataUsageEnum::StreamCopy: return GL_STREAM_COPY;
    case VertexBufferClass::BufferDataUsageEnum::StaticDraw: return GL_STATIC_DRAW;
    case VertexBufferClass::BufferDataUsageEnum::StaticRead: return GL_STATIC_READ;
    case VertexBufferClass::BufferDataUsageEnum::StaticCopy: return GL_STATIC_COPY;
    case VertexBufferClass::BufferDataUsageEnum::DynamicDraw: return GL_DYNAMIC_DRAW;
    case VertexBufferClass::BufferDataUsageEnum::DynamicRead: return GL_DYNAMIC_READ;
    case VertexBufferClass::BufferDataUsageEnum::DynamicCopy: return GL_DYNAMIC_COPY;
    }
    return 0;
}
void VertexBufferClass::SetData(unsigned int dataLength, const void* data, const BufferDataUsageEnum usage) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

	glSC(glBufferData(GL_ARRAY_BUFFER, dataLength, data, _GetVBUsageForGL(usage)));
}

void VertexBufferClass::SetSubData(unsigned int offset, unsigned int dataLength, void* firstElement) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

	glSC(glBufferSubData(GL_ARRAY_BUFFER, offset, dataLength, firstElement));
}
void VertexBufferClass::Bind() {
    Assert_NotDeleted_Macro;
#if defined KE2_Debug
    BindedInstances.sVertexBuffer_ID(ID);
#endif
	glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
}
void VertexBufferClass::Unbind() {
    glSC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}