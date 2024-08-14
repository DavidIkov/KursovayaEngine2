#include"Quad.h"
#include"GraphicsPrimitives/Renderer.h"

QuadsHandler::QuadsHandler(const char* vs, const char* fs) {

    VA.Bind();

    float data[] = {
            -1,-1,
            -1,1,
            1,1,
            1,1,
            1,-1,
            -1,-1
    };
    VB.SetData(data, sizeof(data), VertexBufferDataUsage::StaticDraw);
    VB.SetLayout({ 2 });

    VA.Unbind();

    Shader VS(vs, ShaderTypesEnum::Vertex);
    VS.Compile();
    SP.AttachShader(VS);

    Shader FS(fs, ShaderTypesEnum::Fragment);
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
    Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, 6);
    Handler.VA.Unbind();
}

QuadsHandler::ImageQuad::ImageQuad(QuadsHandler& handler, Texture&& tex) :Quad(handler), Tex(std::move(tex)) {};
void QuadsHandler::ImageQuad::Draw(unsigned int screenWidth, unsigned int screenHeight) {
    Handler.VA.Bind();
    Handler.SP.Bind();
    Handler.SP.SetUniform2fv("u_Pos", 1, &Position[0]);
    Vector2 curSize = Size;
    switch (TexClampingType) {
    case TextureClampingType::SquareByX:
    {
        curSize.sY(Size.gX() * screenWidth / screenHeight);
        break;
    }
    case TextureClampingType::SquareByY:
    {
        curSize.sX(Size.gY() * screenHeight / screenWidth);
        break;
    }
    case TextureClampingType::SquareByXY:
    {
        curSize = Vector2((float)screenWidth, (float)screenHeight) * Size;//get size in pixels
        if (curSize.gX() > curSize.gY()) {
            curSize = Vector2(curSize.gY()) / Vector2((float)screenWidth, (float)screenHeight);
        }
        else {
            curSize = Vector2(curSize.gX()) / Vector2((float)screenWidth, (float)screenHeight);
        }
        break;
    }
    }
    Handler.SP.SetUniform2fv("u_Size", 1, &curSize[0]);
    Handler.SP.SetUniform2fv("u_LocalOffset", 1, &LocalOffset[0]);
    Tex.Bind(0);
    Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, 6);
    Handler.VA.Unbind();
}