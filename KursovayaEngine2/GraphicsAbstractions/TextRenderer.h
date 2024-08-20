#pragma once
#include"DLL.h"
#include"GraphicsPrimitives/Texture.h"
#include"Maths/Vector.h"
#include"GraphicsPrimitives/ShaderProgram.h"
#include"GraphicsPrimitives/RenderingPreset.h"
#include"GraphicsPrimitives/VertexArray.h"
#include"GraphicsPrimitives/VertexBuffer.h"
#include<algorithm>
#include<vector>
#include<string>
class TextRenderer {
public:
    struct Character {
        Texture Tex;
        Vector<2>   Size;       // Size of glyph
        Vector<2>   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
        Character(Texture&& tex, Vector<2>&& size, Vector<2>&& bearing, unsigned int advance);
    };
private:
    std::vector<Character> BasicCharacters;
    std::vector<Character> RussianCharacters;
    ShaderProgram TEXT_SP;
    RenderingPreset TextPreset;
    VertexArray TEXT_VA;
    VertexBuffer TEXT_VB;
public:
    enum class ClampingFuncs {
        None, ClampIfBigger, AlwaysScaleTillClamp,
    };
    
    DLLTREATMENT TextRenderer(const char* fontDir, const char* vShaderDir, const char* fShaderDir);

    DLLTREATMENT void DrawText(
        std::wstring text,
        unsigned int screenWidth, unsigned int screenHeight,
        float textScale,
        float posX, float posY,
        float localOffsetX, float localOffsetY,
        float xSize, ClampingFuncs xClampFunc,
        float ySize, bool ignoreBottomPartOfSymbol, ClampingFuncs yClampFunc
    );
};