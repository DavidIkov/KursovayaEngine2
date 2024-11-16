#include"VertexArray.h"

using namespace KE2;
using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

VertexArrayClass::VertexArrayClass() {};
VertexArrayClass::VertexArrayClass(const ArrayView<AttributeDataStruct>& attribsData) :
	GP::VertexArrayClass(attribsData) {};
VertexArrayClass::VertexArrayClass(const VertexArrayClass& toCopy) :
	GP::VertexArrayClass(toCopy.AttribsData), AttribsData(toCopy.AttribsData) {};
VertexArrayClass::VertexArrayClass(const VertexArrayClass&& toCopy) :
	GP::VertexArrayClass(std::move(toCopy)), AttribsData(std::move(toCopy.AttribsData)) {};
VertexArrayClass::~VertexArrayClass(){}
VertexArrayClass& VertexArrayClass::operator=(const VertexArrayClass& toCopy) {
	Delete();
	new(this) VertexArrayClass(toCopy);
	return *this;
}
VertexArrayClass& VertexArrayClass::operator=(const VertexArrayClass&& toCopy) {
	Delete();
	new(this) VertexArrayClass(std::move(toCopy));
	return *this;
}

void VertexArrayClass::SetLayout(const ArrayView<AttributeDataStruct>& attribsData) {
	GP::VertexArrayClass::SetAttributes(attribsData);
}
void VertexArrayClass::SetAttribute(const AttributeDataStruct& attribData) {
	GP::VertexArrayClass::SetAttribute(attribData);
}
void VertexArrayClass::EnableAttribute(unsigned int attribInd) const {
	GP::VertexArrayClass::EnableAttribute(attribInd);
}
void VertexArrayClass::DisableAttribute(unsigned int attribInd) const {
	GP::VertexArrayClass::DisableAttribute(attribInd);
}
void VertexArrayClass::Change_VB_ID_InAttribute(unsigned int attribInd, GP::VertexBufferClass::VertexBufferID_Type vb_id) { AttribsData[attribInd].VB_ID = vb_id; SetAttribute(AttribsData[attribInd]); }
void VertexArrayClass::Change_Normalize_InAttribute(unsigned int attribInd, bool normalize) { AttribsData[attribInd].Normalize = normalize; SetAttribute(AttribsData[attribInd]); }
void VertexArrayClass::Change_Divisor_InAttribute(unsigned int attribInd, unsigned int divisor) { AttribsData[attribInd].Divisor = divisor; SetAttribute(AttribsData[attribInd]); }
void VertexArrayClass::Change_ComponentsAmount_InAttribute(unsigned int attribInd, unsigned int componentsAmount) { AttribsData[attribInd].ComponentsAmount = componentsAmount; SetAttribute(AttribsData[attribInd]); }
void VertexArrayClass::Change_ByteOffsetToNextElement_InAttribute(unsigned int attribInd, unsigned int byteOffsetToNextElement) { AttribsData[attribInd].ByteOffsetToNextElement = byteOffsetToNextElement; SetAttribute(AttribsData[attribInd]); }
void VertexArrayClass::Change_DataTypeOnCPU_InAttribute(unsigned int attribInd, AttributeDataStruct::DataTypeOnCPU_Enum dataTypeOnCPU) { AttribsData[attribInd].DataTypeOnCPU = dataTypeOnCPU; SetAttribute(AttribsData[attribInd]); }
void VertexArrayClass::Change_DataTypeOnGPU_InAttribute(unsigned int attribInd, AttributeDataStruct::DataTypeOnGPU_Enum dataTypeOnGPU) { AttribsData[attribInd].DataTypeOnGPU = dataTypeOnGPU; SetAttribute(AttribsData[attribInd]); }



void VertexArrayClass::Delete() {
	this->~VertexArrayClass();
}

void VertexArrayClass::Bind() const {
	GP::VertexArrayClass::Bind();
}
void VertexArrayClass::Unbind() {
	GP::VertexArrayClass::Unbind();
}

