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
		DLLTREATMENT VertexArrayClass(VertexArrayClass&& toCopy) noexcept;
		DLLTREATMENT virtual ~VertexArrayClass() noexcept(false) override;
		DLLTREATMENT VertexArrayClass& operator=(const VertexArrayClass& toCopy);
		DLLTREATMENT VertexArrayClass& operator=(VertexArrayClass&& toCopy);

		inline const DynArr<AttributeDataStruct>& gAttribsData() const noexcept { return AttribsData; }
		
		DLLTREATMENT void SetAttributes(const ArrayView<AttributeDataStruct>& attribsData);
		DLLTREATMENT void SetAttribute(const AttributeDataStruct& attribData);
		DLLTREATMENT void EnableAttribute(unsigned int attribInd) const;
		DLLTREATMENT void DisableAttribute(unsigned int attribInd) const;

		DLLTREATMENT void Change_VB_ID_InAttribute(unsigned int attribInd, Primitives::VertexBufferClass::VertexBufferID_Type vb_id);
		DLLTREATMENT void Change_Normalize_InAttribute(unsigned int attribInd, bool normalize);
		DLLTREATMENT void Change_Divisor_InAttribute(unsigned int attribInd, unsigned int divisor);
		DLLTREATMENT void Change_ComponentsAmount_InAttribute(unsigned int attribInd, unsigned int componentsAmount);
		DLLTREATMENT void Change_ByteOffsetToNextElement_InAttribute(unsigned int attribInd, unsigned int byteOffsetToNextElement);
		DLLTREATMENT void Change_DataTypeInMemory_InAttribute(unsigned int attribInd, AttributeDataStruct::DataTypeInMemory_Enum dataTypeInMemory);
		DLLTREATMENT void Change_DataTypeForReadingOnGPU_InAttribute(unsigned int attribInd, AttributeDataStruct::DataTypeForReadingOnGPU_Enum dataTypeForReadingOnGPU);

		inline const AttributeDataStruct& gAttributeData(unsigned int ind) const noexcept { return AttribsData[ind]; }
		inline const DynArr<AttributeDataStruct>& gAttributesData() const noexcept { return AttribsData; }

		using Primitives::VertexArrayClass::Bind;
		using Primitives::VertexArrayClass::Unbind;

	};
}
