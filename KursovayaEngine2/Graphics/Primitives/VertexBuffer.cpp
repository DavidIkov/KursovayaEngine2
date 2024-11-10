#include"VertexBuffer.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;
#define Assert_NotDeleted_Macro if (Deleted) ErrorsSystemNamespace::SendError<<"VertexBuffer is already deleted">>ErrorsEnumWrapperStruct(ErrorsEnum::AlreadyDeleted);
#if defined KE2_Debug
#define Assert_Binded_Macro if(BindedInstances.gVertexBufferID() != ID) ErrorsSystemNamespace::SendWarning<<"VertexBuffer is not binded">>ErrorsSystemNamespace::EndOfWarning;
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
    Delete();
    memcpy(this, &toCopy, sizeof(VertexBufferClass));
    toCopy.Deleted = true;
}
VertexBufferClass::~VertexBufferClass() {
    if (not Deleted) {
        Unbind();
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
static unsigned int _GetSizeOfTypeByBufferDataTypeEnum_SwitchCase(VertexBufferClass::LayoutDataStruct::DataTypeEnum typ) {
    switch (typ) {
    case VertexBufferClass::LayoutDataStruct::DataTypeEnum::Byte: return sizeof(unsigned char);
    case VertexBufferClass::LayoutDataStruct::DataTypeEnum::UnsignedByte: return sizeof(unsigned char);
    case VertexBufferClass::LayoutDataStruct::DataTypeEnum::Float: return sizeof(float);
    case VertexBufferClass::LayoutDataStruct::DataTypeEnum::Int: return sizeof(int);
    case VertexBufferClass::LayoutDataStruct::DataTypeEnum::UnsignedInt: return sizeof(unsigned int);
    }
    return 0;
}
void VertexBufferClass::SetLayout(const DynArr<LayoutDataStruct>& layout) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
#if defined KE2_Debug
    if (BindedInstances.gVertexArrayID() == 0) ErrorsSystemNamespace::SendError << "changing layout of VertexBuffer without any VertexArray binded dosent make any sence" >> ErrorsEnumWrapperStruct(ErrorsEnum::ChangingLayoutOfVertexBufferWithoutAnyVertexArrayBinded);
#endif

	for (unsigned int i = 0; i < EnabledAttributesAmount; i++) { glSC(glDisableVertexAttribArray(i)); }
	EnabledAttributesAmount = (unsigned short int)layout.gLength();

    unsigned int totalLayoutSize = 0; for (unsigned int i = 0; i < layout.gLength(); i++)
        totalLayoutSize += layout[i].ComponentsAmount * _GetSizeOfTypeByBufferDataTypeEnum_SwitchCase(layout[i].DataType);

	

	unsigned char* off = 0;
	for (unsigned int i = 0; i < layout.gLength(); i++) {

		unsigned int gl_dataType = 0;
		switch (layout[i].DataType) {
		case LayoutDataStruct::DataTypeEnum::Byte: gl_dataType = GL_BYTE; break;
		case LayoutDataStruct::DataTypeEnum::UnsignedByte: gl_dataType = GL_UNSIGNED_BYTE; break;
		case LayoutDataStruct::DataTypeEnum::Float: gl_dataType = GL_FLOAT; break;
		case LayoutDataStruct::DataTypeEnum::Int: gl_dataType = GL_INT; break;
		case LayoutDataStruct::DataTypeEnum::UnsignedInt: gl_dataType = GL_UNSIGNED_INT; break;
		}

		glSC(glVertexAttribPointer(i, layout[i].ComponentsAmount, gl_dataType, GL_FALSE, totalLayoutSize, (void*)off));
		glSC(glEnableVertexAttribArray(i));
		off += _GetSizeOfTypeByBufferDataTypeEnum_SwitchCase(layout[i].DataType)*layout[i].ComponentsAmount;
	}
}

static unsigned int _GetVBUsageForGL(VertexBufferClass::BufferReadWriteModeEnum usage) {
    switch (usage) {
    case VertexBufferClass::BufferReadWriteModeEnum::None: return 0;
    case VertexBufferClass::BufferReadWriteModeEnum::StreamDraw: return GL_STREAM_DRAW;
    case VertexBufferClass::BufferReadWriteModeEnum::StreamRead: return GL_STREAM_READ;
    case VertexBufferClass::BufferReadWriteModeEnum::StreamCopy: return GL_STREAM_COPY;
    case VertexBufferClass::BufferReadWriteModeEnum::StaticDraw: return GL_STATIC_DRAW;
    case VertexBufferClass::BufferReadWriteModeEnum::StaticRead: return GL_STATIC_READ;
    case VertexBufferClass::BufferReadWriteModeEnum::StaticCopy: return GL_STATIC_COPY;
    case VertexBufferClass::BufferReadWriteModeEnum::DynamicDraw: return GL_DYNAMIC_DRAW;
    case VertexBufferClass::BufferReadWriteModeEnum::DynamicRead: return GL_DYNAMIC_READ;
    case VertexBufferClass::BufferReadWriteModeEnum::DynamicCopy: return GL_DYNAMIC_COPY;
    }
    return 0;
}
void VertexBufferClass::SetData(const void* data, unsigned int dataSizeInBytes, const BufferReadWriteModeEnum bufferReadWriteMode) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

    if (bufferReadWriteMode == BufferReadWriteModeEnum::None) ErrorsSystemNamespace::SendError << "BufferReadWriteMode is none" >> ErrorsEnumWrapperStruct(ErrorsEnum::BufferReadWriteModeInNone);
	glSC(glBufferData(GL_ARRAY_BUFFER, dataSizeInBytes, data, _GetVBUsageForGL(bufferReadWriteMode)));
}
void VertexBufferClass::SetSubData(unsigned int offsetInBytes, const void* data, unsigned int dataSizeInBytes) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

	glSC(glBufferSubData(GL_ARRAY_BUFFER, offsetInBytes, dataSizeInBytes, data));
}

void VertexBufferClass::CopySubData(const VertexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy) {
    Assert_NotDeleted_Macro;
    
    glSC(glBindBuffer(GL_COPY_READ_BUFFER, srcBuffer.ID));
    glSC(glBindBuffer(GL_COPY_WRITE_BUFFER, ID));

    glSC(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, srcOffsetInBytes, dstOffsetInBytes, amountOfBytesToCopy));

    glSC(glBindBuffer(GL_COPY_READ_BUFFER, 0));
    glSC(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
}

void VertexBufferClass::GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

    glSC(glGetBufferSubData(GL_ARRAY_BUFFER, offsetInBytes, amountOfBytesToCopy, data));
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
#if defined KE2_Debug
    BindedInstances.sVertexBuffer_ID(0);
#endif
}