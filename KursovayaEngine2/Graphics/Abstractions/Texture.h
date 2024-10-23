#pragma once
#include"DLL.h"
#include"Graphics/Primitives/Texture.h"
#include"Tools/ClassFunctionsAccessController.h"
#include"Tools/AnonDynArr.h"

namespace Graphics::Abstractions {

	//this class can store data of texture in RAM if you want so.
	class TextureClass {
		mutable bool Deleted = false;
		Primitives::TextureClass Texture;
		Vector3U Size;
		AnonDynArr TexData;
		bool IsStoringTexData = false;
	public:
		typedef Primitives::TextureClass::SettingsStruct SettingsStruct;
		typedef Primitives::TextureClass::DataSettingsStruct DataSettingsStruct;
	private:
		SettingsStruct Settings;
		DataSettingsStruct DataSettings;
		bool DataFormatOnGPU_WasUpdated = false;
	public:
	typedef Primitives::TextureClass::DimensionsEnum DimensionsEnum;
	private:
		DimensionsEnum Dimensions;
	public:
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, bool storeData, const char* filePath, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
		//uses move constructor to copy AnynDynArr if storeData=true
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, bool storeData, AnonDynArr* data, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
		template<typename TexStoreType = void>
		TextureClass(const TextureClass& toCopy, AnonDynArr::TypeContainer<TexStoreType>) :
		Size(toCopy.Size), IsStoringTexData(toCopy.IsStoringTexData), Settings(toCopy.Settings), DataSettings(toCopy.DataSettings), 
		Dimensions(toCopy.Dimensions), Texture(Dimensions, Size, toCopy.TexData.gArr(), Settings, DataSettings) {
			TexData = AnonDynArr(toCopy.TexData, AnonDynArr::TypeContainer<TexStoreType>);
		}
		DLLTREATMENT TextureClass(const TextureClass&& toCopy);
		DLLTREATMENT ~TextureClass();
		//if Texture is not holding data then TexStoreType can be not specified
		template<typename TexStoreType = void>
		void operator=(const TextureClass& toCopy) {
			Delete();
			Deleted = false;
			
			if (toCopy.TexData.gArr() != nullptr) TexData = AnonDynArr(toCopy.TexData, AnonDynArr::TypeContainer<TexStoreType>);
			Size = toCopy.Size;
			IsStoringTexData = toCopy.IsStoringTexData;
			Settings = toCopy.Settings;
			DataSettings = toCopy.DataSettings;
			Dimensions = toCopy.Dimensions;
			Texture = Primitives::TextureClass(Dimensions, Size, TexData, Settings, DataSettings);
		}
		DLLTREATMENT void operator=(const TextureClass&& toCopy);
		
		//if texture is not storing data then dataSizeInBits can be kept zero
		//if texture is storing data then responsibility for "data" will be taken
		//if "data" and actual texture data memory overlap then a the overlap region will be "freed" and
		//then this memory can be overtaken which will result in undefined behaviour
		DLLTREATMENT void ChangeData(Vector3U newSize, void* data, unsigned int dataSizeInBits);
		//if texture is storing data then "data" will be copyed
		DLLTREATMENT void ChangeSubData(Vector3U offset, Vector3U size, const void* data);
		//you can manually update data in GPU's texture only if data is stored 
		//in texture and you changed some data in memory of data manually
		DLLTREATMENT void UpdateDataInTexture();

		DLLTREATMENT AnonDynArr& GetData();

		DLLTREATMENT void StartStoringData();
		//will free currently stored data if there is any
		DLLTREATMENT void StopStoringData();

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
			CFAC_FuncPtrConstr(ChangeData)
			CFAC_FuncPtrConstr(ChangeSubData)
			CFAC_FuncPtrConstr(UpdateDataInTexture)
			CFAC_FuncPtrConstr(GetData)
			CFAC_FuncPtrConstr(StartStoringData)
			CFAC_FuncPtrConstr(StopStoringData)
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



