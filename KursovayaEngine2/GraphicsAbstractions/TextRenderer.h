#pragma once
#include"DLL.h"
#include"GraphicsPrimitives/Texture.h"
#include"Maths/Vector.h"
#include"GraphicsPrimitives/ShaderProgram.h"
#include"GraphicsPrimitives/RenderingPreset.h"
#include"GraphicsPrimitives/VertexArray.h"
#include"GraphicsPrimitives/VertexBuffer.h"
#include"Tools/DynArr.h"
#include<algorithm>
#include<string>

class TextRenderer {
    static bool First; //false
    struct CharacterClass {
        TextureClass Tex;
        unsigned int UnicodeInd;
        Vector<2> Size;       // Size of glyph
        Vector<2> Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance; // Offset to advance to next glyph
        CharacterClass(TextureClass&& tex, unsigned int unicodeInd, Vector<2>&& size, Vector<2>&& bearing, unsigned int advance);
        //CharacterClass& operator=(const CharacterClass& copy);
        CharacterClass(const CharacterClass* toCopy);
    };
    struct FontClass {
        mutable bool Deleted = false;
        std::string FontName;
        void* FreeTypeFace;//FT_Face
        DynArr<CharacterClass> Characters;
        FontClass();
        FontClass(const FontClass* toCopy);
        ~FontClass();
    };
    
    void* FreeTypeLib;//FT_Library

    DynArr<FontClass> Fonts;

    ShaderProgram TEXT_SP;
    RenderingPreset TextPreset;
    VertexArray TEXT_VA;
    VertexBuffer TEXT_VB;
public:
    
    DLLTREATMENT TextRenderer(const wchar_t* vShaderDir, const wchar_t* fShaderDir);
    DLLTREATMENT ~TextRenderer();

    DLLTREATMENT std::string LoadFont(const char* fontDir, unsigned int charsSize);
    DLLTREATMENT void LoadCharacters(const std::string& fontName, const wchar_t* chars);

    enum class ClampingFuncs {
        None, ClampIfBigger, AlwaysScaleTillClamp,
    };
    DLLTREATMENT void DrawText(
        const std::string& fontName, std::wstring text,
        unsigned int screenWidth, unsigned int screenHeight,
        float textScale,
        float posX, float posY,
        float localOffsetX, float localOffsetY,
        float xSize, ClampingFuncs xClampFunc,
        float ySize, bool ignoreBottomPartOfSymbol, ClampingFuncs yClampFunc
    );
};