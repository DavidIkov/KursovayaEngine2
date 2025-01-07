#include"Texture.h"
#include"stb_image/stb_image.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"
#include"Graphics/Globals.h"
#include"FrameBuffer.h"

using namespace KE2;
using namespace Graphics::Primitives;

#if defined KE2_Debug
#define Assert_Binded_Macro if(BindedInstances.gTextureID() != ID) { ErrorsSystemNamespace::SendWarning<<"Texture is not binded">>ErrorsSystemNamespace::EndOfWarning; Bind(); }
#else
#define Assert_Binded_Macro
#endif

unsigned int TextureClass::_DataFormatOnGPU_SwitchCase(DataSettingsStruct::DataFormatOnGPU_Enum format) noexcept {
    switch (format) {
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Depth: return GL_DEPTH_COMPONENT24;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::DepthStencil: return GL_DEPTH24_STENCIL8;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Stencil: return GL_STENCIL_INDEX8;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::R8UN: return GL_R8;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RG8UN: return GL_RG8;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGB8UN: return GL_RGB8;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA8UN: return GL_RGBA8;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::R8SN: return GL_R8_SNORM;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RG8SN: return GL_RG8_SNORM;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGB8SN: return GL_RGB8_SNORM;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA8SN: return GL_RGBA8_SNORM;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::R32F: return GL_R32F;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RG32F: return GL_RG32F;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGB32F: return GL_RGB32F;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA32F: return GL_RGBA32F;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::R8S: return GL_R8I;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RG8S: return GL_RG8I;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGB8S: return GL_RGB8I;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA8S: return GL_RGBA8I;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::R32S: return GL_R32I;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RG32S: return GL_RG32I;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGB32S: return GL_RGB32I;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA32S: return GL_RGBA32I;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::R8U: return GL_R8UI;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RG8U: return GL_RG8UI;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGB8U: return GL_RGB8UI;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA8U: return GL_RGBA8UI;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::R32U: return GL_R32UI;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RG32U: return GL_RG32UI;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGB32U: return GL_RGB32UI;
    case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA32U: return GL_RGBA32UI;
    default: return 0;
    }
}
unsigned int TextureClass::_DataFormatOnCPU_SwitchCase(DataSettingsStruct::DataFormatOnCPU_Enum format) noexcept {
    switch (format) {
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::R: return GL_RED;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RG: return GL_RG;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGB: return GL_RGB;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::BGR: return GL_BGR;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGBA: return GL_RGBA;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::BGRA: return GL_BGRA;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::R_NN: return GL_RED_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RG_NN: return GL_RG_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGB_NN: return GL_RGB_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::BGR_NN: return GL_BGR_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGBA_NN: return GL_RGBA_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::BGRA_NN: return GL_BGRA_INTEGER;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::Stencil: return GL_STENCIL_INDEX;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::Depth: return GL_DEPTH_COMPONENT;
    case TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::DepthStencil: return GL_DEPTH_STENCIL;
    default: return 0;
    }
}
unsigned int TextureClass::_DataTypeOnCPU_SwitchCase(DataSettingsStruct::DataTypeOnCPU_Enum type) noexcept {
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
unsigned int TextureClass::_DataTypeOnCPU_Sizeof_SwitchCase(DataSettingsStruct::DataTypeOnCPU_Enum type) noexcept {
    switch (type) {
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedByte: return sizeof(unsigned char);
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::Byte: return sizeof(unsigned char);
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedInt: return sizeof(unsigned int);
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::Int: return sizeof(int);
    case TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::Float: return sizeof(float);
    default: return 0;
    }
}
unsigned int TextureClass::_WrapType_SwitchCase(SettingsStruct::WrapTypeEnum wrapTyp) noexcept {
    switch (wrapTyp) {
    case TextureClass::SettingsStruct::WrapTypeEnum::ClampToEdge: return GL_CLAMP_TO_EDGE;
    case TextureClass::SettingsStruct::WrapTypeEnum::MirroredRepeat: return GL_MIRRORED_REPEAT;
    case TextureClass::SettingsStruct::WrapTypeEnum::Repeat: return GL_REPEAT;
    case TextureClass::SettingsStruct::WrapTypeEnum::MirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
    default: return 0;
    }
}
unsigned int TextureClass::_DownscalingFilterFunc_SwitchCase(SettingsStruct::DownscalingFilterFuncEnum filt) noexcept {
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
unsigned int TextureClass::_UpscalingFilterFunc_SwitchCase(SettingsStruct::UpscalingFilterFuncEnum filt) noexcept {
    switch (filt) {
    case TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Nearest: return GL_NEAREST;
    case TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Linear: return GL_LINEAR;
    default: return 0;
    }
}
unsigned int TextureClass::_DepthStencilReadMode_SwitchCase(SettingsStruct::DepthStencilReadModeEnum readMode) noexcept {
    switch (readMode) {
    case TextureClass::SettingsStruct::DepthStencilReadModeEnum::Depth: return GL_DEPTH_COMPONENT;
    case TextureClass::SettingsStruct::DepthStencilReadModeEnum::Stencil: return GL_STENCIL_INDEX;
    case TextureClass::SettingsStruct::DepthStencilReadModeEnum::None: return GL_DEPTH_COMPONENT;
    default: return 0;
    }
}
unsigned int TextureClass::_SwizzleMask_SwitchCase(SettingsStruct::SwizzleMaskEnum swizzleMask) noexcept {
    switch (swizzleMask) {
    case SettingsStruct::SwizzleMaskEnum::Red: return GL_RED;
    case SettingsStruct::SwizzleMaskEnum::Blue: return GL_BLUE;
    case SettingsStruct::SwizzleMaskEnum::Green: return GL_GREEN;
    case SettingsStruct::SwizzleMaskEnum::Alpha: return GL_ALPHA;
    case SettingsStruct::SwizzleMaskEnum::Zero: return GL_ZERO;
    case SettingsStruct::SwizzleMaskEnum::One: return GL_ONE;
    }
}

unsigned int TextureClass::_GL_TextureEnum_SwitchCase(TextureClass::DimensionsEnum dim) noexcept {
    switch (dim) {
    case TextureClass::DimensionsEnum::One: return GL_TEXTURE_1D; break;
    case TextureClass::DimensionsEnum::Two: return GL_TEXTURE_2D; break;
    case TextureClass::DimensionsEnum::Three: return GL_TEXTURE_3D; break;
    }
    return 0;
}


TextureClass::TextureClass(DimensionsEnum dimensions, const char* filePath, Vector3U* writeSizePtr, ArrayView<void>* writeArrayView, unsigned int mipmapLevels, const SettingsStruct& sets, const DataSettingsStruct& dataSets) 
    :Dimensions(dimensions), GL_TexEnum(_GL_TextureEnum_SwitchCase(dimensions)) {

    int width, height, textureChannelsAmount;
    unsigned char* textureData = stbi_load(filePath, &width, &height, &textureChannelsAmount, 0);
    if (textureData == nullptr)
        ErrorsSystemNamespace::SendError << "stb_image error" >> ErrorsEnumWrapperStruct(ErrorsEnum::STB_IMAGE_Failed);

    if (writeSizePtr != nullptr) *writeSizePtr = Vector3U(width, height, 0);
    if (writeArrayView != nullptr) *writeArrayView = ArrayView<void>(textureData, width * height * sizeof(unsigned char));

	glSC(glGenTextures(1, &ID));
    Bind();

    _AllocatePixels(Vector3U(width, height, 0u), mipmapLevels, dataSets.DataFormatOnGPU);
    SetSubData(Vector3U(0u), Vector3U(width, height, 0u), textureData, dataSets.DataFormatOnCPU, dataSets.DataTypeOnCPU);

    glSC(glGenerateMipmap(GL_TexEnum));

    SetSettings(sets);
    
    if (writeArrayView == nullptr) stbi_image_free(textureData);
}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, unsigned int mipmapLevels, const SettingsStruct& sets, const DataSettingsStruct& dataSets) 
    :Dimensions(dimensions), GL_TexEnum(_GL_TextureEnum_SwitchCase(dimensions)) {

    if (pixelsAmount == Vector3U(0u)) return;

	glSC(glGenTextures(1, &ID));
	Bind();

	_AllocatePixels(pixelsAmount, mipmapLevels, dataSets.DataFormatOnGPU);
	SetSubData(Vector3U(0u), pixelsAmount, data, dataSets.DataFormatOnCPU, dataSets.DataTypeOnCPU);

	glSC(glGenerateMipmap(GL_TexEnum));

	SetSettings(sets);

}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, unsigned int mipmapLevels, DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, const SettingsStruct& sets) :
    Dimensions(dimensions), GL_TexEnum(_GL_TextureEnum_SwitchCase(dimensions)) {

    if (pixelsAmount == Vector3U(0u)) return;

	glSC(glGenTextures(1, &ID));
    Bind();

	_AllocatePixels(pixelsAmount, mipmapLevels, dataFormatOnGPU);

    glSC(glGenerateMipmap(GL_TexEnum));

    SetSettings(sets);
}
TextureClass::TextureClass(TextureClass&& toCopy) noexcept :
    ID(toCopy.ID), Dimensions(toCopy.Dimensions), GL_TexEnum(toCopy.GL_TexEnum) {
    toCopy.ID = 0u;
}
TextureClass& TextureClass::operator=(TextureClass&& toCopy){
    this->~TextureClass();
    new(this) TextureClass(std::move(toCopy));
    return *this;
}
TextureClass::~TextureClass() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteTextures(1, &ID));
        ID = 0u;
    }
}

void TextureClass::_AllocatePixels(Vector3U pixelsAmount, unsigned int mipmapLevels, DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU) {
    Assert_Binded_Macro;
    unsigned int gl_dataFormatOnGPU = _DataFormatOnGPU_SwitchCase(dataFormatOnGPU);
    switch (Dimensions) {
    case DimensionsEnum::One: glSC(glTexStorage1D(GL_TexEnum, mipmapLevels + 1, gl_dataFormatOnGPU, pixelsAmount[0])); return;
    case DimensionsEnum::Two: glSC(glTexStorage2D(GL_TexEnum, mipmapLevels + 1, gl_dataFormatOnGPU, pixelsAmount[0], pixelsAmount[1])); return;
    case DimensionsEnum::Three: glSC(glTexStorage3D(GL_TexEnum, mipmapLevels + 1, gl_dataFormatOnGPU, pixelsAmount[0], pixelsAmount[1], pixelsAmount[2])); return;
    }
}

void TextureClass::CopySubData(const TextureClass& srcTex, Vector3U offsetInSource, Vector3U offsetInDestination, Vector3U pixelsAmount) {

    glSC(glCopyImageSubData(srcTex.ID, srcTex.GL_TexEnum, 0, offsetInSource[0], offsetInSource[1], offsetInSource[2],
        ID, GL_TexEnum, 0, offsetInDestination[0], offsetInDestination[1], offsetInDestination[2], pixelsAmount[0], pixelsAmount[1], pixelsAmount[2]));
    //TODO make it work with 3.3
    //opengl 3.3 variant
    /*
    FrameBufferClass fb;
    fb.AttachTexture(textureID, dataFormatOnGPU);
    fb.Finish();
    CopyFromFrameBuffer(fb.gID(), offsetInSource, offsetInDestination, pixelsAmount);
    // glCopyTexSubImage2D(GL_TEXTURE_2D, 0, offsetInDestination[0], offsetInDestination[1], offsetInSource[0], offsetInSource[1], pixelsAmount[0], pixelsAmount[1]);
    */
}

void TextureClass::SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data,
    DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType) {
    Assert_Binded_Macro;

    unsigned int gl_dataFormatOnCPU = _DataFormatOnCPU_SwitchCase(dataFormat);
    unsigned int gl_dataTypeOnCPU = _DataTypeOnCPU_SwitchCase(dataType);
    switch (Dimensions) {
    case DimensionsEnum::One: glSC(glTexSubImage1D(GL_TexEnum, 0, pixelsOffset[0], pixelsAmount[0], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    case DimensionsEnum::Two: glSC(glTexSubImage2D(GL_TexEnum, 0, pixelsOffset[0], pixelsOffset[1], pixelsAmount[0], pixelsAmount[1], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    case DimensionsEnum::Three: glSC(glTexSubImage3D(GL_TexEnum, 0, pixelsOffset[0], pixelsOffset[1], pixelsOffset[2], pixelsAmount[0], pixelsAmount[1], pixelsAmount[2], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    }
}


void TextureClass::GetData(void* buffer, DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType) const {
    Assert_Binded_Macro;

    glSC(glGetTexImage(GL_TexEnum, 0, _DataFormatOnCPU_SwitchCase(dataFormat), _DataTypeOnCPU_SwitchCase(dataType), buffer));
}
void TextureClass::GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount, DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, 
    DataSettingsStruct::DataTypeOnCPU_Enum dataType) const {
    
    //TODO make this work with 3.3, now in order for this to work i have to use opengl 4.5
    glSC(glGetTextureSubImage(ID, 0, offset[0], offset[1], offset[2], pixelsAmount[0], pixelsAmount[1], pixelsAmount[2],
        _DataFormatOnCPU_SwitchCase(dataFormat), _DataTypeOnCPU_SwitchCase(dataType),
        _DataTypeOnCPU_Sizeof_SwitchCase(dataType) * pixelsAmount[0] * pixelsAmount[1] * pixelsAmount[2], buffer));
}

void TextureClass::SetSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapType) {
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_WRAP_S, _WrapType_SwitchCase(wrapType)));
}
void TextureClass::SetSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapType) {
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_WRAP_T, _WrapType_SwitchCase(wrapType)));
}
void TextureClass::SetSettings_WrapTypeByZ(SettingsStruct::WrapTypeEnum wrapType) {
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_WRAP_R, _WrapType_SwitchCase(wrapType)));
}
void TextureClass::SetSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt) {
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_MIN_FILTER, _DownscalingFilterFunc_SwitchCase(downscalingFilt)));
}
void TextureClass::SetSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt) {
    Assert_Binded_Macro; 
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_MAG_FILTER, _UpscalingFilterFunc_SwitchCase(upscalingFilt)));
}
void TextureClass::SetSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode) {
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_DEPTH_STENCIL_TEXTURE_MODE, _DepthStencilReadMode_SwitchCase(depthStencilReadMode)));
}
void TextureClass::SetSettings_SwizzleMaskByR(SettingsStruct::SwizzleMaskEnum swizzleMask) {
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_SWIZZLE_R, _SwizzleMask_SwitchCase(swizzleMask)));
}
void TextureClass::SetSettings_SwizzleMaskByG(SettingsStruct::SwizzleMaskEnum swizzleMask) {
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_SWIZZLE_G, _SwizzleMask_SwitchCase(swizzleMask)));
}
void TextureClass::SetSettings_SwizzleMaskByB(SettingsStruct::SwizzleMaskEnum swizzleMask) {
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_SWIZZLE_B, _SwizzleMask_SwitchCase(swizzleMask)));
}
void TextureClass::SetSettings_SwizzleMaskByA(SettingsStruct::SwizzleMaskEnum swizzleMask) {
    Assert_Binded_Macro;
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_SWIZZLE_A, _SwizzleMask_SwitchCase(swizzleMask)));
}

void TextureClass::Bind(unsigned int textureInd) const {
	glSC(glActiveTexture(GL_TEXTURE0 + textureInd));
#if defined KE2_Debug
    BindedInstances.sTextureID(ID);
#endif
	glSC(glBindTexture(GL_TexEnum, ID));
}
void TextureClass::Bind() const {
#if defined KE2_Debug
    BindedInstances.sTextureID(ID);
#endif
	glSC(glBindTexture(GL_TexEnum, ID));
}

void TextureClass::Unbind() const {
    glSC(glBindTexture(GL_TexEnum, 0));
#if defined KE2_Debug
    BindedInstances.sTextureID(0);
#endif
}

