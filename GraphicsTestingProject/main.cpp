
#include<iostream>
#include"Tools/DebuggingTools.h"
#include"KE2_Manager.h"
#include"Windows/Window.h"
#include"Graphics/Abstractions/TextRenderer.h"
#include"Graphics/Abstractions/Texture.h"
#include"Graphics/Primitives/FrameBuffer.h"
#include"Graphics/Primitives/VertexArray.h"
#include"Graphics/Primitives/VertexBuffer.h"
#include"Graphics/Primitives/RenderBuffer.h"
#include"Graphics/Primitives/ShaderProgram.h"
#include"Graphics/Primitives/Texture.h"
#include"Graphics/Primitives/Renderer.h"
#include"Maths/Vector.h"
#include"Maths/Matrix.h"
#include"Tools/FileTypesReaders/Obj.h"

namespace GP = Graphics::Primitives;
namespace GA = Graphics::Abstractions;

unsigned int ProgramExitCode = 0;

struct Dummy {
    alignas(GA::TextRendererClass) unsigned char data[sizeof(GA::TextRendererClass)];
    Dummy() { for (unsigned int i = 0; i < (sizeof(data) / sizeof(unsigned char)); i++) data[i] = i; }
};

int main()
{

    InitializeKE2();
    
    try {

        unsigned int Width = 900;
        unsigned int Height = 600;

        WindowClass window(Width, Height, "haiiiii", false, 1);
        GA::TextRendererClass TEXT_RENDERER(L"Shaders/textNEW.vs", L"Shaders/textNEW.fs");
        StalkerClass ArialFont = TEXT_RENDERER.AddFont(50, "Fonts/arial.ttf", L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"\
            "`abcdefghijklmnopqrstuvwxyz{|}~"\
            "¿¡¬√ƒ≈®∆«»… ÀÃÕŒœ–—“”‘’÷◊ÿŸ⁄€‹›ﬁﬂˇ˛˝¸˚˙˘¯˜ˆıÙÛÚÒÔÓÌÏÎÍÈËÁÊ∏Â‰„‚·‡");

        
        GP::FrameBufferClass FB(Width, Height);
        GP::TextureClass FB_COLOR_TEX(GP::TextureClass::DimensionsEnum::Two, Vector3U(Width, Height, 0), nullptr,
            GP::TextureSettingsClass{ GP::TextureSettingsClass::WrapTypeEnum::ClampToEdge,GP::TextureSettingsClass::WrapTypeEnum::ClampToEdge,
            GP::TextureSettingsClass::DownscalingFilterFuncEnum::Nearest,GP::TextureSettingsClass::UpscalingFilterFuncEnum::Nearest,
            GP::TextureSettingsClass::DepthStencilReadModeEnum::Depth },
            GP::TextureDataSettingsClass{ GP::TextureDataSettingsClass::DataFormatOnGPU_Enum::RGB,
            GP::TextureDataSettingsClass::DataFormatOnCPU_Enum::RGB, GP::TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte }
        );
        FB.AttachTexture(FB_COLOR_TEX.gID(), GP::TextureDataSettingsClass::DataFormatOnGPU_Enum::RGB);
        /*TextureClass FB_DEPTH_STENCIL_TEX(Width, Height, nullptr, TextureClass::TypeEnum::Texture2D,
            GP::TextureSettingsClass{ GP::TextureSettingsClass::WrapTypeEnum::ClampToEdge,GP::TextureSettingsClass::WrapTypeEnum::ClampToEdge,
            GP::TextureSettingsClass::DownscalingFilterFuncEnum::Nearest,GP::TextureSettingsClass::UpscalingFilterFuncEnum::Nearest,
            GP::TextureSettingsClass::DepthStencilReadModeEnum::Stencil },
            TextureClass::DataSettingsClass{ TextureClass::DataSettingsClass::DataFormatOnGPU_Enum::DepthStencil,
            TextureClass::DataSettingsClass::DataFormatOnCPU_Enum::DepthStencil, TextureClass::DataSettingsClass::DataTypeOnCPU_Enum::UnsignedInt_24_8 });
        FB.AttachTexture(FB_DEPTH_STENCIL_TEX.gID(), TextureClass::DataSettingsClass::DataFormatOnGPU_Enum::DepthStencil);*/
        GP::RenderBufferClass RB(Width, Height, true, true);
        FB.AttachRenderBuffer(RB.gID(), true, true);
        FB.Finish();
        FB.Unbind(Width, Height);


        GP::RenderingPresetClass Preset3D(
            true, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise,
            true, true, GP::RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual,
            true,0xff, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::AlwaysPass,1,0xff, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
            GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Replace,
            true, 0,0,0,0, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.1f,0.2f,0.3f
        );
        GP::RenderingPresetClass QuadPreset(
            false, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise,
            false, true, GP::RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual,
            false, 0, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::Equal, 1, 255, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
            GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
            false, 0, 0, 0, 0, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.f,0.f,0.f
        );
        

        unsigned int floatsAmountPerVertex = 3 + 3 + 3 + 2;
        std::vector<float> VB1_DATA = ReadObjFileType(L"Models3D/koleso.obj");
        std::vector<float> VB2_DATA = ReadObjFileType(L"Models3D/sphere.obj");




        GP::VertexArrayClass VA1;

        GP::VertexBufferClass VB1;
        VB1.SetData((unsigned int)(VB1_DATA.size()*sizeof(float)), &VB1_DATA[0], GP::VertexBufferClass::BufferDataUsageEnum::StaticDraw);
        VB1.SetLayout(GP::VertexBufferClass::BufferDataTypeEnum::Float, { 3,3,3,2 });

        VA1.Unbind();

        GP::VertexArrayClass VA2;
        
        GP::VertexBufferClass VB2;
        VB2.SetData((unsigned int)(VB2_DATA.size() * sizeof(float)), &VB2_DATA[0], GP::VertexBufferClass::BufferDataUsageEnum::StaticDraw);
        VB2.SetLayout(GP::VertexBufferClass::BufferDataTypeEnum::Float, { 3,3,3,2 });

        VA2.Unbind();

        std::vector<float> quadVBD({ 1,-1,1,1,-1,1,1,-1,-1,-1,-1,1 });

        GP::VertexArrayClass QUAD_VA;

        GP::VertexBufferClass QUAD_VB;
        QUAD_VB.SetData((unsigned int)(quadVBD.size()*sizeof(float)), &quadVBD[0], GP::VertexBufferClass::BufferDataUsageEnum::StaticDraw);
        QUAD_VB.SetLayout(GP::VertexBufferClass::BufferDataTypeEnum::Float, { 2 });

        QUAD_VA.Unbind();

        GP::ShaderProgramClass SP;
        {
            GP::ShaderClass VS(L"Shaders/full3d.vs", GP::ShaderClass::TypesEnum::Vertex);
            VS.Compile();
            SP.AttachShader(VS);

            GP::ShaderClass FS(L"Shaders/full3d.fs", GP::ShaderClass::TypesEnum::Fragment);
            FS.Compile();
            SP.AttachShader(FS);

            SP.LinkShaders();
        }

        GP::ShaderProgramClass SP_OUTLINE;
        {
            GP::ShaderClass VS(L"Shaders/outline.vs", GP::ShaderClass::TypesEnum::Vertex);
            VS.Compile();
            SP_OUTLINE.AttachShader(VS);

            GP::ShaderClass FS(L"Shaders/outline.fs", GP::ShaderClass::TypesEnum::Fragment);
            FS.Compile();
            SP_OUTLINE.AttachShader(FS);

            SP_OUTLINE.LinkShaders();
        }

        GP::ShaderProgramClass SP_NORMAL;
        {
            GP::ShaderClass VS(L"Shaders/normal.vs", GP::ShaderClass::TypesEnum::Vertex);
            VS.Compile();
            SP_NORMAL.AttachShader(VS);

            GP::ShaderClass FS(L"Shaders/normal.fs", GP::ShaderClass::TypesEnum::Fragment);
            FS.Compile();
            SP_NORMAL.AttachShader(FS);

            GP::ShaderClass GS(L"Shaders/normal.gs", GP::ShaderClass::TypesEnum::Geometry);
            GS.Compile();
            SP_NORMAL.AttachShader(GS);

            SP_NORMAL.LinkShaders();
        }


        GP::ShaderProgramClass QUAD_SP;
        {
            GP::ShaderClass VS(L"Shaders/quad.vs", GP::ShaderClass::TypesEnum::Vertex);
            VS.Compile();
            QUAD_SP.AttachShader(VS);

            GP::ShaderClass FS(L"Shaders/quad.fs", GP::ShaderClass::TypesEnum::Fragment);
            FS.Compile();
            QUAD_SP.AttachShader(FS);

            QUAD_SP.LinkShaders();
        }

        


        QUAD_SP.SetUniform1i("u_Texture", 0);

        SP.SetUniform1i("u_tex1", 0);
        SP.SetUniform1i("u_tex2", 1);

        GP::TextureClass TEX0(GP::TextureClass::DimensionsEnum::Two, "Textures/blackFace.jpg",
            GP::TextureSettingsClass{ GP::TextureSettingsClass::WrapTypeEnum::Repeat,GP::TextureSettingsClass::WrapTypeEnum::Repeat,
            GP::TextureSettingsClass::DownscalingFilterFuncEnum::Linear,GP::TextureSettingsClass::UpscalingFilterFuncEnum::Linear,
            GP::TextureSettingsClass::DepthStencilReadModeEnum::Depth });
        GP::TextureClass TEX1(GP::TextureClass::DimensionsEnum::Two, "Textures/simpleFace.png",
            GP::TextureSettingsClass{ GP::TextureSettingsClass::WrapTypeEnum::Repeat,GP::TextureSettingsClass::WrapTypeEnum::Repeat,
            GP::TextureSettingsClass::DownscalingFilterFuncEnum::Linear,GP::TextureSettingsClass::UpscalingFilterFuncEnum::Linear,
            GP::TextureSettingsClass::DepthStencilReadModeEnum::Depth });
        

        
        
        Vector3F Object1Position(0, 0, 2);
        Matrix33F Object1RotationMatrix(1, 0, 0, 0, 1, 0, 0, 0, 1);
        Vector3F Object1Scale(0.5,0.5,0.5);
        Vector3F Object2Position(2, 0, 3);
        Matrix33F Object2RotationMatrix(1, 0, 0, 0, 1, 0, 0, 0, 1);
        Vector3F Object2Scale(2,2,2);

        Vector3F LightPosition(0, 0, 0);

        Vector3F CameraPosition(0, 0, 0);
        Matrix33F CameraRotationMatrix(1, 0, 0, 0, 1, 0, 0, 0, 1);
        Matrix33F InversedCameraRotationMatrix = CameraRotationMatrix.GetInversedMatrix(CameraRotationMatrix.GetDeterminant());

        float CameraVerticalFov = 80.f / 180.f * 3.14f;

        float near = 0.1f;
        float far = 100;

        Vector2F ResolutionLength(tanf(CameraVerticalFov / 2) * Width / Height, tanf(CameraVerticalFov / 2));

        EventConnectionsHandlerClass EventsHandler;

        EventsHandler.ConnectToEvent(&window.gKeyboardHandle().gPressableKeyEvent(KeyboardClass::PressableKeysEnum::L), [&window](void* pressedDown) {
            if (*(bool*)pressedDown) {
                if (window.gCursorMode() == CursorModes::Free) window.SetCursorMode(CursorModes::LockedAndInvisible);
                else window.SetCursorMode(CursorModes::Free);
            }
            
            });

        float time = 0;

        while (!window.WindowWaitingToClose()) {
            window.UpdateMouseData();

            time += 0.1f;

            Object2Scale = Vector3F(sinf(time)+1);

            Vector2I MouseDelta;
            window.gCursorDelta(&MouseDelta);


            float cameraSpeed = 0.04f;
            if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::W)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3F(0, 0, cameraSpeed);
            if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::S)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3F(0, 0, -cameraSpeed);
            if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::A)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3F(-cameraSpeed, 0, 0);
            if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::D)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3F(cameraSpeed, 0, 0);
            if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::Q)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3F(0, -cameraSpeed, 0);
            if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::E)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3F(0, cameraSpeed, 0);


            {
                Vector2F CameraRotationByDelta = (Vector2F)MouseDelta / Vector2F((float)Width, (float)Height) / ResolutionLength;
                CameraRotationByDelta[0] = atanf(CameraRotationByDelta[0]); CameraRotationByDelta[1] = atanf(CameraRotationByDelta[1]);

                CameraRotationMatrix = CameraRotationMatrix.RotateIn3DByAnglesC<0, 1, 2>(CameraRotationByDelta[1], 0, 0);
                Vector3F xv(1, 0, 0);
                Vector3F zv(0, 0, 1);
                Matrix23F rotMat(1, 0, 0, 0, 0, 1);
                xv = rotMat.RotateVectorC<0, 1>(xv, -CameraRotationByDelta[0]);
                zv = rotMat.RotateVectorC<0, 1>(zv, -CameraRotationByDelta[0]);
                CameraRotationMatrix = Matrix33F(xv[0], xv[1], xv[2], 0, 1, 0, zv[0], zv[1], zv[2]) * CameraRotationMatrix;


                InversedCameraRotationMatrix = CameraRotationMatrix.GetInversedMatrix(CameraRotationMatrix.GetDeterminant());
            }


            TEX0.Bind(0);
            TEX1.Bind(1);
            SP.Bind();

            Matrix44F ProjectionMatrix(1 / ResolutionLength[0], 0, 0, 0, 0, 1 / ResolutionLength[1], 0, 0, 0, 0, (far + near) / (far - near), 1, 0, 0, 2 * far * near / (near - far), 0);

            Object1RotationMatrix = Object1RotationMatrix.RotateIn3DByAnglesC<0, 1, 2>(0.01f, 0.01f, 0);

            SP.SetUniform3fv("u_LightPos", 1, &LightPosition[0]);
            SP.SetUniform3fv("u_CameraPosition", 1, &CameraPosition[0]);
            SP.SetUniformMatrix3fv("u_InversedCameraRotationMatrix", 1, false, &InversedCameraRotationMatrix[0]);
            SP.SetUniformMatrix4fv("u_ProjectionMatrix", 1, false, &ProjectionMatrix[0]);
            SP.SetUniform1f("u_VisualData.Shininess", 4.f);
            SP.SetUniform1f("u_VisualData.LightSourceReflectionMaxAngle", 40.f / 180.f * 3.14f);
            SP.SetUniform1f("u_VisualData.MinColorMultiplierForSurfaceLighting", 0.05f);

            FB.Bind();

            Preset3D.Bind();
            FB.ClearAllBuffers();

            VA1.Bind();

            SP.SetUniform3fv("u_ObjectPosition", 1, &Object1Position[0]);
            SP.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object1RotationMatrix[0]);
            SP.SetUniform3fv("u_ObjectScale", 1, &Object1Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

            VA2.Bind();

            SP.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
            SP.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
            SP.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);

            Preset3D.sStencilTest_BaseMask(0);
            Preset3D.sStencilTest_ComparisonType(GP::RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::NotEqual);
            Preset3D.sDepthTest_Enabled(false);

            {//outline


                SP_OUTLINE.Bind();
                SP_OUTLINE.SetUniform3fv("u_CameraPosition", 1, &CameraPosition[0]);
                SP_OUTLINE.SetUniformMatrix3fv("u_InversedCameraRotationMatrix", 1, false, &InversedCameraRotationMatrix[0]);
                SP_OUTLINE.SetUniformMatrix4fv("u_ProjectionMatrix", 1, false, &ProjectionMatrix[0]);
                SP_OUTLINE.SetUniform1f("u_OutlineScale", 0.1f);
                
                VA1.Bind();

                SP_OUTLINE.SetUniform3fv("u_ObjectPosition", 1, &Object1Position[0]);
                SP_OUTLINE.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object1RotationMatrix[0]);
                SP_OUTLINE.SetUniform3fv("u_ObjectScale", 1, &Object1Scale[0]);
                GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

                VA2.Bind();

                SP_OUTLINE.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
                SP_OUTLINE.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
                SP_OUTLINE.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
                GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);
            }

            Preset3D.sStencilTest_BaseMask(0xff);
            Preset3D.sStencilTest_ComparisonType(GP::RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::AlwaysPass);
            Preset3D.sDepthTest_Enabled(true);

            //////

            SP_NORMAL.Bind();
            SP_NORMAL.SetUniform3fv("u_CameraPosition", 1, &CameraPosition[0]);
            SP_NORMAL.SetUniformMatrix3fv("u_InversedCameraRotationMatrix", 1, false, &InversedCameraRotationMatrix[0]);
            SP_NORMAL.SetUniformMatrix4fv("u_ProjectionMatrix", 1, false, &ProjectionMatrix[0]);

            VA1.Bind();

            SP_NORMAL.SetUniform3fv("u_ObjectPosition", 1, &Object1Position[0]);
            SP_NORMAL.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object1RotationMatrix[0]);
            SP_NORMAL.SetUniform3fv("u_ObjectScale", 1, &Object1Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

            VA2.Bind();

            SP_NORMAL.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
            SP_NORMAL.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
            SP_NORMAL.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);


            FB.Unbind(Width,Height);

            QuadPreset.Bind();
            window.ClearColorBuffer();
            QUAD_SP.Bind();

            QUAD_SP.SetUniform1i("u_Texture", 0);

            FB_COLOR_TEX.Bind(0);

            QUAD_VA.Bind();

            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)quadVBD.size() / 2);

            QUAD_VA.Unbind();

            {//text
                //for (unsigned int i = 0; i < 5; i++) {
                    /*TEXT_RENDERER.DrawText(ArialFont, L"abcds",
                        Width, Height,
                        1,
                        -1.f + 2.f * ((float)i / 5), -1.f + 2.f * ((float)i / 5),
                        -1,-1,
                        0, TextGP::RendererNamespace::ClampingFuncs::None,
                        0, true, TextGP::RendererNamespace::ClampingFuncs::None);*/
                TEXT_RENDERER.RenderText(ArialFont, L"english ÛÒÒÍËÈ …∆®!:(|&", Vector2F(-1,0), Vector2F(-1,0), Vector2U(Width, Height),
                    Vector2F(1, 0), 0.5f);
                //}
            }

            window.SwapScreenBuffers();
            window.ProcessEvents();
        }

    }
    catch (KURSAVAYAENGINE2_CORE_ERRORS& err) { ProgramExitCode = (unsigned int)err; }

	UninitializeKE2();
    return ProgramExitCode;
}