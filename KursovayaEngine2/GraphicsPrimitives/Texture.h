#pragma once
#include"DLL.h"
#include"Maths/Vector2.h"

enum class TextureStorageType {
    RGB,RGBA,Depth,DepthStencil,SingleChannel
};
enum class TextureWrapType {
    ClampToEdge, ClampToBorder,MirroredRepeat,Repeat,MirrorClampToEdge
};
enum class TextureDownscalingFilterFunc {
    Nearest,Linear,NearestMipmapNearest,NearestMipmapLinear,LinearMipmapLinear,LinearMipmapNearest
};
enum class TextureUpscalingFilterFunc {
    Nearest, Linear
};
enum class TextureDepthStencilReadMode {
    Depth,Stencil
};

class Texture {
    
	unsigned int ID;

    /*"deleted" can also mean that this instance of class is 
    not responsible for this element of opengl, such if another
    instance took control over it*/
	mutable bool Deleted = false;

    unsigned int Width, Height;
    TextureStorageType StorageType;

    void UpdParameters_WrapTypeByX();
    void UpdParameters_WrapTypeByY();
    void UpdParameters_DownscalingFilt();
    void UpdParameters_UpscalingFilt();
    void UpdParameters_DepthStencilReadMode();

    void UpdateAllParameters();
public:
    struct TexParameters {
        TextureWrapType WrapTypeByX;
        TextureWrapType WrapTypeByY;
        TextureDownscalingFilterFunc DownscalingFilt;
        TextureUpscalingFilterFunc UpscalingFilt;
        TextureDepthStencilReadMode DepthStencilReadMode;
    };
private:
    TexParameters Parameters;
public:
    DLLTREATMENT Texture(const char* filePath, TexParameters&& parameters);
    DLLTREATMENT Texture(unsigned int Width, unsigned int Height, TextureStorageType storageTyp, TexParameters&& parameters);
    DLLTREATMENT Texture(unsigned int Width, unsigned int Height, unsigned int channelsAmount, const unsigned char* data, TexParameters&& parameters);
    DLLTREATMENT Texture(Texture&& tempTex);
    DLLTREATMENT ~Texture();

    DLLTREATMENT void GenerateMipmaps();

    DLLTREATMENT TextureStorageType gStorageType() const;

    DLLTREATMENT void sParameters_WrapTypeByX(TextureWrapType wrapTypeByX);
    DLLTREATMENT void sParameters_WrapTypeByY(TextureWrapType wrapTypeByY);
    DLLTREATMENT void sParameters_DownscalingFilt(TextureDownscalingFilterFunc downscalingFilt);
    DLLTREATMENT void sParameters_UpscalingFilt(TextureUpscalingFilterFunc upscalingFilt);
    DLLTREATMENT void sParameters_DepthStencilReadMode(TextureDepthStencilReadMode depthStencilReadMode);

    DLLTREATMENT unsigned int gWidth() const;
    DLLTREATMENT unsigned int gHeight() const;
    DLLTREATMENT Vector2 gWidthAndHeight() const;

    DLLTREATMENT unsigned int gID() const;
    DLLTREATMENT void Delete();
    DLLTREATMENT void Bind(unsigned int textureInd=0) const;
    DLLTREATMENT static void Unbind();
};