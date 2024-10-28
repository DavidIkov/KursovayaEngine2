#pragma once
#include"DLL.h"
#include"Graphics/Primitives/Texture.h"
#include"Tools/ClassFunctionsAccessController.h"
#include"Tools/AnonDynArr.h"

namespace Graphics::Abstractions {

	//make sure that if you dont use some axes in size specification of texture then 
	//you dont leave them as 0, use 1, otherwise it will mean that texture is empty
	class TextureClass : protected Primitives::TextureClass {
	public:
		typedef Primitives::TextureClass::SettingsStruct SettingsStruct;
		typedef Primitives::TextureClass::DataSettingsStruct DataSettingsStruct;
		typedef Primitives::TextureClass::DimensionsEnum DimensionsEnum;
	protected:
		bool DataFormatOnGPU_WasUpdated = false;
		Vector3U Size;
		SettingsStruct Settings;
		DataSettingsStruct DataSettings;
	public:
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

		DLLTREATMENT SettingsStruct gSettings();
		DLLTREATMENT void sSettings(SettingsStruct newSets);

		DLLTREATMENT void sDataSettings_DataFormatOnGPU(DataSettingsStruct::DataFormatOnGPU_Enum dataFormat);
		DLLTREATMENT DataSettingsStruct::DataFormatOnGPU_Enum gDataSettings_DataFormatOnGPU();
		DLLTREATMENT void sDataSettings_DataFormatOnCPU(DataSettingsStruct::DataFormatOnCPU_Enum dataFormat);
		DLLTREATMENT DataSettingsStruct::DataFormatOnCPU_Enum gDataSettings_DataFormatOnCPU();
		DLLTREATMENT void sDataSettings_DataTypeOnCPU(DataSettingsStruct::DataTypeOnCPU_Enum dataType);
		DLLTREATMENT DataSettingsStruct::DataTypeOnCPU_Enum gDataSettings_DataTypeOnCPU();

		DLLTREATMENT DataSettingsStruct gDataSettings();
		DLLTREATMENT void sDataSettings(DataSettingsStruct newDataSets);

		DLLTREATMENT void Delete();
		DLLTREATMENT void Bind(unsigned int bindingInd = 0);
		DLLTREATMENT void Unbind();

		DLLTREATMENT const Primitives::TextureClass& gPrimitiveTexture();
		DLLTREATMENT Primitives::TextureClass::CFAC_FullAccess_Class gPrimitiveTextureCFAC();

#define CFAC_ClassName TextureClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncConstr(ChangeData)
			CFAC_FuncConstr(ChangeSubData)
			CFAC_FuncConstr(gSize)
			CFAC_FuncConstr(sSettings_WrapTypeByX) CFAC_FuncConstr(gSettings_WrapTypeByX)
			CFAC_FuncConstr(sSettings_WrapTypeByY) CFAC_FuncConstr(gSettings_WrapTypeByY)
			CFAC_FuncConstr(sSettings_DownscalingFilt) CFAC_FuncConstr(gSettings_DownscalingFilt)
			CFAC_FuncConstr(sSettings_UpscalingFilt) CFAC_FuncConstr(gSettings_UpscalingFilt)
			CFAC_FuncConstr(sSettings_DepthStencilReadMode) CFAC_FuncConstr(gSettings_DepthStencilReadMode)
			CFAC_FuncConstr(sDataSettings_DataFormatOnGPU) CFAC_FuncConstr(gDataSettings_DataFormatOnGPU)
			CFAC_FuncConstr(sDataSettings_DataFormatOnCPU) CFAC_FuncConstr(gDataSettings_DataFormatOnCPU)
			CFAC_FuncConstr(sDataSettings_DataTypeOnCPU) CFAC_FuncConstr(gDataSettings_DataTypeOnCPU)
			CFAC_FuncConstr(Bind)
			CFAC_FuncConstr(Unbind)
		);
#undef CFAC_ClassName

	};
}



