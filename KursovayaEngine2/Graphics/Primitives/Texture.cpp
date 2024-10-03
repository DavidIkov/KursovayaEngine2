#include"Texture.h"
#include"stb_image/stb_image.h"
#include"Tools/GLDebug.h"
#include"Tools/ReadFromFile.h"
#include"glad/glad.h"
#include"Tools/DebuggingTools.h"
#include"Tools/DebugRuntimeAssert.h"
#include"Graphics/Globals.h"

using namespace Graphics::Primitives;
#define Assert_NotDeleted_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Deleted, "TextureClass is deleted", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#if defined KE2_Debug
#define Assert_Binded_Macro //if (DebugRuntimeAssert(DebuggingTools::ErrorTypes::Warning, BindedInstances.gTextureID() == ID, "Texture is not binded", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION)) Bind();
#else
#define Assert_Binded_Macro
#endif

static unsigned int _DataFormatOnGPU_SwitchCase(TextureDataSettingsClass::DataFormatOnGPU_Enum format) {
    switch (format) {
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::DepthComponent: return GL_DEPTH_COMPONENT24;
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::DepthStencil: return GL_DEPTH24_STENCIL8;
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::Red: return GL_RED;
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::RG: return GL_RG;
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::RGB: return GL_RGB;
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::RGBA: return GL_RGBA;
    default: return 0;
    }
}
static unsigned int _DataFormatOnCPU_SwitchCase(TextureDataSettingsClass::DataFormatOnCPU_Enum format) {
    switch (format) {
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::Red: return GL_RED;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RG: return GL_RG;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RGB: return GL_RGB;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::BGR: return GL_BGR;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RGBA: return GL_RGBA;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::BGRA: return GL_BGRA;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RedInteger: return GL_RED_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RG_Integer: return GL_RG_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RGB_Integer: return GL_RGB_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::BGR_Integer: return GL_BGR_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RGBA_Integer: return GL_RGBA_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::BGRA_Integer: return GL_BGRA_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::StencilIndex: return GL_STENCIL_INDEX;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::DepthComponent: return GL_DEPTH_COMPONENT;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::DepthStencil: return GL_DEPTH_STENCIL;
    default: return 0;
    }
}
static unsigned int _DataTypeOnCPU_SwitchCase(TextureDataSettingsClass::DataTypeOnCPU_Enum type) {
    switch (type) {
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte: return GL_UNSIGNED_BYTE;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Byte: return GL_BYTE;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedShort: return GL_UNSIGNED_SHORT;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Short: return GL_SHORT;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedInt: return GL_UNSIGNED_INT;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Int: return GL_INT;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Float: return GL_FLOAT;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedInt_24_8: return GL_UNSIGNED_INT_24_8;
    default: return 0;
    }
}
static unsigned int _DataTypeOnCPU_Sizeof_SwitchCase(TextureDataSettingsClass::DataTypeOnCPU_Enum type) {
    switch (type) {
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte: return sizeof(unsigned char);
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Byte: return sizeof(unsigned char);
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedInt: return sizeof(unsigned int);
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Int: return sizeof(int);
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Float: return sizeof(float);
    default: return 0;
    }
}
static unsigned int _WrapType_SwitchCase(TextureSettingsClass::WrapTypeEnum wrapTyp) {
    switch (wrapTyp) {
    case TextureSettingsClass::WrapTypeEnum::ClampToEdge: return GL_CLAMP_TO_EDGE;
    case TextureSettingsClass::WrapTypeEnum::ClampToBorder: return GL_CLAMP_TO_BORDER;
    case TextureSettingsClass::WrapTypeEnum::MirroredRepeat: return GL_MIRRORED_REPEAT;
    case TextureSettingsClass::WrapTypeEnum::Repeat: return GL_REPEAT;
    case TextureSettingsClass::WrapTypeEnum::MirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
    default: return 0;
    }
}
static unsigned int _DownscalingFilterFunc_SwitchCase(TextureSettingsClass::DownscalingFilterFuncEnum filt) {
    switch (filt) {
    case TextureSettingsClass::DownscalingFilterFuncEnum::Nearest: return GL_NEAREST;
    case TextureSettingsClass::DownscalingFilterFuncEnum::Linear: return GL_LINEAR;
    case TextureSettingsClass::DownscalingFilterFuncEnum::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
    case TextureSettingsClass::DownscalingFilterFuncEnum::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
    case TextureSettingsClass::DownscalingFilterFuncEnum::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
    case TextureSettingsClass::DownscalingFilterFuncEnum::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
    default: return 0;
    }
}
static unsigned int _UpscalingFilterFunc_SwitchCase(TextureSettingsClass::UpscalingFilterFuncEnum filt) {
    switch (filt) {
    case TextureSettingsClass::UpscalingFilterFuncEnum::Nearest: return GL_NEAREST;
    case TextureSettingsClass::UpscalingFilterFuncEnum::Linear: return GL_LINEAR;
    default: return 0;
    }
}
static unsigned int _DepthStencilReadMode_SwitchCase(TextureSettingsClass::DepthStencilReadModeEnum readMode) {
    switch (readMode) {
    case TextureSettingsClass::DepthStencilReadModeEnum::Depth: return GL_DEPTH_COMPONENT;
    case TextureSettingsClass::DepthStencilReadModeEnum::Stencil: return GL_STENCIL_INDEX;
    default: return 0;
    }
}



void TextureClass::_Constructor(Vector3U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets) {

    switch (Dimensions) {
    case DimensionsEnum::One: GL_TexEnum = GL_TEXTURE_1D; break;
    case DimensionsEnum::Two: GL_TexEnum = GL_TEXTURE_2D; break;
    case DimensionsEnum::Three: GL_TexEnum = GL_TEXTURE_3D; break;
    }

    glSC(glGenTextures(1, &ID));
    Bind();

    SetData(pixelsAmount, data, dataSets);
}

void TextureClass::_UpdSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTyp) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_WRAP_S, _WrapType_SwitchCase(wrapTyp)));
}
void TextureClass::_UpdSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTyp) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_WRAP_T, _WrapType_SwitchCase(wrapTyp)));
}
void TextureClass::_UpdSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum filt) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_MIN_FILTER, _DownscalingFilterFunc_SwitchCase(filt)));
}
void TextureClass::_UpdSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum filt) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro; 
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_MAG_FILTER, _UpscalingFilterFunc_SwitchCase(filt)));
}
void TextureClass::_UpdSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum readMode) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_DEPTH_STENCIL_TEXTURE_MODE, _DepthStencilReadMode_SwitchCase(readMode)));
}

void TextureClass::_UpdateSettings(const TextureSettingsClass& sets) {
    _UpdSettings_WrapTypeByX(sets.WrapTypeByX);
    _UpdSettings_WrapTypeByY(sets.WrapTypeByY);
    _UpdSettings_DownscalingFilt(sets.DownscalingFilt);
    _UpdSettings_UpscalingFilt(sets.UpscalingFilt);
    _UpdSettings_DepthStencilReadMode(sets.DepthStencilReadMode);
}

TextureClass::TextureClass(DimensionsEnum dimensions, const char* filePath, const TextureSettingsClass& sets) :Dimensions(dimensions) {

    int width, height, textureChannelsAmount;
    unsigned char* textureData = stbi_load(filePath, &width, &height, &textureChannelsAmount, 0);
    if (textureData == nullptr) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "failed to load image", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }
    TextureDataSettingsClass dataSettings;
    dataSettings.DataFormatOnGPU = TextureDataSettingsClass::DataFormatOnGPU_Enum::RGBA;
    dataSettings.DataFormatOnCPU = (textureChannelsAmount == 4) ? TextureDataSettingsClass::DataFormatOnCPU_Enum::RGBA : TextureDataSettingsClass::DataFormatOnCPU_Enum::RGB;
    dataSettings.DataTypeOnCPU = TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte;

    _Constructor(Vector3U(width, height, 0), textureData, dataSettings);
    _UpdateSettings(sets);
    
    stbi_image_free(textureData);
}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, const TextureSettingsClass& sets, const TextureDataSettingsClass& dataSets) :Dimensions(dimensions) {
    _Constructor(pixelsAmount, data, dataSets);
    _UpdateSettings(sets);
}
TextureClass::TextureClass(RespConstrFlag, const TextureClass& toCopy) {
    memcpy(this, &toCopy, sizeof(TextureClass));
    toCopy.Deleted = true;
}
TextureClass::TextureClass(const TextureClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(TextureClass));
    toCopy.Deleted = true;
}
void TextureClass::operator=(const TextureClass&& toCopy) {
    this->~TextureClass();
    memcpy(this, &toCopy, sizeof(TextureClass));
    toCopy.Deleted = true;
}
TextureClass::~TextureClass() {
    if (not Deleted) {
        glSC(glDeleteTextures(1, &ID));
        Deleted = true;
    }
}

void TextureClass::SetData(Vector3U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

    unsigned int gl_dataFormatOnGPU = _DataFormatOnGPU_SwitchCase(dataSets.DataFormatOnGPU);
    unsigned int gl_dataFormatOnCPU = _DataFormatOnCPU_SwitchCase(dataSets.DataFormatOnCPU);
    unsigned int gl_dataTypeOnCPU = _DataTypeOnCPU_SwitchCase(dataSets.DataTypeOnCPU);
    switch (Dimensions) {
    case DimensionsEnum::One: glSC(glTexImage1D(GL_TexEnum, 0, gl_dataFormatOnGPU, pixelsAmount[0], 0, gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    case DimensionsEnum::Two: glSC(glTexImage2D(GL_TexEnum, 0, gl_dataFormatOnGPU, pixelsAmount[0], pixelsAmount[1], 0, gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    case DimensionsEnum::Three: glSC(glTexImage3D(GL_TexEnum, 0, gl_dataFormatOnGPU, pixelsAmount[0], pixelsAmount[1], pixelsAmount[2], 0, gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    }
}
void TextureClass::SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data,
    TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormatOnCPU, TextureDataSettingsClass::DataTypeOnCPU_Enum dataTypeOnCPU) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

    glSC(glTexSubImage2D(GL_TEXTURE_2D, 0, pixelsOffset[0], pixelsOffset[1], pixelsAmount[0], pixelsAmount[1],
        _DataFormatOnCPU_SwitchCase(dataFormatOnCPU), _DataTypeOnCPU_SwitchCase(dataTypeOnCPU), data));
    unsigned int gl_dataFormatOnCPU = _DataFormatOnCPU_SwitchCase(dataFormatOnCPU);
    unsigned int gl_dataTypeOnCPU = _DataTypeOnCPU_SwitchCase(dataTypeOnCPU);
    switch (Dimensions) {
    case DimensionsEnum::One: glSC(glTexSubImage1D(GL_TexEnum, 0, pixelsOffset[0], pixelsAmount[0], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    case DimensionsEnum::Two: glSC(glTexSubImage2D(GL_TexEnum, 0, pixelsOffset[0], pixelsOffset[1], pixelsAmount[0], pixelsAmount[1], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    case DimensionsEnum::Three: glSC(glTexSubImage3D(GL_TexEnum, 0, pixelsOffset[0], pixelsOffset[1], pixelsOffset[2], pixelsAmount[0], pixelsAmount[1], pixelsAmount[2], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    }
}

void TextureClass::GenerateMipmaps() {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
    glSC(glGenerateMipmap(GL_TexEnum));
}

void TextureClass::GetData(TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormat, TextureDataSettingsClass::DataTypeOnCPU_Enum dataType, void* buffer) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
#if defined KE2_Debug
    if (buffer == nullptr) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"buffer shouldnt be nullptr",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
#endif
    glSC(glGetTexImage(GL_TexEnum, 0, _DataFormatOnCPU_SwitchCase(dataFormat), _DataTypeOnCPU_SwitchCase(dataType), buffer));
}

void TextureClass::sSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTypeByX) { _UpdSettings_WrapTypeByX(wrapTypeByX); }
void TextureClass::sSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTypeByY) { _UpdSettings_WrapTypeByY(wrapTypeByY); }
void TextureClass::sSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum downscalingFilt) { _UpdSettings_DownscalingFilt(downscalingFilt); }
void TextureClass::sSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum upscalingFilt) { _UpdSettings_UpscalingFilt(upscalingFilt); }
void TextureClass::sSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum depthStencilReadMode) { _UpdSettings_DepthStencilReadMode(depthStencilReadMode); }

unsigned int TextureClass::gID()  {
    Assert_NotDeleted_Macro;
    return ID;
}
void TextureClass::Delete() {
    Assert_NotDeleted_Macro;
    this->~TextureClass();
}

void TextureClass::Bind(unsigned int textureInd){
    Assert_NotDeleted_Macro;
	glSC(glActiveTexture(GL_TEXTURE0 + textureInd));
#if defined KE2_Debug
    BindedInstances.sTexture_ID(ID);
#endif
	glSC(glBindTexture(GL_TexEnum, ID));
}
void TextureClass::Unbind() {
    glSC(glBindTexture(GL_TexEnum, 0));
}

