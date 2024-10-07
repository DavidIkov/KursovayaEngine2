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
#define Assert_Binded_Macro if (DebugRuntimeAssert(DebuggingTools::ErrorTypes::Warning, BindedInstances.gTextureID() == ID, "Texture is not binded", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION)) Bind();
#else
#define Assert_Binded_Macro
#endif

static unsigned int _DataFormatOnGPU_SwitchCase(TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum format) {
    switch (format) {
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::DepthComponent: return GL_DEPTH_COMPONENT24;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::DepthStencil: return GL_DEPTH24_STENCIL8;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Red: return GL_RED;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RG: return GL_RG;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGB: return GL_RGB;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA: return GL_RGBA;
    default: return 0;
    }
}
static unsigned int _DataFormatOnCPU_SwitchCase(TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum format) {
    switch (format) {
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::Red: return GL_RED;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RG: return GL_RG;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGB: return GL_RGB;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::BGR: return GL_BGR;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGBA: return GL_RGBA;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::BGRA: return GL_BGRA;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RedInteger: return GL_RED_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RG_Integer: return GL_RG_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGB_Integer: return GL_RGB_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::BGR_Integer: return GL_BGR_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGBA_Integer: return GL_RGBA_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::BGRA_Integer: return GL_BGRA_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::StencilIndex: return GL_STENCIL_INDEX;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::DepthComponent: return GL_DEPTH_COMPONENT;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::DepthStencil: return GL_DEPTH_STENCIL;
    default: return 0;
    }
}
static unsigned int _DataTypeOnCPU_SwitchCase(TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum type) {
    switch (type) {
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedByte: return GL_UNSIGNED_BYTE;
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::Byte: return GL_BYTE;
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedShort: return GL_UNSIGNED_SHORT;
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::Short: return GL_SHORT;
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedInt: return GL_UNSIGNED_INT;
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::Int: return GL_INT;
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::Float: return GL_FLOAT;
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedInt_24_8: return GL_UNSIGNED_INT_24_8;
    default: return 0;
    }
}
static unsigned int _DataTypeOnCPU_Sizeof_SwitchCase(TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum type) {
    switch (type) {
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedByte: return sizeof(unsigned char);
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::Byte: return sizeof(unsigned char);
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedInt: return sizeof(unsigned int);
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::Int: return sizeof(int);
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::Float: return sizeof(float);
    default: return 0;
    }
}
static unsigned int _WrapType_SwitchCase(TextureClass::SettingsStruct::WrapTypeEnum wrapTyp) {
    switch (wrapTyp) {
    case TextureClass::SettingsStruct::WrapTypeEnum::ClampToEdge: return GL_CLAMP_TO_EDGE;
    case TextureClass::SettingsStruct::WrapTypeEnum::ClampToBorder: return GL_CLAMP_TO_BORDER;
    case TextureClass::SettingsStruct::WrapTypeEnum::MirroredRepeat: return GL_MIRRORED_REPEAT;
    case TextureClass::SettingsStruct::WrapTypeEnum::Repeat: return GL_REPEAT;
    case TextureClass::SettingsStruct::WrapTypeEnum::MirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
    default: return 0;
    }
}
static unsigned int _DownscalingFilterFunc_SwitchCase(TextureClass::SettingsStruct::DownscalingFilterFuncEnum filt) {
    switch (filt) {
    case TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Nearest: return GL_NEAREST;
    case TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Linear: return GL_LINEAR;
    case TextureClass::SettingsStruct::DownscalingFilterFuncEnum::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
    case TextureClass::SettingsStruct::DownscalingFilterFuncEnum::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
    case TextureClass::SettingsStruct::DownscalingFilterFuncEnum::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
    case TextureClass::SettingsStruct::DownscalingFilterFuncEnum::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
    default: return 0;
    }
}
static unsigned int _UpscalingFilterFunc_SwitchCase(TextureClass::SettingsStruct::UpscalingFilterFuncEnum filt) {
    switch (filt) {
    case TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Nearest: return GL_NEAREST;
    case TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Linear: return GL_LINEAR;
    default: return 0;
    }
}
static unsigned int _DepthStencilReadMode_SwitchCase(TextureClass::SettingsStruct::DepthStencilReadModeEnum readMode) {
    switch (readMode) {
    case TextureClass::SettingsStruct::DepthStencilReadModeEnum::Depth: return GL_DEPTH_COMPONENT;
    case TextureClass::SettingsStruct::DepthStencilReadModeEnum::Stencil: return GL_STENCIL_INDEX;
    case TextureClass::SettingsStruct::DepthStencilReadModeEnum::None: return GL_DEPTH_COMPONENT;
    default: return 0;
    }
}



void TextureClass::_Constructor(Vector3U pixelsAmount, const void* data, const TextureClass::DataSettingsStruct& dataSets) {

    switch (Dimensions) {
    case DimensionsEnum::One: GL_TexEnum = GL_TEXTURE_1D; break;
    case DimensionsEnum::Two: GL_TexEnum = GL_TEXTURE_2D; break;
    case DimensionsEnum::Three: GL_TexEnum = GL_TEXTURE_3D; break;
    }

    glSC(glGenTextures(1, &ID));
    Bind();

    SetData(pixelsAmount, data, dataSets);
}

void TextureClass::_UpdSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTyp) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_WRAP_S, _WrapType_SwitchCase(wrapTyp)));
}
void TextureClass::_UpdSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTyp) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_WRAP_T, _WrapType_SwitchCase(wrapTyp)));
}
void TextureClass::_UpdSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum filt) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_MIN_FILTER, _DownscalingFilterFunc_SwitchCase(filt)));
}
void TextureClass::_UpdSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum filt) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro; 
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_MAG_FILTER, _UpscalingFilterFunc_SwitchCase(filt)));
}
void TextureClass::_UpdSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum readMode) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_DEPTH_STENCIL_TEXTURE_MODE, _DepthStencilReadMode_SwitchCase(readMode)));
}

void TextureClass::_UpdateSettings(const SettingsStruct& sets) {
    _UpdSettings_WrapTypeByX(sets.WrapTypeByX);
    _UpdSettings_WrapTypeByY(sets.WrapTypeByY);
    _UpdSettings_DownscalingFilt(sets.DownscalingFilt);
    _UpdSettings_UpscalingFilt(sets.UpscalingFilt);
    _UpdSettings_DepthStencilReadMode(sets.DepthStencilReadMode);
}

TextureClass::TextureClass(DimensionsEnum dimensions, const char* filePath, Vector3U* writeSizePtr, void** writeDataPtr, unsigned int* writeDataSizeInBitsPtr, const SettingsStruct& sets, const DataSettingsStruct& dataSets) :Dimensions(dimensions) {

    int width, height, textureChannelsAmount;
    unsigned char* textureData = stbi_load(filePath, &width, &height, &textureChannelsAmount, 0);
    if (textureData == nullptr) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "failed to load image", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }

    if (writeSizePtr != nullptr) *writeSizePtr = Vector3U(width, height, 0);
    if (writeDataPtr != nullptr) *writeDataPtr = textureData;
    if (writeDataSizeInBitsPtr != nullptr) *writeDataSizeInBitsPtr = sizeof(unsigned char) * width * height;

    _Constructor(Vector3U(width, height, 0), textureData, dataSets);
    _UpdateSettings(sets);
    
    if (writeDataPtr == nullptr) stbi_image_free(textureData);
}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, const SettingsStruct& sets, const DataSettingsStruct& dataSets) :Dimensions(dimensions) {
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

void TextureClass::SetData(Vector3U pixelsAmount, const void* data, const DataSettingsStruct& dataSets) {
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
    DataSettingsStruct::DataFormatOnCPU_Enum dataFormatOnCPU, DataSettingsStruct::DataTypeOnCPU_Enum dataTypeOnCPU) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

    //glSC(glTexSubImage2D(GL_TEXTURE_2D, 0, pixelsOffset[0], pixelsOffset[1], pixelsAmount[0], pixelsAmount[1],
    //    _DataFormatOnCPU_SwitchCase(dataFormatOnCPU), _DataTypeOnCPU_SwitchCase(dataTypeOnCPU), data));
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

void TextureClass::GetData(DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType, void* buffer) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;
#if defined KE2_Debug
    if (buffer == nullptr) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"buffer shouldnt be nullptr",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
#endif
    glSC(glGetTexImage(GL_TexEnum, 0, _DataFormatOnCPU_SwitchCase(dataFormat), _DataTypeOnCPU_SwitchCase(dataType), buffer));
}

void TextureClass::sSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX) { _UpdSettings_WrapTypeByX(wrapTypeByX); }
void TextureClass::sSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY) { _UpdSettings_WrapTypeByY(wrapTypeByY); }
void TextureClass::sSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt) { _UpdSettings_DownscalingFilt(downscalingFilt); }
void TextureClass::sSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt) { _UpdSettings_UpscalingFilt(upscalingFilt); }
void TextureClass::sSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode) { _UpdSettings_DepthStencilReadMode(depthStencilReadMode); }

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
    Assert_NotDeleted_Macro;
    glSC(glBindTexture(GL_TexEnum, 0));
}

