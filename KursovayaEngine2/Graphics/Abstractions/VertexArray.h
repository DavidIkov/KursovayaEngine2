#pragma once
#include"DLL.h"
#include"Tools/DynArr.h"
#include"Tools/ArrayView.h"
#include"Graphics/Primitives/VertexArray.h"

namespace Graphics::Abstractions {
	class VertexArrayClass :protected Primitives::VertexArrayClass {
	public:
		using AttributeDataStruct = Primitives::VertexArrayClass::AttributeDataStruct;
	protected:
		DynArr<AttributeDataStruct> AttribsData;
	public:

		DLLTREATMENT VertexArrayClass();
		DLLTREATMENT VertexArrayClass(const ArrayView<AttributeDataStruct>& attribsData);
		DLLTREATMENT VertexArrayClass(const VertexArrayClass& toCopy);
		DLLTREATMENT VertexArrayClass(const VertexArrayClass&& toCopy);
		DLLTREATMENT ~VertexArrayClass();
		DLLTREATMENT VertexArrayClass& operator=(const VertexArrayClass& toCopy);
		DLLTREATMENT VertexArrayClass& operator=(const VertexArrayClass&& toCopy);

		inline const DynArr<AttributeDataStruct>& gAttribsData() const { return AttribsData; }
		
		DLLTREATMENT void SetLayout(const ArrayView<AttributeDataStruct>& attribsData);
		DLLTREATMENT void SetAttribute(const AttributeDataStruct& attribData);
		DLLTREATMENT void EnableAttribute(unsigned int attribInd) const;
		DLLTREATMENT void DisableAttribute(unsigned int attribInd) const;

		DLLTREATMENT void Change_VB_ID_InAttribute(unsigned int attribInd, Primitives::VertexBufferClass::VertexBufferID_Type vb_id);
		DLLTREATMENT void Change_Normalize_InAttribute(unsigned int attribInd, bool normalize);
		DLLTREATMENT void Change_Divisor_InAttribute(unsigned int attribInd, unsigned int divisor);
		DLLTREATMENT void Change_ComponentsAmount_InAttribute(unsigned int attribInd, unsigned int componentsAmount);
		DLLTREATMENT void Change_ByteOffsetToNextElement_InAttribute(unsigned int attribInd, unsigned int byteOffsetToNextElement);
		DLLTREATMENT void Change_DataTypeOnCPU_InAttribute(unsigned int attribInd, AttributeDataStruct::DataTypeOnCPU_Enum dataTypeOnCPU);
		DLLTREATMENT void Change_DataTypeOnGPU_InAttribute(unsigned int attribInd, AttributeDataStruct::DataTypeOnGPU_Enum dataTypeOnGPU);

		inline const AttributeDataStruct& gAttributeData(unsigned int ind) const { return AttribsData[ind]; }
		inline const DynArr<AttributeDataStruct>& gAttributesData() const { return AttribsData; }

		DLLTREATMENT void Delete();

		DLLTREATMENT void Bind() const;
		DLLTREATMENT static void Unbind();


	};
}
