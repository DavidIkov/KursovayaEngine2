
#include"TextRenderer.h"
#include"Tools/DebuggingTools.h"
#include"Graphics/Primitives/Renderer.h"
#include"Tools/GLDebug.h"
#include"FreeType/ft2build.h"
#include"Tools/BinarySearch.h"
#include"Shader.h"
#include FT_FREETYPE_H

namespace GP = Graphics::Primitives;
namespace GA = Graphics::Abstractions;

void* GA::TextRendererClass::FreeTypeLib = nullptr;

GA::TextRendererClass::TextRendererClass(const wchar_t* vertexShaderDir, const wchar_t* fragmentShaderDir) :TextPreset(
    false, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise,
    false, true, GP::RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual,
    false, 0, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::Equal, 1, 255, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
    GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
    true, 0, 0, 0, 0, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha,
    0.f, 0.f, 0.f, 1.f
) {
    if (FreeTypeLib == nullptr) {
        FreeTypeLib = (void*)(new FT_Library);

        if (FT_Init_FreeType((FT_Library*)FreeTypeLib)) {
            DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"failed to initialize FreeType library",KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_INITIALIZE_LIBRARY });
        }
    }

    {
        GP::ShaderClass VS(vertexShaderDir, GP::ShaderClass::TypesEnum::Vertex);
        VS.Compile();
        TEXT_SP.AttachShader(VS.gID());

        GP::ShaderClass FS(fragmentShaderDir, GP::ShaderClass::TypesEnum::Fragment);
        FS.Compile();
        TEXT_SP.AttachShader(FS.gID());

        TEXT_SP.LinkShaders();

        TEXT_SP.Bind();
        TEXT_SP.SetUniform1i(TEXT_SP.GetUniformIDByName("u_TextTexture"), 0);
    }

    TEXT_VA.Bind();
    TEXT_VB.Bind();
    TEXT_VB.SetData(6 * 4 * sizeof(float), nullptr, GP::VertexBufferClass::BufferDataUsageEnum::DynamicDraw);
    TEXT_VB.SetLayout(GP::VertexBufferClass::BufferDataTypeEnum::Float, { 2,2 });
    TEXT_VA.Unbind();
}

GA::TextRendererClass::~TextRendererClass() {
    Fonts.Clear();
    FT_Done_FreeType(*(FT_Library*)FreeTypeLib);
    delete (FT_Library*)FreeTypeLib;
}

StalkerClass GA::TextRendererClass::AddFont(unsigned int characterSize, const char* fontDir, const wchar_t* characters) {
    Fonts.InsertByConstructor(Fonts.gLength(), characterSize, fontDir, characters);
    return StalkerClass(&Fonts, Fonts.gLength() - 1);
}

GA::TextRendererClass::FontStruct::FontStruct(const FontStruct&& toCopy) :
    Texture(std::move(toCopy.Texture)), FreeTypeFace(toCopy.FreeTypeFace), Characters(toCopy.Characters),
    MaxCharacterUp(toCopy.MaxCharacterUp),MaxCharacterDown(toCopy.MaxCharacterDown) {
    toCopy.Deleted = true;
}

GA::TextRendererClass::FontStruct::~FontStruct() {
    if (not Deleted) {
        Deleted = true;
        FT_Done_Face(*(FT_Face*)FreeTypeFace);
    }
}


GA::TextRendererClass::FontStruct::FontStruct(unsigned int characterSize, const char* fontDir, const wchar_t* chars) :
    Texture(GP::TextureClass::DimensionsEnum::Two, Vector3U(0, 0, 0), false, nullptr, 0, GP::TextureClass::SettingsStruct{
                    GP::TextureClass::SettingsStruct::WrapTypeEnum::ClampToBorder,GP::TextureClass::SettingsStruct::WrapTypeEnum::ClampToBorder,
                    GP::TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Linear,GP::TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Linear,
                    GP::TextureClass::SettingsStruct::DepthStencilReadModeEnum::Depth },
                    GP::TextureClass::DataSettingsStruct{ GP::TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Red,
                    GP::TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::Red,GP::TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedByte })
{ 
    FreeTypeFace = new FT_Face;

    if (FT_New_Face(*(FT_Library*)TextRendererClass::FreeTypeLib, fontDir, 0, (FT_Face*)FreeTypeFace)) {
        std::string errMsg;
        errMsg += "FreeType error: failed to open font \"";
        errMsg += fontDir;
        errMsg += "\"";
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,errMsg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }

    FT_Set_Pixel_Sizes(*(FT_Face*)FreeTypeFace, 0, characterSize);


    unsigned int charsAmount = 0; while (chars[charsAmount] != L'\0') charsAmount++;

    Characters.ChangeCapacity(charsAmount);
    std::vector<unsigned char*> buffers; buffers.reserve(charsAmount);


    unsigned int maxWidth = 0; unsigned int maxHeight = 0;
    unsigned int totalXSize = 0;

    for (unsigned int chi = 0; chi < charsAmount; chi++) {

        unsigned int unicodeInd = (unsigned int)chars[chi];


        unsigned int insertInd = (Characters.gLength() == 0) ? 0 : BinarySearch(&Characters[0], Characters.gLength(), unicodeInd,
            +[](const unsigned int& v1, const CharacterStruct& v2)->bool {return v2.UnicodeInd > v1; },
            +[](const unsigned int& v1, const CharacterStruct& v2)->bool {return v1 == v2.UnicodeInd; });

        if (insertInd<Characters.gLength() and Characters[insertInd].UnicodeInd == unicodeInd) continue;

        if (FT_Load_Char(*(FT_Face*)FreeTypeFace, unicodeInd, FT_LOAD_RENDER))
        {
            DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"FreeType ERROR: FAILED TO LOAD CHAR",KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
            continue;
        }
        FT_Face& face = *(FT_Face*)FreeTypeFace;

        unsigned char* newBuffer = new unsigned char[face->glyph->bitmap.width * face->glyph->bitmap.rows];
        memcpy(newBuffer, face->glyph->bitmap.buffer, sizeof(unsigned char) * face->glyph->bitmap.width * face->glyph->bitmap.rows);
        buffers.insert(buffers.begin() + insertInd, newBuffer);

        if (face->glyph->bitmap.width > maxWidth) maxWidth = face->glyph->bitmap.width;
        if (face->glyph->bitmap.rows > maxHeight) maxHeight = face->glyph->bitmap.rows;

        if (face->glyph->bitmap_top > MaxCharacterUp) MaxCharacterUp = face->glyph->bitmap_top;
        if ((int)face->glyph->bitmap.rows - face->glyph->bitmap_top > MaxCharacterDown) MaxCharacterDown = (int)face->glyph->bitmap.rows - face->glyph->bitmap_top;

        totalXSize += face->glyph->bitmap.width;

        Characters.InsertByConstructor(insertInd, CharacterStruct{
            unicodeInd,
            Vector2U(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Vector2I(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x / 64,
            Vector2F(0,0),0
            });
    }

    totalXSize += charsAmount + 1;

    unsigned char* textureBuffer = new unsigned char[totalXSize * maxHeight] {0};
    
    {
        unsigned int xOffset = 1;
        for (unsigned int ci = 0; ci < Characters.gLength(); ci++) {
            CharacterStruct& char_ = Characters[ci];
            char_.XOffsetInTexture = xOffset / (float)totalXSize;
            char_.SizeInTexture = Vector2F(char_.Size[0] / (float)totalXSize, char_.Size[1] / (float)maxHeight);
            xOffset += char_.Size[0] + 1;
        }
    }

    {
        unsigned int xOffset = 0;
        for (unsigned int ci = 0; ci < Characters.gLength(); ci++) {
            CharacterStruct& char_ = Characters[ci];
            for (unsigned int x = 0; x < char_.Size[0]; x++) for (unsigned int y = 0; y < char_.Size[1]; y++) {
                textureBuffer[ci + 1 + y * totalXSize + xOffset + x] = buffers[ci][(char_.Size[1] - y - 1) * char_.Size[0] + x];
            }

            xOffset += char_.Size[0];
        }
    }

	glSC(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // disable byte-alignment restriction

    Texture.ChangeData(Vector3U(totalXSize, maxHeight, 0), textureBuffer, 0);
    
	glSC(glPixelStorei(GL_UNPACK_ALIGNMENT, 4)); // enable byte-alignment back
    

	delete[] textureBuffer;
    for (unsigned int bi = 0; bi < charsAmount; bi++) delete[] buffers[bi];


}

static constexpr unsigned int MAX_TEXT_LEN = 100000;

void GA::TextRendererClass::RenderText(const StalkerClass& fontStalker, const wchar_t* text, Vector2F pos, Vector2F localOffset, Vector2U pixelsInTexture,
    Vector2F boxSize, float lineSizeInBox, const wchar_t* dividingSymbols) {
    
    if (boxSize[0] == 0 and boxSize[1] == 0) return;//theres nothing we can do

	TextPreset.Bind();
    TEXT_SP.Bind();
    TEXT_VA.Bind();
    TEXT_VB.Bind();

	FontStruct& font = fontStalker.GetTarget<FontStruct>();

    font.Texture.Bind(0);

    unsigned int textLen = 0;
    while (textLen != MAX_TEXT_LEN and text[textLen] != L'\0') textLen++;
    if (textLen == MAX_TEXT_LEN) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"Text is too big OR no end symbol was found",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });

    std::vector<unsigned int> charsInds; charsInds.resize(textLen);//adding +1 to mark unexisting characters, ind=0 will mean that character not found
    for (unsigned int ci = 0; ci < textLen; ci++) {
        charsInds[ci] = BinarySearch(
            &font.Characters[0], font.Characters.gLength(), (unsigned int)text[ci],
            +[](const unsigned int& num, const FontStruct::CharacterStruct& ch)->bool {return num < ch.UnicodeInd; },
            +[](const unsigned int& num, const FontStruct::CharacterStruct& ch)->bool {return num == ch.UnicodeInd; });
        if (charsInds[ci] >= font.Characters.gLength() or font.Characters[charsInds[ci]].UnicodeInd != (unsigned int)text[ci]) charsInds[ci] = 0;
		else charsInds[ci]++;
    }


    unsigned int fullLocalXSize = 0;
    int maxUp = 0; int maxDown = 0;
    for (unsigned int ci = 0; ci < textLen; ci++) {
        if (charsInds[ci] == 0) continue;
        FontStruct::CharacterStruct& char_ = font.Characters[charsInds[ci] - 1];
        fullLocalXSize += char_.Advance;
        int up = char_.Bearing[1]; int down = char_.Size[1] - up;
        if (maxUp < up) maxUp = up; if (maxDown < down) maxDown = down;
    }

    unsigned int fullLocalYSize = font.MaxCharacterUp + font.MaxCharacterDown;



    float localPixelsToTexPixels = 0;
    {
        float byX = (pixelsInTexture[0] * boxSize[0]) / fullLocalXSize;
        float byY = (pixelsInTexture[1] * boxSize[1] * lineSizeInBox) / fullLocalYSize;
        if (boxSize[0] != 0 and boxSize[1] == 0) localPixelsToTexPixels = byX;
        else if (boxSize[0] == 0 and boxSize[1] != 0) localPixelsToTexPixels = byY;
        else if (boxSize[0] != 0 and boxSize[1] != 0) localPixelsToTexPixels = (byX > byY) ? byY : byX;
    }
    
    float localPixelsToTexScaleByX = localPixelsToTexPixels / pixelsInTexture[0] * 2;
    float localPixelsToTexScaleByY = localPixelsToTexPixels / pixelsInTexture[1] * 2;

    Vector2F realTextBoxSize(fullLocalXSize * localPixelsToTexScaleByX, fullLocalYSize * localPixelsToTexScaleByY);

    Vector2F scaledLocalOffset = ((localOffset + 1) / 2) * -realTextBoxSize;

	float xOffset = 0;
    for (unsigned int ci = 0; ci < textLen; ci++) {
		if (charsInds[ci] == 0) continue;
        FontStruct::CharacterStruct& char_ = font.Characters[charsInds[ci] - 1];
        
        Vector2F lbp(pos[0] + xOffset + char_.Bearing[0] * localPixelsToTexScaleByX, 
            pos[1] + (font.MaxCharacterDown - ((int)char_.Size[1] - char_.Bearing[1])) * localPixelsToTexScaleByY);
        Vector2F rbp(lbp[0] + char_.Size[0] * localPixelsToTexScaleByX, lbp[1]);
        Vector2F ltp(lbp[0], lbp[1] + char_.Size[1] * localPixelsToTexScaleByY);
        Vector2F rtp(rbp[0], ltp[1]);

        Vector2F lbc(char_.XOffsetInTexture, 0);
        Vector2F rbc(lbc[0] + char_.SizeInTexture[0], 0);
        Vector2F ltc(lbc[0], char_.SizeInTexture[1]);
        Vector2F rtc(rbc[0], ltc[1]);


        lbp[0] += scaledLocalOffset[0];
        lbp[1] += scaledLocalOffset[1];
		rbp[0] += scaledLocalOffset[0];
        rbp[1] += scaledLocalOffset[1];
		ltp[0] += scaledLocalOffset[0];
        ltp[1] += scaledLocalOffset[1];
		rtp[0] += scaledLocalOffset[0];
        rtp[1] += scaledLocalOffset[1];


        float data[4 * 6] = {
            lbp[0],lbp[1],lbc[0],lbc[1],
            rbp[0],rbp[1],rbc[0],rbc[1],
            rtp[0],rtp[1],rtc[0],rtc[1],
            
            rtp[0],rtp[1],rtc[0],rtc[1],
            ltp[0],ltp[1],ltc[0],ltc[1],
            lbp[0],lbp[1],lbc[0],lbc[1],
        };

		TEXT_VB.SetSubData(0, 6 * 4 * sizeof(float), data);
        GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, 6);


        xOffset += char_.Advance * localPixelsToTexScaleByX;
   
    }

    TEXT_VA.Unbind();
}







