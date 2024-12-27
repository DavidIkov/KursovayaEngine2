#include"VertexArray.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;

#if defined KE2_Debug
#define Assert_Binded_Macro if (BindedInstances.gVertexArrayID() != ID) ErrorsSystemNamespace::SendWarning << "VertexArray is not binded" >> ErrorsSystemNamespace::EndOfWarning;
#else
#define Assert_Binded_Macro
#endif

VertexArrayClass::VertexArrayClass() {
    glSC(glGenVertexArrays(1, &ID));
    Bind();
}
VertexArrayClass::VertexArrayClass(const ArrayView<AttributeDataStruct>& attribsData) :VertexArrayClass() {
    SetAttributes(attribsData);
}
VertexArrayClass::VertexArrayClass(VertexArrayClass&& toCopy) noexcept :
    ID(toCopy.ID) {
    toCopy.ID = 0u;
}
VertexArrayClass& VertexArrayClass::operator=(VertexArrayClass&& toCopy) {
    this->~VertexArrayClass();
    new(this) VertexArrayClass(std::move(toCopy));
    return *this;
}
VertexArrayClass::~VertexArrayClass() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteVertexArrays(1, &ID));
        ID = 0u;
    }
}
/*
static unsigned int _GetSizeOfTypeByBufferDataTypeEnum_SwitchCase(VertexArrayClass::AttributeDataStruct::DataTypeEnum typ) {
    switch (typ) {
    case VertexArrayClass::AttributeDataStruct::DataTypeEnum::Byte: return sizeof(unsigned char);
    case VertexArrayClass::AttributeDataStruct::DataTypeEnum::UnsignedByte: return sizeof(unsigned char);
    case VertexArrayClass::AttributeDataStruct::DataTypeEnum::Float: return sizeof(float);
    case VertexArrayClass::AttributeDataStruct::DataTypeEnum::Int: return sizeof(int);
    case VertexArrayClass::AttributeDataStruct::DataTypeEnum::UnsignedInt: return sizeof(unsigned int);
    }
    return 0;
}
*/
void VertexArrayClass::SetAttribute(unsigned int attribInd, const AttributeDataStruct& attribData) {
    Assert_Binded_Macro;
	if (attribData.VB_ID == 0)
		ErrorsSystemNamespace::SendError << "Cant use zero vertex buffer for attribute" >> ErrorsEnumWrapperStruct(ErrorsEnum::ZeroVB_CantBeUsed);

    EnableAttribute(attribInd);

	glSC(glBindBuffer(GL_ARRAY_BUFFER, attribData.VB_ID));
	unsigned int glDataTypeOnCPU = 0;
	switch (attribData.DataTypeInMemory) {
	case AttributeDataStruct::DataTypeInMemory_Enum::Byte: glDataTypeOnCPU = GL_BYTE; break;
	case AttributeDataStruct::DataTypeInMemory_Enum::UnsignedByte: glDataTypeOnCPU = GL_UNSIGNED_BYTE; break;
	case AttributeDataStruct::DataTypeInMemory_Enum::Float: glDataTypeOnCPU = GL_FLOAT; break;
	case AttributeDataStruct::DataTypeInMemory_Enum::Int: glDataTypeOnCPU = GL_INT; break;
	case AttributeDataStruct::DataTypeInMemory_Enum::UnsignedInt: glDataTypeOnCPU = GL_UNSIGNED_INT; break;
	case AttributeDataStruct::DataTypeInMemory_Enum::Double: glDataTypeOnCPU = GL_DOUBLE; break;
	}

	switch (attribData.DataTypeForReadingOnGPU) {
	case AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float: {
		glSC(glVertexAttribPointer(attribInd, attribData.ComponentsAmount, glDataTypeOnCPU, attribData.Normalize, attribData.ByteOffsetToNextElement, (const void*)(unsigned long long int)attribData.FirstElementByteOffset));
		break;
	} case AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Int: {
		glSC(glVertexAttribIPointer(attribInd, attribData.ComponentsAmount, glDataTypeOnCPU, attribData.ByteOffsetToNextElement, (const void*)(unsigned long long int)attribData.FirstElementByteOffset));
		break;
	} case AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Double: {
		glSC(glVertexAttribLPointer(attribInd, attribData.ComponentsAmount, glDataTypeOnCPU, attribData.ByteOffsetToNextElement, (const void*)(unsigned long long int)attribData.FirstElementByteOffset));
		break;
	}
	}

	glSC(glVertexAttribDivisor(attribInd, attribData.Divisor));
}
void VertexArrayClass::SetAttributes(const ArrayView<AttributeDataStruct>& attribsData) {
    Assert_Binded_Macro;

    for (unsigned int i = 0; i < attribsData.gLen(); i++)
        SetAttribute(i, attribsData[i]);

    glSC(glBindBuffer(GL_ARRAY_BUFFER, 0));

    /*
        unsigned int last
        unsigned char* off = 0;
        for (unsigned int i = 0; i < EnabledAttributesAmount; i++) {

            unsigned int gl_dataType = 0;
            switch (layout[i].DataType) {
            case AttributeDataStruct::DataTypeEnum::Byte: gl_dataType = GL_BYTE; break;
            case AttributeDataStruct::DataTypeEnum::UnsignedByte: gl_dataType = GL_UNSIGNED_BYTE; break;
            case AttributeDataStruct::DataTypeEnum::Float: gl_dataType = GL_FLOAT; break;
            case AttributeDataStruct::DataTypeEnum::Int: gl_dataType = GL_INT; break;
            case AttributeDataStruct::DataTypeEnum::UnsignedInt: gl_dataType = GL_UNSIGNED_INT; break;
            }

            glSC(glVertexAttribPointer(i, layout[i].ComponentsAmount, gl_dataType, GL_FALSE, totalLayoutSize, (void*)off));
            glSC(glEnableVertexAttribArray(i));
            off += _GetSizeOfTypeByBufferDataTypeEnum_SwitchCase(layout[i].DataType)*layout[i].ComponentsAmount;
        }
        */
}
void VertexArrayClass::EnableAttribute(unsigned int attribInd) {
    Assert_Binded_Macro;
    glSC(glEnableVertexAttribArray(attribInd));
}
void VertexArrayClass::DisableAttribute(unsigned int attribInd) {
    Assert_Binded_Macro;
    glSC(glDisableVertexAttribArray(attribInd));
}
void VertexArrayClass::Bind() const {
#if defined KE2_Debug
    BindedInstances.sVertexArrayID(ID);
#endif
	glSC(glBindVertexArray(ID));
}
void VertexArrayClass::Unbind() {
    glSC(glBindVertexArray(0));
#if defined KE2_Debug
    BindedInstances.sVertexArrayID(0);
#endif
}