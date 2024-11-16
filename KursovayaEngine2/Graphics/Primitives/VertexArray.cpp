#include"VertexArray.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;

#define Assert_NotDeleted_Macro if (Deleted) ErrorsSystemNamespace::SendError<<"VertexArray is already deleted">>ErrorsEnumWrapperStruct(ErrorsEnum::AlreadyDeleted);
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
VertexArrayClass::VertexArrayClass(const VertexArrayClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexArrayClass));
    toCopy.Deleted = true;
}
void VertexArrayClass::operator=(const VertexArrayClass&& toCopy) {
    Delete();
    memcpy(this, &toCopy, sizeof(VertexArrayClass));
    toCopy.Deleted = true;
}
VertexArrayClass::~VertexArrayClass() {
    if (not Deleted) {
        Unbind();
        glSC(glDeleteVertexArrays(1, &ID));
        Deleted = true;
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
void VertexArrayClass::SetAttribute(const AttributeDataStruct& attribData) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
	if (attribData.VB_ID == 0)
		ErrorsSystemNamespace::SendError << "Cant use zero vertex buffer for attribute" >> ErrorsEnumWrapperStruct(ErrorsEnum::ZeroVB_CantBeUsed);

    EnableAttribute(attribData.AttribInd);

	glSC(glBindBuffer(GL_ARRAY_BUFFER, attribData.VB_ID));
	unsigned int glDataTypeOnCPU = 0;
	switch (attribData.DataTypeOnCPU) {
	case AttributeDataStruct::DataTypeOnCPU_Enum::Byte: glDataTypeOnCPU = GL_BYTE; break;
	case AttributeDataStruct::DataTypeOnCPU_Enum::UnsignedByte: glDataTypeOnCPU = GL_UNSIGNED_BYTE; break;
	case AttributeDataStruct::DataTypeOnCPU_Enum::Float: glDataTypeOnCPU = GL_FLOAT; break;
	case AttributeDataStruct::DataTypeOnCPU_Enum::Int: glDataTypeOnCPU = GL_INT; break;
	case AttributeDataStruct::DataTypeOnCPU_Enum::UnsignedInt: glDataTypeOnCPU = GL_UNSIGNED_INT; break;
	case AttributeDataStruct::DataTypeOnCPU_Enum::Double: glDataTypeOnCPU = GL_DOUBLE; break;
	}

	switch (attribData.DataTypeOnGPU) {
	case AttributeDataStruct::DataTypeOnGPU_Enum::Float: {
		glSC(glVertexAttribPointer(attribData.AttribInd, attribData.ComponentsAmount, glDataTypeOnCPU, attribData.Normalize, attribData.ByteOffsetToNextElement, (const void*)(unsigned long long int)attribData.FirstElementByteOffset));
		break;
	} case AttributeDataStruct::DataTypeOnGPU_Enum::Int: {
		glSC(glVertexAttribIPointer(attribData.AttribInd, attribData.ComponentsAmount, glDataTypeOnCPU, attribData.ByteOffsetToNextElement, (const void*)(unsigned long long int)attribData.FirstElementByteOffset));
		break;
	} case AttributeDataStruct::DataTypeOnGPU_Enum::Double: {
		glSC(glVertexAttribLPointer(attribData.AttribInd, attribData.ComponentsAmount, glDataTypeOnCPU, attribData.ByteOffsetToNextElement, (const void*)(unsigned long long int)attribData.FirstElementByteOffset));
		break;
	}
	}

	glSC(glVertexAttribDivisor(attribData.AttribInd, attribData.Divisor));
}
void VertexArrayClass::SetAttributes(const ArrayView<AttributeDataStruct>& attribsData) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

    unsigned int beforeFunctionBindedVB_ID = BindedInstances.gVertexBufferID();

    for (unsigned int i = 0; i < attribsData.gLen(); i++)
        SetAttribute(attribsData[i]);

    glBindBuffer(GL_ARRAY_BUFFER, beforeFunctionBindedVB_ID);

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
void VertexArrayClass::EnableAttribute(unsigned int attribInd) const {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
    glSC(glEnableVertexAttribArray(attribInd));
}
void VertexArrayClass::DisableAttribute(unsigned int attribInd) const {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
    glSC(glDisableVertexAttribArray(attribInd));
}
unsigned int VertexArrayClass::gID() const {
    Assert_NotDeleted_Macro;
    return ID;
}
void VertexArrayClass::Delete() {
    Assert_NotDeleted_Macro;
	this->~VertexArrayClass();
}
void VertexArrayClass::Bind() const {
    Assert_NotDeleted_Macro;
#if defined KE2_Debug
    BindedInstances.sVertexArray_ID(ID);
#endif
	glSC(glBindVertexArray(ID));
}
void VertexArrayClass::Unbind() {
    glSC(glBindVertexArray(0));
#if defined KE2_Debug
    BindedInstances.sVertexArray_ID(0);
#endif
}