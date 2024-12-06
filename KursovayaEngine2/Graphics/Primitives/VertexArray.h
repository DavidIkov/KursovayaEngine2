#pragma once
#include"DLL.h"
#include"Tools/ErrorsSystem.h"
#include"Tools/ArrayView.h"
#include"VertexBuffer.h"

namespace Graphics::Primitives {

	class VertexArrayClass {
	protected:
		unsigned int ID = 0u;
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				ZeroVB_CantBeUsed,
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

		struct AttributeDataStruct {
			unsigned int AttribInd;
			//vertex buffer ID
			VertexBufferClass::VertexBufferID_Type VB_ID;
			/* used only if DataOnOnGPU is Float,
			if true then values will be converted to floats with value/maxValueOfThisType,
			otherwise they are converted to floats without any divisions */
			bool Normalize = false;
			/* amount of insances that have to be passed to use next
			vertex buffer data, for example if divisor is 1 then
			every instance data will update, if 2 then every 2 instances and etc,
			used for instanced rendering
			if Divisor is 0 then data will update every vertex(most often usecase) 
			donst ask why name is like that, thats how this is named in OpenGL*/
			unsigned int Divisor;
			/* amount of components for a attribute, for example if in result
			you want to have vec2 then this should be 2, vec3 then 3 and etc,
			if just 1 value then 1 by same logic */
			unsigned int ComponentsAmount;
			unsigned int FirstElementByteOffset;
			/* byte offset representing where is next value will be located */
			unsigned int ByteOffsetToNextElement;
			enum class DataTypeInMemory_Enum :unsigned char {
				Byte, UnsignedByte, Float, Int, UnsignedInt, Double
			};
			DataTypeInMemory_Enum DataTypeInMemory;
			enum class DataTypeForReadingOnGPU_Enum :unsigned char {
				Float,Int,Double
			};
			DataTypeForReadingOnGPU_Enum DataTypeForReadingOnGPU;
		};

		DLLTREATMENT VertexArrayClass();
		DLLTREATMENT VertexArrayClass(const ArrayView<AttributeDataStruct>& attribsData);
		DLLTREATMENT VertexArrayClass(VertexArrayClass&& toCopy) noexcept;
		DLLTREATMENT VertexArrayClass& operator=(VertexArrayClass&& toCopy);
		DLLTREATMENT virtual ~VertexArrayClass() noexcept(false);

		DLLTREATMENT void SetAttributes(const ArrayView<AttributeDataStruct>& attribsData) const;
		DLLTREATMENT void SetAttribute(const AttributeDataStruct& attribData) const;
		DLLTREATMENT void EnableAttribute(unsigned int attribInd) const;
		DLLTREATMENT void DisableAttribute(unsigned int attribInd) const;

		inline unsigned int gID() const noexcept { return ID; }
		DLLTREATMENT void Bind() const;
		DLLTREATMENT static void Unbind();

	};
}