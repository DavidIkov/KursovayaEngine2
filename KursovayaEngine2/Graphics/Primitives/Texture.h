#pragma once
#include"DLL.h"
#include"Maths/Vector.h"
#include"Tools/ErrorsSystem.h"
#include"Tools/UsefullMacros/ComparisonByBytes.h"
#include"Tools/ArrayView.h"

namespace KE2::Graphics::Primitives {
	class TextureClass {
	public:
		struct SettingsStruct {
			enum class WrapTypeEnum :unsigned char {
				ClampToEdge, MirroredRepeat, Repeat, MirrorClampToEdge
			};
			enum class DownscalingFilterFuncEnum :unsigned char {
				Nearest, Linear, NearestMipmapNearest, NearestMipmapLinear, LinearMipmapLinear, LinearMipmapNearest
			};
			enum class UpscalingFilterFuncEnum :unsigned char {
				Nearest, Linear
			};
			enum class DepthStencilReadModeEnum :unsigned char {
				Depth, Stencil, 
				/*you cant technically select mode as None but in here its used to just dont mention depth mode texture when they have nothing to do with depth*/
				None
			};
			enum class SwizzleMaskEnum :char {
				Red, Green, Blue, Alpha, Zero, One
			};

			Vector<3, WrapTypeEnum> WrapType = Vector<3,WrapTypeEnum>(WrapTypeEnum::Repeat);
			DownscalingFilterFuncEnum DownscalingFilt = DownscalingFilterFuncEnum::LinearMipmapLinear;
			UpscalingFilterFuncEnum UpscalingFilt = UpscalingFilterFuncEnum::Linear;
			DepthStencilReadModeEnum DepthStencilReadMode = DepthStencilReadModeEnum::Depth;
			Vector<4, SwizzleMaskEnum> SwizzleMask = Vector<4, SwizzleMaskEnum>(SwizzleMaskEnum::Red, SwizzleMaskEnum::Green, SwizzleMaskEnum::Blue, SwizzleMaskEnum::Alpha);

			ComparisonByBytesMacro(SettingsStruct,==);
			ComparisonByBytesMacro(SettingsStruct,!=);

		};
		struct DataSettingsStruct {
			//those are not all possible formats but i dont really see good use case for others, for full list https://www.khronos.org/opengl/wiki/GLAPI/glTexImage2D
			//N-normalized, means that when data will be accesed it will be normalized(aka divided by maximum value)
			//S-signed, U-unsigned, and number means amount of bits, if no N is in name then value is not normalized
			//note that opengl dont have to actually store in format that you provided becouse of hardware limitations, it will choose closest one,
			//but most likely you will never notice this
			enum class DataFormatOnGPU_Enum :unsigned char {
				Depth, DepthStencil, Stencil,
				R8UN, RG8UN, RGB8UN, RGBA8UN,
				R8SN, RG8SN, RGB8SN, RGBA8SN,
				R32F, RG32F, RGB32F, RGBA32F,
				R8S, RG8S, RGB8S, RGBA8S,
				R32S, RG32S, RGB32S, RGBA32S,
				R8U, RG8U, RGB8U, RGBA8U,
				R32U, RG32U, RGB32U, RGBA32U,
			};
			//NN means not normalized, so you can pass a whole integer
			enum class DataFormatOnCPU_Enum :unsigned char {
				R, RG, RGB, BGR, RGBA, BGRA, R_NN, RG_NN, RGB_NN, BGR_NN, RGBA_NN, BGRA_NN, Stencil,
				Depth, DepthStencil
			};
			enum class DataTypeOnCPU_Enum :unsigned char {
				UnsignedByte, Byte, UnsignedShort, Short, UnsignedInt, Int, Float, UnsignedInt_24_8
			};

			DataFormatOnGPU_Enum DataFormatOnGPU = DataFormatOnGPU_Enum::RGB8UN;
			DataFormatOnCPU_Enum DataFormatOnCPU = DataFormatOnCPU_Enum::RGB;
			DataTypeOnCPU_Enum DataTypeOnCPU = DataTypeOnCPU_Enum::UnsignedByte;

			ComparisonByBytesMacro(DataSettingsStruct,==);
			ComparisonByBytesMacro(DataSettingsStruct,!=);
		};

		enum class DimensionsEnum :unsigned char {
			One, Two, Three
		};
	protected:

		static unsigned int _DataFormatOnGPU_SwitchCase(DataSettingsStruct::DataFormatOnGPU_Enum format) noexcept;
		static unsigned int _DataFormatOnCPU_SwitchCase(DataSettingsStruct::DataFormatOnCPU_Enum format) noexcept;
		static unsigned int _DataTypeOnCPU_SwitchCase(DataSettingsStruct::DataTypeOnCPU_Enum type) noexcept;
		static unsigned int _DataTypeOnCPU_Sizeof_SwitchCase(DataSettingsStruct::DataTypeOnCPU_Enum type) noexcept;
		static unsigned int _WrapType_SwitchCase(SettingsStruct::WrapTypeEnum wrapTyp) noexcept;
		static unsigned int _DownscalingFilterFunc_SwitchCase(SettingsStruct::DownscalingFilterFuncEnum filt) noexcept;
		static unsigned int _UpscalingFilterFunc_SwitchCase(SettingsStruct::UpscalingFilterFuncEnum filt) noexcept;
		static unsigned int _DepthStencilReadMode_SwitchCase(SettingsStruct::DepthStencilReadModeEnum readMode) noexcept;
		static unsigned int _SwizzleMask_SwitchCase(SettingsStruct::SwizzleMaskEnum swizzleMask) noexcept;

		static unsigned int _GL_TextureEnum_SwitchCase(TextureClass::DimensionsEnum dim) noexcept;

		unsigned int ID = 0u;

		const DimensionsEnum Dimensions;
		const unsigned int GL_TexEnum;

		void _AllocatePixels(Vector3U pixelsAmount, unsigned int mipmapLevels, DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU);
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				STB_IMAGE_Failed,
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

		DLLTREATMENT TextureClass(DimensionsEnum dimensions, const char* filePath, Vector3U* writeSizePtr, ArrayView<void>* writeArrayView, unsigned int mipmapLevels, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, unsigned int mipmapLevels, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, unsigned int mipmapLevels, DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, const SettingsStruct& sets);
		DLLTREATMENT TextureClass(TextureClass&& toCopy) noexcept;
		DLLTREATMENT virtual TextureClass& operator=(TextureClass&& toCopy);
		DLLTREATMENT virtual ~TextureClass() noexcept(false);

		DLLTREATMENT virtual void SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data,
			DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType);

		//if you dont use some axes in pixelsAmounts then dont leave them 0, use 1
		//make sure that your texture have enough pixels for copying
		DLLTREATMENT void CopySubData(const TextureClass& srcTex, Vector3U offsetInSource, Vector3U offsetInDestination, Vector3U pixelsAmount);

		//this function is slow since it will get data from gpu to cpu
		//buffer should not be nullptr, it should point to already allocated memory
		DLLTREATMENT virtual void GetData(void* buffer, DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType) const;
		//this function is slow since it will get data from gpu to cpu
		//buffer should not be nullptr, it should point to already allocated memory
		DLLTREATMENT virtual void GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount, 
			DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType) const;

		DLLTREATMENT virtual void SetSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapType);
		DLLTREATMENT virtual void SetSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapType);
		DLLTREATMENT virtual void SetSettings_WrapTypeByZ(SettingsStruct::WrapTypeEnum wrapType);
		DLLTREATMENT virtual void SetSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt);
		DLLTREATMENT virtual void SetSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt);
		DLLTREATMENT virtual void SetSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode);
		DLLTREATMENT virtual void SetSettings_SwizzleMaskByR(SettingsStruct::SwizzleMaskEnum swizzleMask);
		DLLTREATMENT virtual void SetSettings_SwizzleMaskByG(SettingsStruct::SwizzleMaskEnum swizzleMask);
		DLLTREATMENT virtual void SetSettings_SwizzleMaskByB(SettingsStruct::SwizzleMaskEnum swizzleMask);
		DLLTREATMENT virtual void SetSettings_SwizzleMaskByA(SettingsStruct::SwizzleMaskEnum swizzleMask);

		inline virtual void SetSettings(SettingsStruct sets) {
			SetSettings_WrapTypeByX(sets.WrapType[0]); SetSettings_WrapTypeByY(sets.WrapType[1]); SetSettings_WrapTypeByZ(sets.WrapType[2]);
			SetSettings_DownscalingFilt(sets.DownscalingFilt); SetSettings_UpscalingFilt(sets.UpscalingFilt);
			SetSettings_DepthStencilReadMode(sets.DepthStencilReadMode);
			SetSettings_SwizzleMaskByR(sets.SwizzleMask[0]); SetSettings_SwizzleMaskByG(sets.SwizzleMask[1]);
			SetSettings_SwizzleMaskByB(sets.SwizzleMask[2]); SetSettings_SwizzleMaskByA(sets.SwizzleMask[3]);
		}

		typedef unsigned int TextureID_Type;
		inline TextureID_Type gID() const noexcept { return ID; }
		inline operator TextureID_Type() const noexcept { return ID; }

		DLLTREATMENT void Bind(unsigned int bindingInd) const;
		//will not set active texture, so it will replace texture in last binded slot(by opengl rules)
		DLLTREATMENT void Bind() const;
		DLLTREATMENT void Unbind() const;

		
	};
}
