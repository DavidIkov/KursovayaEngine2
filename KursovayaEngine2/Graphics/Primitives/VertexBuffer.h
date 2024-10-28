#pragma once
#include"DLL.h"
#include<vector>
#include"Tools/ClassFunctionsAccessController.h"
#include"Tools/DynArr.h"
#include"Tools/AnonDynArr.h"

namespace Graphics::Primitives {

	class VertexBufferClass {
	protected:
		unsigned int ID = 0;
		mutable bool Deleted = false;
		unsigned short int EnabledAttributesAmount = 0;

	public:
		enum class BufferDataUsageEnum :unsigned short int {
			StreamDraw, StreamRead, StreamCopy,
			StaticDraw, StaticRead, StaticCopy,
			DynamicDraw, DynamicRead, DynamicCopy,
		};
		enum class BufferDataTypeEnum :unsigned short int {
			Byte, UnsignedByte, Float, Int, UnsignedInt
		};

		DLLTREATMENT VertexBufferClass();
		DLLTREATMENT VertexBufferClass(const VertexBufferClass&) = delete;
		DLLTREATMENT VertexBufferClass(const VertexBufferClass&& toCopy);
		DLLTREATMENT void operator=(const VertexBufferClass&& toCopy);
		DLLTREATMENT ~VertexBufferClass();
		DLLTREATMENT void Delete();
		DLLTREATMENT unsigned int gID();

		struct LayoutDataStruct {
			unsigned int ComponentsAmount;
			BufferDataTypeEnum DataType;
		};
		DLLTREATMENT void SetLayout(const DynArr<LayoutDataStruct>& layout);
		DLLTREATMENT void SetData(const AnonDynArr& data, const BufferDataUsageEnum usage);
		DLLTREATMENT void SetSubData(unsigned int offsetInBytes, const AnonDynArr& data);
		DLLTREATMENT void Bind();
		DLLTREATMENT static void Unbind();

#define CFAC_ClassName VertexBufferClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncConstr(gID)
			CFAC_FuncConstr(SetLayout)
			CFAC_FuncConstr(SetData)
			CFAC_FuncConstr(SetSubData)
			CFAC_FuncConstr(Bind)
			CFAC_FuncConstr(Unbind)
		);
#undef CFAC_ClassName
	};
}