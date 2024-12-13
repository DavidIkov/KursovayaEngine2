#pragma once
#include"DLL.h"
#include"Graphics/Primitives/Texture.h"
#include"Tools/ClassFunctionsAccessController.h"

namespace KE2::Graphics::Abstractions {

	//make sure that if you dont use some axes in size specification of texture then 
	//you dont leave them as 0, use 1, otherwise it will mean that texture is empty
	class TextureClass : protected Primitives::TextureClass {
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
            enum ErrorsEnum {
#ifdef KE2_Debug
				TryingToUseDifferentDataSettings,
#endif
            };
            ErrorsEnum Error;
            inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
        }; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

		using SettingsStruct = Primitives::TextureClass::SettingsStruct;
		using DataSettingsStruct = Primitives::TextureClass::DataSettingsStruct;
		using DimensionsEnum = Primitives::TextureClass::DimensionsEnum;
	protected:
		Vector3U Size;
		SettingsStruct Settings;
		const DataSettingsStruct DataSettings;
	public:

		inline Primitives::TextureClass& gPrimitiveTextureClass() noexcept { return *this; }
		inline const Primitives::TextureClass& gPrimitiveTextureClass() const noexcept { return *this; }

		DLLTREATMENT TextureClass(DimensionsEnum dimensions, const char* filePath, SettingsStruct sets, DataSettingsStruct dataSets);
		//no taking responsibility for "data"
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, SettingsStruct sets, DataSettingsStruct dataSets);
		DLLTREATMENT TextureClass(const TextureClass& toCopy, bool copyTextureData = false);
		DLLTREATMENT TextureClass(TextureClass&& toCopy) noexcept;
		DLLTREATMENT virtual ~TextureClass() noexcept(false) override = default;
		//will just copy data in RAM and allocate empty texture with same size on GPU
		DLLTREATMENT virtual TextureClass& operator=(const TextureClass& toCopy);
	private:
		inline virtual Primitives::TextureClass& operator=(Primitives::TextureClass&& toCopy) override final { return operator=(dynamic_cast<TextureClass&&>(toCopy)); }
	public:
		DLLTREATMENT virtual TextureClass& operator=(TextureClass&& toCopy);

	private:
		inline virtual void SetData(Vector3U pixelsAmount, const void* data, const DataSettingsStruct& dataSets) override final { 
#ifdef KE2_Debug
			if (dataSets != DataSettings)
				ErrorsSystemNamespace::SendError << "Trying to use different data settings" >> ErrorsEnumWrapperStruct(ErrorsEnum::TryingToUseDifferentDataSettings);
#endif
			SetData(pixelsAmount, data);
		}
	public:
		DLLTREATMENT virtual void SetData(Vector3U newSize, const void* data);
		DLLTREATMENT virtual void SetData(const void* data);

	private:
		inline virtual void SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data,
			DataSettingsStruct::DataFormatOnCPU_Enum dataFormatOnCPU, DataSettingsStruct::DataTypeOnCPU_Enum dataTypeOnCPU) override final {
#ifdef KE2_Debug
			if (DataSettings.DataFormatOnCPU != dataFormatOnCPU || DataSettings.DataTypeOnCPU != dataTypeOnCPU)
				ErrorsSystemNamespace::SendError << "Trying to use different data settings" >> ErrorsEnumWrapperStruct(ErrorsEnum::TryingToUseDifferentDataSettings);
#endif
			SetSubData(pixelsOffset, pixelsAmount, data);
		}
	public:
		//size of texture will remain same
		DLLTREATMENT virtual void SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data);


		DLLTREATMENT virtual void AllocatePixels(Vector3U newSize);

	private:
		inline virtual void CopySubData(const Primitives::TextureClass& srcTex, Vector3U offsetInSource, Vector3U offsetInDestination, Vector3U pixelsAmount) override final {
			CopySubData(dynamic_cast<const TextureClass&>(srcTex), offsetInSource, offsetInDestination, pixelsAmount);
		}
	public:
		DLLTREATMENT virtual void CopySubData(const TextureClass& srcTex, Vector3U offsetInSrc, Vector3U offsetInDst, Vector3U pixelsAmount);

	private:
		inline virtual void GetData(void* buffer, DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType) const override final {
#ifdef KE2_Debug
			if (DataSettings.DataFormatOnCPU != dataFormat || DataSettings.DataTypeOnCPU != dataType)
				ErrorsSystemNamespace::SendError << "Trying to use different data settings" >> ErrorsEnumWrapperStruct(ErrorsEnum::TryingToUseDifferentDataSettings);
#endif
			GetData(buffer);
		}
	public:
		//buffer should point to already allocated memory
		DLLTREATMENT void GetData(void* buffer) const;

	private:
		inline virtual void GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount,
			DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType) const override final {
#ifdef KE2_Debug
			if (DataSettings.DataFormatOnCPU != dataFormat || DataSettings.DataTypeOnCPU != dataType)
				ErrorsSystemNamespace::SendError << "Trying to use different data settings" >> ErrorsEnumWrapperStruct(ErrorsEnum::TryingToUseDifferentDataSettings);
#endif
			GetSubData(offset, buffer, pixelsAmount);
		}
	public:
		//buffer should point to already allocated memory
		DLLTREATMENT void GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount) const;

		inline Vector3U gSize() const noexcept { return Size; }

		DLLTREATMENT virtual void sSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX) override final;
		inline SettingsStruct::WrapTypeEnum gSettings_WrapTypeByX() const noexcept { return Settings.WrapTypeByX; }
		DLLTREATMENT virtual void sSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY) override final;
		inline SettingsStruct::WrapTypeEnum gSettings_WrapTypeByY() const noexcept { return Settings.WrapTypeByY; }
		DLLTREATMENT virtual void sSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt) override final;
		inline SettingsStruct::DownscalingFilterFuncEnum gSettings_DownscalingFilt() const noexcept { return Settings.DownscalingFilt; }
		DLLTREATMENT virtual void sSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt) override final;
		inline SettingsStruct::UpscalingFilterFuncEnum gSettings_UpscalingFilt() const noexcept { return Settings.UpscalingFilt; }
		DLLTREATMENT virtual void sSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode) override final;
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



