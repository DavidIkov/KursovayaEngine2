#include"Quad.h"
#include"GraphicsPrimitives/Renderer.h"

QuadsHandler::QuadsHandler(const wchar_t* vs, const wchar_t* fs) {

    VA.Bind();

    float data[] = {
            -1,-1,
            -1,1,
            1,1,
            1,1,
            1,-1,
            -1,-1
    };
    VB.SetData(sizeof(data), data, VertexBufferClass::BufferDataUsageEnum::StaticDraw);
    VB.SetLayout(VertexBufferClass::BufferDataTypeEnum::Float, { 2 });

    VA.Unbind();

    ShaderClass VS(vs, ShaderClass::TypesEnum::Vertex);
    VS.Compile();
    SP.AttachShader(VS);

    ShaderClass FS(fs, ShaderClass::TypesEnum::Fragment);
    FS.Compile();
    SP.AttachShader(FS);

    SP.LinkShaders();
}

QuadsHandler::Quad::Quad(QuadsHandler& handler) :Handler(handler) {}

void QuadsHandler::Quad::Draw() {
    Handler.VA.Bind();
    Handler.SP.Bind();
    Handler.SP.SetUniform2fv("u_Pos", 1, &Position[0]);
    Handler.SP.SetUniform2fv("u_Size", 1, &Size[0]);
    Handler.SP.SetUniform2fv("u_LocalOffset", 1, &LocalOffset[0]);
    Handler.SP.SetUniform3fv("u_Color", 1, &Color[0]);
    RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, 6);
    Handler.VA.Unbind();
}

QuadsHandler::ImageQuad::ImageQuad(QuadsHandler& handler, TextureClass&& tex) :Quad(handler), Tex(std::move(tex)) {};
void QuadsHandler::ImageQuad::Draw(unsigned int screenWidth, unsigned int screenHeight) {
    Handler.VA.Bind();
    Handler.SP.Bind();
    Handler.SP.SetUniform2fv("u_Pos", 1, &Position[0]);
    Vector2F curSize = Size;
    switch (TexClampingType) {
    case TextureClampingType::SquareByX:
    {
        curSize[1] = Size[0] * (float)screenWidth / (float)screenHeight;
        break;
    }
    case TextureClampingType::SquareByY:
    {
        curSize[0] = Size[1] * (float)screenHeight / (float)screenWidth;
        break;
    }
    case TextureClampingType::SquareByXY:
    {
        curSize = Vector2F((float)screenWidth, (float)screenHeight) * Size;//get size in pixels
        if (curSize[0] > curSize[1]) {
            curSize = Vector2F(curSize[1]) / Vector2F((float)screenWidth, (float)screenHeight);
        }
        else {
            curSize = Vector2F(curSize[0]) / Vector2F((float)screenWidth, (float)screenHeight);
        }
        break;
    }
    }
    Handler.SP.SetUniform2fv("u_Size", 1, &curSize[0]);
    Handler.SP.SetUniform2fv("u_LocalOffset", 1, &LocalOffset[0]);
    Tex.Bind(0);
    RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, 6);
    Handler.VA.Unbind();
}