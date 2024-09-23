
#include<iostream>
#include"Tools/DebuggingTools.h"
#include"KursovayaEngine2Manager.h"
#include"Windows/Window.h"
#include"GraphicsAbstractions/TextRenderer.h"
#include"GraphicsPrimitives/FrameBuffer.h"
#include"GraphicsPrimitives/VertexArray.h"
#include"GraphicsPrimitives/VertexBuffer.h"
#include"GraphicsPrimitives/RenderBuffer.h"
#include"GraphicsPrimitives/ShaderProgram.h"
#include"GraphicsPrimitives/Texture.h"
#include"GraphicsPrimitives/Renderer.h"
#include"Maths/Vector.h"
#include"Maths/Matrix.h"
#include"Tools/FileTypesReaders/Obj.h"

int main()
{
    
    InitializeKursovayaEngine2();
    
    try {

        unsigned int Width = 900;
        unsigned int Height = 600;


        WindowClass window(Width, Height, "haiiiii", false, 1);


        TextRendererClass TEXT_RENDERER(L"Shaders/textNEW.vs", L"Shaders/textNEW.fs");
        StalkerClass ArialFont = TEXT_RENDERER.AddFont(50, "Fonts/arial.ttf", L" !\"#שפ$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"\
            "`abcdefghijklmnopqrstuvwxyz{|}~");
        
        FrameBufferClass FB(Width, Height);
        Texture2DClass FB_COLOR_TEX(Vector2U(Width, Height), nullptr,
            TextureSettingsClass{ TextureSettingsClass::WrapTypeEnum::ClampToEdge,TextureSettingsClass::WrapTypeEnum::ClampToEdge,
            TextureSettingsClass::DownscalingFilterFuncEnum::Nearest,TextureSettingsClass::UpscalingFilterFuncEnum::Nearest,
            TextureSettingsClass::DepthStencilReadModeEnum::Depth },
            TextureDataSettingsClass{ TextureDataSettingsClass::DataFormatOnGPU_Enum::RGB,
            TextureDataSettingsClass::DataFormatOnCPU_Enum::RGB, TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte }
        );
        FB.AttachTexture(FB_COLOR_TEX.gID(), TextureDataSettingsClass::DataFormatOnGPU_Enum::RGB);
        /*TextureClass FB_DEPTH_STENCIL_TEX(Width, Height, nullptr, TextureClass::TypeEnum::Texture2D,
            TextureSettingsClass{ TextureSettingsClass::WrapTypeEnum::ClampToEdge,TextureSettingsClass::WrapTypeEnum::ClampToEdge,
            TextureSettingsClass::DownscalingFilterFuncEnum::Nearest,TextureSettingsClass::UpscalingFilterFuncEnum::Nearest,
            TextureSettingsClass::DepthStencilReadModeEnum::Stencil },
            TextureClass::DataSettingsClass{ TextureClass::DataSettingsClass::DataFormatOnGPU_Enum::DepthStencil,
            TextureClass::DataSettingsClass::DataFormatOnCPU_Enum::DepthStencil, TextureClass::DataSettingsClass::DataTypeOnCPU_Enum::UnsignedInt_24_8 });
        FB.AttachTexture(FB_DEPTH_STENCIL_TEX.gID(), TextureClass::DataSettingsClass::DataFormatOnGPU_Enum::DepthStencil);*/
        RenderBufferClass RB(Width, Height, true, true);
        FB.AttachRenderBuffer(RB.gID(), true, true);
        FB.Finish();
        FB.Unbind(Width, Height);

        RenderingPresetClass Preset3D(
            true, RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back, RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise,
            true, true, RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual,
            true,0xff, RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::AlwaysPass,1,0xff, RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
            RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep, RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Replace,
            true, 0,0,0,0, RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.1f,0.2f,0.3f
        );
        RenderingPresetClass QuadPreset(
            false, RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back, RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise,
            false, true, RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual,
            false, 0, RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::Equal, 1, 255, RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
            RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep, RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
            false, 0, 0, 0, 0, RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.f,0.f,0.f
        );
        

        unsigned int floatsAmountPerVertex = 3 + 3 + 3 + 2;
        std::vector<float> VB1_DATA = ReadObjFileType(L"Models3D/xyu.obj");
        std::vector<float> VB2_DATA = ReadObjFileType(L"Models3D/sphere.obj");




        VertexArrayClass VA1;

        VertexBufferClass VB1;
        VB1.SetData((unsigned int)(VB1_DATA.size()*sizeof(float)), &VB1_DATA[0], VertexBufferClass::BufferDataUsageEnum::StaticDraw);
        VB1.SetLayout(VertexBufferClass::BufferDataTypeEnum::Float, { 3,3,3,2 });

        VA1.Unbind();

        VertexArrayClass VA2;

        VertexBufferClass VB2;
        VB2.SetData((unsigned int)(VB2_DATA.size() * sizeof(float)), &VB2_DATA[0], VertexBufferClass::BufferDataUsageEnum::StaticDraw);
        VB2.SetLayout(VertexBufferClass::BufferDataTypeEnum::Float, { 3,3,3,2 });

        VA2.Unbind();

        std::vector<float> quadVBD({ 1,-1,1,1,-1,1,1,-1,-1,-1,-1,1 });

        VertexArrayClass QUAD_VA;

        VertexBufferClass QUAD_VB;
        QUAD_VB.SetData((unsigned int)(quadVBD.size()*sizeof(float)), &quadVBD[0], VertexBufferClass::BufferDataUsageEnum::StaticDraw);
        QUAD_VB.SetLayout(VertexBufferClass::BufferDataTypeEnum::Float, { 2 });

        QUAD_VA.Unbind();

        ShaderProgramClass SP;
        {
            ShaderClass VS(L"Shaders/full3d.vs", ShaderClass::TypesEnum::Vertex);
            VS.Compile();
            SP.AttachShader(VS);

            ShaderClass FS(L"Shaders/full3d.fs", ShaderClass::TypesEnum::Fragment);
            FS.Compile();
            SP.AttachShader(FS);

            SP.LinkShaders();
        }

        ShaderProgramClass SP_OUTLINE;
        {
            ShaderClass VS(L"Shaders/outline.vs", ShaderClass::TypesEnum::Vertex);
            VS.Compile();
            SP_OUTLINE.AttachShader(VS);

            ShaderClass FS(L"Shaders/outline.fs", ShaderClass::TypesEnum::Fragment);
            FS.Compile();
            SP_OUTLINE.AttachShader(FS);

            SP_OUTLINE.LinkShaders();
        }

        ShaderProgramClass SP_NORMAL;
        {
            ShaderClass VS(L"Shaders/normal.vs", ShaderClass::TypesEnum::Vertex);
            VS.Compile();
            SP_NORMAL.AttachShader(VS);

            ShaderClass FS(L"Shaders/normal.fs", ShaderClass::TypesEnum::Fragment);
            FS.Compile();
            SP_NORMAL.AttachShader(FS);

            ShaderClass GS(L"Shaders/normal.gs", ShaderClass::TypesEnum::Geometry);
            GS.Compile();
            SP_NORMAL.AttachShader(GS);

            SP_NORMAL.LinkShaders();
        }


        ShaderProgramClass QUAD_SP;
        {
            ShaderClass VS(L"Shaders/quad.vs", ShaderClass::TypesEnum::Vertex);
            VS.Compile();
            QUAD_SP.AttachShader(VS);

            ShaderClass FS(L"Shaders/quad.fs", ShaderClass::TypesEnum::Fragment);
            FS.Compile();
            QUAD_SP.AttachShader(FS);

            QUAD_SP.LinkShaders();
        }

        


        QUAD_SP.SetUniform1i("u_Texture", 0);

        SP.SetUniform1i("u_tex1", 0);
        SP.SetUniform1i("u_tex2", 1);

        Texture2DClass TEX0("Textures/blackFace.jpg",
            TextureSettingsClass{ TextureSettingsClass::WrapTypeEnum::Repeat,TextureSettingsClass::WrapTypeEnum::Repeat,
            TextureSettingsClass::DownscalingFilterFuncEnum::Linear,TextureSettingsClass::UpscalingFilterFuncEnum::Linear,
            TextureSettingsClass::DepthStencilReadModeEnum::Depth });
        Texture2DClass TEX1("Textures/simpleFace.png",
            TextureSettingsClass{ TextureSettingsClass::WrapTypeEnum::Repeat,TextureSettingsClass::WrapTypeEnum::Repeat,
            TextureSettingsClass::DownscalingFilterFuncEnum::Linear,TextureSettingsClass::UpscalingFilterFuncEnum::Linear,
            TextureSettingsClass::DepthStencilReadModeEnum::Depth });
        

        
        
        Vector3F Object1Position(0, 0, 2);
        Matrix33F Object1RotationMatrix(1, 0, 0, 0, 1, 0, 0, 0, 1);
        Vector3F Object1Scale(1,1,1);
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
            RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

            VA2.Bind();

            SP.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
            SP.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
            SP.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
            RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);

            Preset3D.sStencilTest_BaseMask(0);
            Preset3D.sStencilTest_ComparisonType(RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::NotEqual);
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
                RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

                VA2.Bind();

                SP_OUTLINE.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
                SP_OUTLINE.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
                SP_OUTLINE.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
                RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);
            }

            Preset3D.sStencilTest_BaseMask(0xff);
            Preset3D.sStencilTest_ComparisonType(RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::AlwaysPass);
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
            RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

            VA2.Bind();

            SP_NORMAL.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
            SP_NORMAL.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
            SP_NORMAL.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
            RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);


            FB.Unbind(Width,Height);

            QuadPreset.Bind();
            window.ClearColorBuffer();
            QUAD_SP.Bind();

            QUAD_SP.SetUniform1i("u_Texture", 0);

            FB_COLOR_TEX.Bind(0);

            QUAD_VA.Bind();

            RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, (int)quadVBD.size() / 2);

            QUAD_VA.Unbind();

            {//text
                //for (unsigned int i = 0; i < 5; i++) {
                    /*TEXT_RENDERER.DrawText(ArialFont, L"abcds",
                        Width, Height,
                        1,
                        -1.f + 2.f * ((float)i / 5), -1.f + 2.f * ((float)i / 5),
                        -1,-1,
                        0, TextRendererNamespace::ClampingFuncs::None,
                        0, true, TextRendererNamespace::ClampingFuncs::None);*/
                TEXT_RENDERER.RenderText(ArialFont, L"abc@%אשפ", Vector2F(-1,0), Vector2F(-1,0), Vector2U(Width, Height),
                    Vector2F(1, 0), 0.5f);
                //}
            }

            window.SwapScreenBuffers();
            window.ProcessEvents();
        }

        window.Destroy();

        UninitializeKursovayaEngine2();
        return 0;
    }
    catch (KURSAVAYAENGINE2_CORE_ERRORS&) {
        UninitializeKursovayaEngine2();
        return 0;
    }//todo console dont close when window is closed, need to fix this bug
}