#pragma once
#include"DLL.h"
#include"Graphics/Primitives/Texture.h"
#include"Tools/ClassFunctionsAccessController.h"
#include"Tools/AnonDynArr.h"

namespace Graphics::Abstractions {

	//this class can store data of texture in RAM if you want so.
	//make sure that if you dont use some axes in size specification of texture then 
	//you dont leave them as 0, use 1, otherwise it will mean that texture is empty
	class TextureClass : protected Primitives::TextureClass {
	protected:
		Vector3U Size;
		SettingsStruct Settings;
		DataSettingsStruct DataSettings;
		bool DataFormatOnGPU_WasUpdated = false;
	public:
		typedef Primitives::TextureClass::SettingsStruct SettingsStruct;
		typedef Primitives::TextureClass::DataSettingsStruct DataSettingsStruct;
		typedef Primitives::TextureClass::DimensionsEnum DimensionsEnum;
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, const char* filePath, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
		//no taking responsibility for "data"
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
		//will just copy settings and allocate empty texture with same size
		DLLTREATMENT TextureClass(const TextureClass& toCopy);
		DLLTREATMENT TextureClass(const TextureClass&& toCopy);
		DLLTREATMENT ~TextureClass();
		DLLTREATMENT void operator=(const TextureClass& toCopy);
		DLLTREATMENT void operator=(const TextureClass&& toCopy);
		
		DLLTREATMENT void ChangeData(Vector3U newSize, const void* data);
		DLLTREATMENT void ChangeData(const void* data);
		DLLTREATMENT void ChangeSubData(Vector3U offset, const void* data, Vector3U pixelsAmount);
		
		//if space is not enough for a copy then only a part that can be fitted will be copyed
		DLLTREATMENT void CopyDataFromOtherTexture(const TextureClass& toCopy);
		DLLTREATMENT void CopySubDataFromOtherTexture(const TextureClass& srcTex, Vector3U offsetInSrc, Vector3U offsetInDst, Vector3U pixelsAmount);

		//buffer should point to already allocated memory
		DLLTREATMENT void GetData(void* buffer);
		//buffer should point to already allocated memory
		DLLTREATMENT void GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount);

		DLLTREATMENT Vector3U gSize();

		DLLTREATMENT void sSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX);
		DLLTREATMENT SettingsStruct::WrapTypeEnum gSettings_WrapTypeByX();
		DLLTREATMENT void sSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY);
		DLLTREATMENT SettingsStruct::WrapTypeEnum gSettings_WrapTypeByY();
		DLLTREATMENT void sSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt);
		DLLTREATMENT SettingsStruct::DownscalingFilterFuncEnum gSettings_DownscalingFilt();
		DLLTREATMENT void sSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt);
		DLLTREATMENT SettingsStruct::UpscalingFilterFuncEnum gSettings_UpscalingFilt();
		DLLTREATMENT void sSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode);
		DLLTREATMENT SettingsStruct::DepthStencilReadModeEnum gSettings_DepthStencilReadMode();

		//changes will happen when data will
		DLLTREATMENT void sDataSettings_DataFormatOnGPU(DataSettingsStruct::DataFormatOnGPU_Enum dataFormat);
		DLLTREATMENT DataSettingsStruct::DataFormatOnGPU_Enum gDataSettings_DataFormatOnGPU();
		DLLTREATMENT void sDataSettings_DataFormatOnCPU(DataSettingsStruct::DataFormatOnCPU_Enum dataFormat);
		DLLTREATMENT DataSettingsStruct::DataFormatOnCPU_Enum gDataSettings_DataFormatOnCPU();
		DLLTREATMENT void sDataSettings_DataTypeOnCPU(DataSettingsStruct::DataTypeOnCPU_Enum dataType);
		DLLTREATMENT DataSettingsStruct::DataTypeOnCPU_Enum gDataSettings_DataTypeOnCPU();

		DLLTREATMENT void Delete();
		DLLTREATMENT void Bind(unsigned int bindingInd = 0);
		DLLTREATMENT void Unbind();

#define CFAC_ClassName TextureClass
		CFAC_ClassConstructor(FullAccess,
			//CFAC_FuncPtrConstr(ChangeData)
			//CFAC_FuncPtrConstr(ChangeSubData(Vector3U,const void*,Vector3U))
			CFAC_FuncPtrConstr(gSize)
			CFAC_FuncPtrConstr(sSettings_WrapTypeByX) CFAC_FuncPtrConstr(gSettings_WrapTypeByX)
			CFAC_FuncPtrConstr(sSettings_WrapTypeByY) CFAC_FuncPtrConstr(gSettings_WrapTypeByY)
			CFAC_FuncPtrConstr(sSettings_DownscalingFilt) CFAC_FuncPtrConstr(gSettings_DownscalingFilt)
			CFAC_FuncPtrConstr(sSettings_UpscalingFilt) CFAC_FuncPtrConstr(gSettings_UpscalingFilt)
			CFAC_FuncPtrConstr(sSettings_DepthStencilReadMode) CFAC_FuncPtrConstr(gSettings_DepthStencilReadMode)
			CFAC_FuncPtrConstr(sDataSettings_DataFormatOnGPU) CFAC_FuncPtrConstr(gDataSettings_DataFormatOnGPU)
			CFAC_FuncPtrConstr(sDataSettings_DataFormatOnCPU) CFAC_FuncPtrConstr(gDataSettings_DataFormatOnCPU)
			CFAC_FuncPtrConstr(sDataSettings_DataTypeOnCPU) CFAC_FuncPtrConstr(gDataSettings_DataTypeOnCPU)
			CFAC_FuncPtrConstr(Bind)
			CFAC_FuncPtrConstr(Unbind)
		);
#undef CFAC_ClassName

	};
}



