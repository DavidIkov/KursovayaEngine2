#include"TextRenderer.h"
#include"Tools/ErrorCodes.h"
#include"Tools/DebuggingTools.h"
#include<string>
#include"GraphicsPrimitives/Renderer.h"
#include"Tools/GLDebug.h"
#include"FreeType/ft2build.h"
#include FT_FREETYPE_H

#include"filesystem"


TextRenderer::Character::Character(Texture&& tex, Vector2&& size, Vector2&& bearing, unsigned int advance) 
	:Tex(std::move(tex)), Size(size), Bearing(bearing), Advance(advance) { }

TextRenderer::TextRenderer(const char* fontDir, const char* vShaderDir, const char* fShaderDir):
    TextPreset(
    false, RenderingPresetEnumArguments::FaceCulling::FaceToCull::Back, RenderingPresetEnumArguments::FaceCulling::FaceDetermination::Clockwise,
    false, true, RenderingPresetEnumArguments::DepthTest::TypeOfComparison::LessOrEqual,
    false, 0, RenderingPresetEnumArguments::StencilTest::TypeOfComparison::Equal, 1, 255, RenderingPresetEnumArguments::StencilTest::Actions::Keep,
    RenderingPresetEnumArguments::StencilTest::Actions::Keep, RenderingPresetEnumArguments::StencilTest::Actions::Keep,
    true, 0, 0, 0, 0, RenderingPresetEnumArguments::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusSrcAlpha,
    0.f, 0.f, 0.f
){

    FT_Library FT;
    if (FT_Init_FreeType(&FT)) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"FAILED TO INITIALIZE FreeType LIBRARY",KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_INITIALIZE_LIBRARY });
    }

    FT_Face FT_FACE;
    if (FT_New_Face(FT, fontDir, 0, &FT_FACE))
    {
        std::string errMsg;
        errMsg += "FreeType ERROR: FAILED TO OPEN FONT \"";
        errMsg += fontDir;
        errMsg += "\"";
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,errMsg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }

    FT_Set_Pixel_Sizes(FT_FACE, 0, 48);

    glSC(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // disable byte-alignment restriction


    BasicCharacters.reserve(128);

    for (unsigned int ci = 0; ci < 128; ci++)
    {
        // load character glyph 
        if (FT_Load_Char(FT_FACE, ci, FT_LOAD_RENDER))
        {
            DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"FreeType ERROR: FAILED TO LOAD Glyph",KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
            continue;
        }
        BasicCharacters.emplace_back(
            Texture{ FT_FACE->glyph->bitmap.width, FT_FACE->glyph->bitmap.rows,1,FT_FACE->glyph->bitmap.buffer,{
            TextureWrapType::ClampToEdge,TextureWrapType::ClampToEdge,
            TextureDownscalingFilterFunc::Linear,TextureUpscalingFilterFunc::Linear
            } },
            Vector2{ (float)FT_FACE->glyph->bitmap.width, (float)FT_FACE->glyph->bitmap.rows },
            Vector2{ (float)FT_FACE->glyph->bitmap_left, (float)FT_FACE->glyph->bitmap_top },
            (unsigned int)FT_FACE->glyph->advance.x / 64
        );
    }

    {//russian characters
        unsigned int first = L'À';
        unsigned int last = L'ÿ';

        RussianCharacters.reserve(last - first + 1);

        for (unsigned int ci = first; ci < last + 1; ci++)
        {
            // load character glyph 
            if (FT_Load_Char(FT_FACE, (wchar_t)ci, FT_LOAD_RENDER))
            {
                DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"FreeType ERROR: FAILED TO LOAD Glyph",KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
                continue;
            }
            RussianCharacters.emplace_back(
                Texture{ FT_FACE->glyph->bitmap.width, FT_FACE->glyph->bitmap.rows,1,FT_FACE->glyph->bitmap.buffer,{
                TextureWrapType::ClampToEdge,TextureWrapType::ClampToEdge,
                TextureDownscalingFilterFunc::Linear,TextureUpscalingFilterFunc::Linear
                } },
                Vector2{ (float)FT_FACE->glyph->bitmap.width, (float)FT_FACE->glyph->bitmap.rows },
                Vector2{ (float)FT_FACE->glyph->bitmap_left, (float)FT_FACE->glyph->bitmap_top },
                (unsigned int)FT_FACE->glyph->advance.x / 64
            );
        }
    }

    glSC(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));

    FT_Done_Face(FT_FACE);
    FT_Done_FreeType(FT);



    {
        Shader VS(vShaderDir, ShaderTypesEnum::Vertex);
        VS.Compile();
        TEXT_SP.AttachShader(VS);

        Shader FS(fShaderDir, ShaderTypesEnum::Fragment);
        FS.Compile();
        TEXT_SP.AttachShader(FS);

        TEXT_SP.LinkShaders();

        TEXT_SP.SetUniform1i("u_TextTexture", 0);
    }

    TEXT_VB.ReserveData(6 * 4, VertexBufferDataUsage::DynamicDraw);
    TEXT_VB.SetLayout({ 2,2 });
}
void TextRenderer::DrawText(
    std::wstring text,
    unsigned int screenWidth, unsigned int screenHeight,
    float textScale,
    float posX, float posY,
    float localOffsetX, float localOffsetY,
    float xSize, ClampingFuncs xClampFunc,
    float ySize, bool ignoreBottomPartOfSymbol, ClampingFuncs yClampFunc
) {
    TextPreset.Bind();
    TEXT_SP.Bind();
    TEXT_SP.SetUniform2f("u_PosOnScreen", 0.f, 0.f);

    TEXT_VA.Bind();

    float totalXSize = 0;
    
    float maxYUp = 0;
    float maxYDown = 0;

#define getCharData()\
if (text[i] < BasicCharacters.size()) charData = &BasicCharacters[text[i]];\
else if (text[i] >= L'À' and text[i] <= L'ÿ') charData = &RussianCharacters[text[i] - L'À'];\
else charData = &BasicCharacters[63];//the '?' symbol

    for (unsigned int i = 0; i < text.size(); i++) {
        Character* charData = nullptr;
        getCharData();
        
        totalXSize += charData->Advance;
        float charYSize = charData->Bearing.gY() + (ignoreBottomPartOfSymbol ? 0 : (charData->Size.gY() - charData->Bearing.gY()));
        
        if (not ignoreBottomPartOfSymbol and maxYDown < charData->Size.gY() - charData->Bearing.gY()) maxYDown = charData->Size.gY() - charData->Bearing.gY();
        if (maxYUp < charData->Bearing.gY()) maxYUp = charData->Bearing.gY();

    }
    float totalYSize = maxYUp + maxYDown;
    totalXSize *= textScale / (float)screenWidth;
    totalYSize *= textScale / (float)screenHeight;
    maxYDown *= textScale / (float)screenHeight;
    {
        float mult = 1;
        bool xpass = xClampFunc == ClampingFuncs::AlwaysScaleTillClamp or xClampFunc == ClampingFuncs::ClampIfBigger and totalXSize > xSize;
        bool ypass = yClampFunc == ClampingFuncs::AlwaysScaleTillClamp or yClampFunc == ClampingFuncs::ClampIfBigger and totalYSize > ySize;
        if (xClampFunc != ClampingFuncs::None and yClampFunc == ClampingFuncs::None) {
            if (xpass) mult = xSize / totalXSize;
        }
        else if (xClampFunc == ClampingFuncs::None and yClampFunc != ClampingFuncs::None) {
            if (ypass)  mult = ySize / totalYSize;
        }
        else if (xClampFunc != ClampingFuncs::None and yClampFunc != ClampingFuncs::None) {
            if (xpass and not ypass) mult = xSize / totalXSize;
            else if (not xpass and ypass) mult = ySize / totalYSize;
            else if (xpass and ypass) mult = fminf(xSize / totalXSize, ySize / totalYSize);
        }
        textScale *= mult;
        totalXSize *= mult;
        totalYSize *= mult;
        maxYDown *= mult;

    }

    localOffsetX = (localOffsetX / 2 + 0.5f) * totalXSize * 2;
    localOffsetY = (localOffsetY / 2 + 0.5f) * totalYSize * 2;

    for (unsigned int i = 0; i < text.size(); i++) {
        Character* charData = nullptr;
        getCharData();

        charData->Tex.Bind(0);
        float xpos = posX + charData->Bearing.gX() * textScale / screenWidth * 2 - localOffsetX;
        float ypos = posY + (charData->Bearing.gY() - charData->Size.gY()) * textScale / screenHeight * 2 - localOffsetY + maxYDown * 2;

        float w = charData->Size.gX() * textScale / screenWidth * 2;
        float h = charData->Size.gY() * textScale / screenHeight * 2;
        // update VBO for each character
        float vertices[6 * 4] = {
             xpos,     ypos + h,   0.0f, 0.0f ,
             xpos,     ypos,       0.0f, 1.0f ,
             xpos + w, ypos,       1.0f, 1.0f ,

             xpos,     ypos + h,   0.0f, 0.0f ,
             xpos + w, ypos,       1.0f, 1.0f ,
             xpos + w, ypos + h,   1.0f, 0.0f
        };

        TEXT_VB.SetSubData(0, 6 * 4, vertices);
        Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, 6);
        posX += (charData->Advance) * textScale / screenWidth * 2;
    }

#undef getCharData
}
