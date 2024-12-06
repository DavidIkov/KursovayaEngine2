#pragma once
#include"DLL.h"
#include"Graphics/Primitives/Texture.h"
#include"Tools/ClassFunctionsAccessController.h"

namespace Graphics::Abstractions {

	//make sure that if you dont use some axes in size specification of texture then 
	//you dont leave them as 0, use 1, otherwise it will mean that texture is empty
	class TextureClass : protected Primitives::TextureClass {
	public:
		using SettingsStruct = Primitives::TextureClass::SettingsStruct;
		using DataSettingsStruct = Primitives::TextureClass::DataSettingsStruct;
		using DimensionsEnum = Primitives::TextureClass::DimensionsEnum;
	protected:
		Vector3U Size;
		SettingsStruct Settings;
		const DataSettingsStruct DataSettings;
	public:
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, const char* filePath, SettingsStruct sets, DataSettingsStruct dataSets);
		//no taking responsibility for "data"
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, SettingsStruct sets, DataSettingsStruct dataSets);
		DLLTREATMENT TextureClass(const TextureClass& toCopy, bool copyTextureData = false);
		DLLTREATMENT TextureClass(TextureClass&& toCopy) noexcept;
		DLLTREATMENT virtual ~TextureClass() noexcept(false) override = default;
		//will just copy data in RAM and allocate empty texture with same size on GPU
		DLLTREATMENT TextureClass& operator=(const TextureClass& toCopy);
		DLLTREATMENT TextureClass& operator=(TextureClass&& toCopy);
		
		DLLTREATMENT void ChangeData(Vector3U newSize, const void* data);
		//size of texture will remain same
		DLLTREATMENT void ChangeData(const void* data) const;
		DLLTREATMENT void ChangeSubData(Vector3U offset, const void* data, Vector3U pixelsAmount) const;

		DLLTREATMENT void AllocatePixels(Vector3U newSize);
		
		DLLTREATMENT void CopySubData(const TextureClass& srcTex, Vector3U offsetInSrc, Vector3U offsetInDst, Vector3U pixelsAmount) const;

		//buffer should point to already allocated memory
		DLLTREATMENT void GetData(void* buffer) const;
		//buffer should point to already allocated memory
		DLLTREATMENT void GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount) const;

		inline Vector3U gSize() const noexcept { return Size; }

		DLLTREATMENT void sSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX);
		inline SettingsStruct::WrapTypeEnum gSettings_WrapTypeByX() const noexcept { return Settings.WrapTypeByX; }
		DLLTREATMENT void sSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY);
		inline SettingsStruct::WrapTypeEnum gSettings_WrapTypeByY() const noexcept { return Settings.WrapTypeByY; }
		DLLTREATMENT void sSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt);
		inline SettingsStruct::DownscalingFilterFuncEnum gSettings_DownscalingFilt() const noexcept { return Settings.DownscalingFilt; }
		DLLTREATMENT void sSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt);
		inline SettingsStruct::UpscalingFilterFuncEnum gSettings_UpscalingFilt() const noexcept { return Settings.UpscalingFilt; }
		DLLTREATMENT void sSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode);
		inline SettingsStruct::DepthStencilReadModeEnum gSettings_DepthStencilReadMode() const noexcept { return Settings.DepthStencilReadMode; }

		inline SettingsStruct gSettings() const noexcept { return Settings; }
		DLLTREATMENT void sSettings(SettingsStruct newSets);

		inline DataSettingsStruct::DataFormatOnGPU_Enum gDataSettings_DataFormatOnGPU() const noexcept { return DataSettings.DataFormatOnGPU; }
		inline DataSettingsStruct::DataFormatOnCPU_Enum gDataSettings_DataFormatOnCPU() const noexcept { return DataSettings.DataFormatOnCPU; }
		inline DataSettingsStruct::DataTypeOnCPU_Enum gDataSettings_DataTypeOnCPU() const noexcept { return DataSettings.DataTypeOnCPU; }

		inline DataSettingsStruct gDataSettings() const noexcept { return DataSettings; }

		using Primitives::TextureClass::Bind;
		using Primitives::TextureClass::Unbind;

		inline const Primitives::TextureClass& gPrimitiveTexture() const noexcept { return *this; }
		inline Primitives::TextureClass& gPrimitiveTexture() noexcept { return *this; }

	};
}



