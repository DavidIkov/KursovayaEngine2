#pragma once

enum class TextureStorageType {
    RGB,RGBA,Depth,DepthStencil
};

class Texture {
	unsigned int ID;
	mutable bool Deleted = false;
    unsigned int Width, Height;
    TextureStorageType StorageType;
public:
    Texture(const char* filePath);
    Texture(unsigned int Width, unsigned int Height, TextureStorageType storageTyp);
    ~Texture();
    TextureStorageType gStorageType() const;
    void gWidthAndHeight(unsigned int& width, unsigned int& height) const;
    unsigned int gID() const;
    void Delete();
    void Bind(unsigned int textureInd=0) const;
    static void Unbind();
};