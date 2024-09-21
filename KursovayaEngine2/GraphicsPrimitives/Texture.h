#pragma once
#include"DLL.h"
#include"Maths/Vector.h"

struct TextureSettingsClass {
    enum class WrapTypeEnum :unsigned short int {
        ClampToEdge, ClampToBorder, MirroredRepeat, Repeat, MirrorClampToEdge
    };
    enum class DownscalingFilterFuncEnum :unsigned short int {
        Nearest, Linear, NearestMipmapNearest, NearestMipmapLinear, LinearMipmapLinear, LinearMipmapNearest
    };
    enum class UpscalingFilterFuncEnum :unsigned short int {
        Nearest, Linear
    };
    enum class DepthStencilReadModeEnum :unsigned short int {
        Depth, Stencil
    };

    WrapTypeEnum WrapTypeByX;
    WrapTypeEnum WrapTypeByY;
    DownscalingFilterFuncEnum DownscalingFilt;
    UpscalingFilterFuncEnum UpscalingFilt;
    DepthStencilReadModeEnum DepthStencilReadMode;
};
struct TextureDataSettingsClass {
    enum class DataFormatOnGPU_Enum :unsigned short int {
        DepthComponent, DepthStencil, Red, RG, RGB, RGBA
    };
    enum class DataFormatOnCPU_Enum :unsigned short int {
        Red, RG, RGB, BGR, RGBA, BGRA, RedInteger, RG_Integer, RGB_Integer, BGR_Integer, RGBA_Integer, BGRA_Integer, StencilIndex,
        DepthComponent, DepthStencil
    };
    enum class DataTypeOnCPU_Enum :unsigned short int {
        UnsignedByte, Byte, UnsignedShort, Short, UnsignedInt, Int, Float, UnsignedInt_24_8
    };

    //number of color components in texture
    DataFormatOnGPU_Enum DataFormatOnGPU;
    DataFormatOnCPU_Enum DataFormatOnCPU;
    DataTypeOnCPU_Enum DataTypeOnCPU;
};

class Texture1DClass {

    unsigned int ID;
    mutable bool Deleted = false;


    void _Constructor(unsigned int pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets);

    void _UpdSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTyp);
    void _UpdSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTyp);
    void _UpdSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum filt);
    void _UpdSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum filt);
    void _UpdSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum readMode);

    void _UpdateSettings(const TextureSettingsClass& sets);
public:
    DLLTREATMENT Texture1DClass(const char* filePath, const TextureSettingsClass& sets);
    DLLTREATMENT Texture1DClass(unsigned int pixelsAmount, const void* data, const TextureSettingsClass& sets, const TextureDataSettingsClass& dataSets);
    DLLTREATMENT Texture1DClass(const Texture1DClass* toCopy);
    DLLTREATMENT Texture1DClass(const Texture1DClass&& toCopy);
    DLLTREATMENT void operator=(const Texture1DClass&& toCopy);
    DLLTREATMENT ~Texture1DClass();

    DLLTREATMENT void SetData(unsigned int pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets);
    DLLTREATMENT void SetSubData(unsigned int pixelsOffset, unsigned int pixelsAmount, const void* data,
        TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormatOnCPU, TextureDataSettingsClass::DataTypeOnCPU_Enum dataTypeOnCPU);

    DLLTREATMENT void GenerateMipmaps();

    DLLTREATMENT void sSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTypeByX);
    DLLTREATMENT void sSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTypeByY);
    DLLTREATMENT void sSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum downscalingFilt);
    DLLTREATMENT void sSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum upscalingFilt);
    DLLTREATMENT void sSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum depthStencilReadMode);

    DLLTREATMENT unsigned int gID() const;
    DLLTREATMENT void Delete();
    DLLTREATMENT void Bind(unsigned int textureInd = 0) const;
    DLLTREATMENT void Unbind();
};

class Texture2DClass {

    unsigned int ID;
    mutable bool Deleted = false;


private:

    void _Constructor(Vector2U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets);

    void _UpdSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTyp);
    void _UpdSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTyp);
    void _UpdSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum filt);
    void _UpdSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum filt);
    void _UpdSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum readMode);

    void _UpdateSettings(const TextureSettingsClass& sets);
public:
    DLLTREATMENT Texture2DClass(const char* filePath, const TextureSettingsClass& sets);
    DLLTREATMENT Texture2DClass(Vector2U pixelsAmount, const void* data, const TextureSettingsClass& sets, const TextureDataSettingsClass& dataSets);
    DLLTREATMENT Texture2DClass(const Texture2DClass* toCopy);
    DLLTREATMENT Texture2DClass(const Texture2DClass&& toCopy);
    DLLTREATMENT void operator=(const Texture2DClass&& toCopy);
    DLLTREATMENT ~Texture2DClass();

    DLLTREATMENT void SetData(Vector2U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets);
    DLLTREATMENT void SetSubData(Vector2U pixelsOffset, Vector2U pixelsAmount, const void* data,
        TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormatOnCPU, TextureDataSettingsClass::DataTypeOnCPU_Enum dataTypeOnCPU);

    DLLTREATMENT void GenerateMipmaps();

    DLLTREATMENT void sSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTypeByX);
    DLLTREATMENT void sSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTypeByY);
    DLLTREATMENT void sSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum downscalingFilt);
    DLLTREATMENT void sSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum upscalingFilt);
    DLLTREATMENT void sSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum depthStencilReadMode);

    DLLTREATMENT unsigned int gID() const;
    DLLTREATMENT void Delete();
    DLLTREATMENT void Bind(unsigned int textureInd = 0) const;
    DLLTREATMENT void Unbind();
};

