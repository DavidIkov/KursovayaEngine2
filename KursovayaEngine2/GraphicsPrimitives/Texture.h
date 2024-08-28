#pragma once
#include"DLL.h"
#include"Maths/Vector.h"

class TextureClass {
    
	unsigned int ID;

	mutable bool Deleted = false;
    
public:
    struct SettingsClass {
        enum class WrapTypeEnum :unsigned short int {
            ClampToEdge, ClampToBorder, MirroredRepeat, Repeat, MirrorClampToEdge
        };
        WrapTypeEnum WrapTypeByX;
        WrapTypeEnum WrapTypeByY;
        enum class DownscalingFilterFuncEnum :unsigned short int {
            Nearest, Linear, NearestMipmapNearest, NearestMipmapLinear, LinearMipmapLinear, LinearMipmapNearest
        };
        DownscalingFilterFuncEnum DownscalingFilt;
        enum class UpscalingFilterFuncEnum :unsigned short int {
            Nearest, Linear
        };
        UpscalingFilterFuncEnum UpscalingFilt;
        enum class DepthStencilReadModeEnum :unsigned short int {
            Depth, Stencil
        };
        DepthStencilReadModeEnum DepthStencilReadMode;
    };
    struct DataSettingsClass {
        //number of color components in texture
        enum class DataFormatOnGPU_Enum :unsigned short int {
            DepthComponent, DepthStencil, Red, RG, RGB, RGBA
        };
        DataFormatOnGPU_Enum DataFormatOnGPU;
        enum class DataFormatOnCPU_Enum :unsigned short int {
            Red, RG, RGB, BGR, RGBA, BGRA, RedInteger, RG_Integer, RGB_Integer, BGR_Integer, RGBA_Integer, BGRA_Integer, StencilIndex, 
            DepthComponent, DepthStencil
        };
        DataFormatOnCPU_Enum DataFormatOnCPU;
        enum class DataTypeOnCPU_Enum :unsigned short int {
            UnsignedByte, Byte, UnsignedShort, Short, UnsignedInt, Int, Float, UnsignedInt_24_8
        };
        DataTypeOnCPU_Enum DataTypeOnCPU;
    };
private:
    void _Constructor(unsigned int width, unsigned int height, const void* data, const DataSettingsClass& dataSets);

    void _UpdSettings_WrapTypeByX(SettingsClass::WrapTypeEnum wrapTyp);
    void _UpdSettings_WrapTypeByY(SettingsClass::WrapTypeEnum wrapTyp);
    void _UpdSettings_DownscalingFilt(SettingsClass::DownscalingFilterFuncEnum filt);
    void _UpdSettings_UpscalingFilt(SettingsClass::UpscalingFilterFuncEnum filt);
    void _UpdSettings_DepthStencilReadMode(SettingsClass::DepthStencilReadModeEnum readMode);

    void _UpdateSettings(const SettingsClass& sets);
public:
    DLLTREATMENT TextureClass(const char* filePath, const SettingsClass& sets);
    DLLTREATMENT TextureClass(unsigned int width, unsigned int height, const void* data, const SettingsClass& sets, const DataSettingsClass& dataSets);
    DLLTREATMENT TextureClass(const TextureClass* toCopy);
    DLLTREATMENT TextureClass(const TextureClass&& toCopy);
    DLLTREATMENT void operator=(const TextureClass&& toCopy);
    DLLTREATMENT ~TextureClass();

    DLLTREATMENT void GenerateMipmaps();
    
    DLLTREATMENT void sSettings_WrapTypeByX(SettingsClass::WrapTypeEnum wrapTypeByX);
    DLLTREATMENT void sSettings_WrapTypeByY(SettingsClass::WrapTypeEnum wrapTypeByY);
    DLLTREATMENT void sSettings_DownscalingFilt(SettingsClass::DownscalingFilterFuncEnum downscalingFilt);
    DLLTREATMENT void sSettings_UpscalingFilt(SettingsClass::UpscalingFilterFuncEnum upscalingFilt);
    DLLTREATMENT void sSettings_DepthStencilReadMode(SettingsClass::DepthStencilReadModeEnum depthStencilReadMode);

    DLLTREATMENT unsigned int gID() const;
    DLLTREATMENT void Delete();
    DLLTREATMENT void Bind(unsigned int textureInd = 0) const;
    DLLTREATMENT static void Unbind();
};