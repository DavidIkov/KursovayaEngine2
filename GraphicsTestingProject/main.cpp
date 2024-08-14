
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
#include"Maths/Vector2.h"
#include"Maths/Vector3.h"
#include"Maths/Matrix.h"
#include"Tools/FileTypesReaders/Obj.h"

int main()
{
    
    InitializeKursovayaEngine2();
    

    try {

        unsigned int Width = 900;
        unsigned int Height = 600;


        Window window(Width, Height, "haiiiii", false, 1);


        TextRenderer TEXT_RENDERER("Fonts/arial.ttf", "Shaders/text.vs", "Shaders/text.fs");


        FrameBuffer FB(Width, Height);
        Texture FB_COLOR_TEX(Width, Height, TextureStorageType::RGB,
            { TextureWrapType::ClampToEdge,TextureWrapType::ClampToEdge,TextureDownscalingFilterFunc::Nearest,TextureUpscalingFilterFunc::Nearest,
            TextureDepthStencilReadMode::Depth });
        FB.AttachTexture(FB_COLOR_TEX);
        Texture FB_DEPTH_STENCIL_TEX(Width, Height, TextureStorageType::DepthStencil,
            { TextureWrapType::ClampToEdge,TextureWrapType::ClampToEdge,TextureDownscalingFilterFunc::Nearest,TextureUpscalingFilterFunc::Nearest,
            TextureDepthStencilReadMode::Stencil});
        FB.AttachTexture(FB_DEPTH_STENCIL_TEX);
        //RenderBuffer RB(Width, Height, true, true);
        //FB.AttachRenderBuffer(RB);
        FB.Finish();
        FB.Unbind(Width, Height);

        RenderingPreset Preset3D(
            true, RenderingPresetEnumArguments::FaceCulling::FaceToCull::Back, RenderingPresetEnumArguments::FaceCulling::FaceDetermination::Clockwise,
            true, true, RenderingPresetEnumArguments::DepthTest::TypeOfComparison::LessOrEqual,
            true,0xff, RenderingPresetEnumArguments::StencilTest::TypeOfComparison::AlwaysPass,1,0xff, RenderingPresetEnumArguments::StencilTest::Actions::Keep,
            RenderingPresetEnumArguments::StencilTest::Actions::Keep, RenderingPresetEnumArguments::StencilTest::Actions::Replace,
            true, 0,0,0,0, RenderingPresetEnumArguments::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.1f,0.2f,0.3f
        );
        RenderingPreset QuadPreset(
            false, RenderingPresetEnumArguments::FaceCulling::FaceToCull::Back, RenderingPresetEnumArguments::FaceCulling::FaceDetermination::Clockwise,
            false, true, RenderingPresetEnumArguments::DepthTest::TypeOfComparison::LessOrEqual,
            false, 0, RenderingPresetEnumArguments::StencilTest::TypeOfComparison::Equal, 1, 255, RenderingPresetEnumArguments::StencilTest::Actions::Keep,
            RenderingPresetEnumArguments::StencilTest::Actions::Keep, RenderingPresetEnumArguments::StencilTest::Actions::Keep,
            false, 0, 0, 0, 0, RenderingPresetEnumArguments::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.f,0.f,0.f
        );
        

        unsigned int floatsAmountPerVertex = 3 + 3 + 3 + 2;
        std::vector<float> VB1_DATA = ReadObjFileType("Models3D/sphere.obj");
        std::vector<float> VB2_DATA = ReadObjFileType("Models3D/sphere.obj");




        VertexArray VA1;

        VertexBuffer VB1;
        VB1.SetData(VB1_DATA, VertexBufferDataUsage::StaticDraw);
        VB1.SetLayout({ 3,3,3,2 });

        VA1.Unbind();

        VertexArray VA2;

        VertexBuffer VB2;
        VB2.SetData(VB2_DATA, VertexBufferDataUsage::StaticDraw);
        VB2.SetLayout({ 3,3,3,2 });

        VA2.Unbind();

        std::vector<float> quadVBD({ 1,-1,1,1,-1,1,1,-1,-1,-1,-1,1 });

        VertexArray QUAD_VA;

        VertexBuffer QUAD_VB;
        QUAD_VB.SetData(quadVBD, VertexBufferDataUsage::StaticDraw);
        QUAD_VB.SetLayout({ 2 });

        QUAD_VA.Unbind();

        ShaderProgram SP;
        {
            Shader VS("Shaders/full3d.vs", ShaderTypesEnum::Vertex);
            VS.Compile();
            SP.AttachShader(VS);

            Shader FS("Shaders/full3d.fs", ShaderTypesEnum::Fragment);
            FS.Compile();
            SP.AttachShader(FS);

            SP.LinkShaders();
        }

        ShaderProgram SP_OUTLINE;
        {
            Shader VS("Shaders/outline.vs", ShaderTypesEnum::Vertex);
            VS.Compile();
            SP_OUTLINE.AttachShader(VS);

            Shader FS("Shaders/outline.fs", ShaderTypesEnum::Fragment);
            FS.Compile();
            SP_OUTLINE.AttachShader(FS);

            SP_OUTLINE.LinkShaders();
        }

        ShaderProgram SP_NORMAL;
        {
            Shader VS("Shaders/normal.vs", ShaderTypesEnum::Vertex);
            VS.Compile();
            SP_NORMAL.AttachShader(VS);

            Shader FS("Shaders/normal.fs", ShaderTypesEnum::Fragment);
            FS.Compile();
            SP_NORMAL.AttachShader(FS);

            Shader GS("Shaders/normal.gs", ShaderTypesEnum::Geometry);
            GS.Compile();
            SP_NORMAL.AttachShader(GS);

            SP_NORMAL.LinkShaders();
        }


        ShaderProgram QUAD_SP;
        {
            Shader VS("Shaders/quad.vs", ShaderTypesEnum::Vertex);
            VS.Compile();
            QUAD_SP.AttachShader(VS);

            Shader FS("Shaders/quad.fs", ShaderTypesEnum::Fragment);
            FS.Compile();
            QUAD_SP.AttachShader(FS);

            QUAD_SP.LinkShaders();
        }

        


        QUAD_SP.SetUniform1i("u_Texture", 0);

        SP.SetUniform1i("u_tex1", 0);
        SP.SetUniform1i("u_tex2", 1);

        Texture TEX0("Textures/blackFace.jpg",
            { TextureWrapType::Repeat,TextureWrapType::Repeat,TextureDownscalingFilterFunc::Linear,TextureUpscalingFilterFunc::Linear,
            TextureDepthStencilReadMode::Depth });
        Texture TEX1("Textures/simpleFace.png",
            { TextureWrapType::Repeat,TextureWrapType::Repeat,TextureDownscalingFilterFunc::Linear,TextureUpscalingFilterFunc::Linear,
            TextureDepthStencilReadMode::Depth });
        

        
        
        Vector3 Object1Position(0, 0, 2);
        Matrix Object1RotationMatrix(3, 3, { 1,0,0,0,1,0,0,0,1 });
        Vector3 Object1Scale(1,1,1);
        Vector3 Object2Position(2, 0, 3);
        Matrix Object2RotationMatrix(3, 3, { 1,0,0,0,1,0,0,0,1 });
        Vector3 Object2Scale(2,2,2);

        Vector3 LightPosition(0, 0, 0);

        Vector3 CameraPosition(0, 0, 0);
        Matrix CameraRotationMatrix(3, 3, { 1,0,0,0,1,0,0,0,1 });
        Matrix InversedCameraRotationMatrix = CameraRotationMatrix.gInversedMatrix();

        float CameraVerticalFov = 80.f / 180.f * 3.14f;

        float near = 0.1f;
        float far = 100;

        Vector2 ResolutionLength(tanf(CameraVerticalFov / 2) * Width / Height, tanf(CameraVerticalFov / 2));

        EventConnectionsHandlerClass EventsHandler;

        EventsHandler.ConnectToEvent(&window.gKeyboardHandle().gPressableKeyEvent(PressableKeys::L), [&window](void* pressedDown) {
            if (*(bool*)pressedDown) {
                if (window.gCursorMode() == CursorModes::Free) window.SetCursorMode(CursorModes::LockedAndInvisible);
                else window.SetCursorMode(CursorModes::Free);
            }

            });

        float time = 0;

        while (!window.WindowWaitingToClose())
        {
            window.UpdateMouseData();

            time += 0.1f;

            Object2Scale = Vector3((sinf(time)+1)/2*20);

            Vector2 MouseDelta;
            window.gCursorDelta(&MouseDelta);


            float cameraSpeed = 0.04f;
            if (window.gKeyboardHandle().gPressableKeyState(PressableKeys::W)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(0, 0, cameraSpeed);
            if (window.gKeyboardHandle().gPressableKeyState(PressableKeys::S)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(0, 0, -cameraSpeed);
            if (window.gKeyboardHandle().gPressableKeyState(PressableKeys::A)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(-cameraSpeed, 0, 0);
            if (window.gKeyboardHandle().gPressableKeyState(PressableKeys::D)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(cameraSpeed, 0, 0);
            if (window.gKeyboardHandle().gPressableKeyState(PressableKeys::Q)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(0, -cameraSpeed, 0);
            if (window.gKeyboardHandle().gPressableKeyState(PressableKeys::E)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(0, cameraSpeed, 0);


            {
                Vector2 CameraRotationByDelta = Vector2::atan(MouseDelta / Vector2((float)Width, (float)Height) / ResolutionLength);

                Matrix resultRotation = CameraRotationMatrix * Matrix::RotateIn3DByAnglesXYZ(CameraRotationByDelta.gY(), 0, 0);
                Vector3 xv(1, 0, 0);
                Vector3 zv(0, 0, 1);
                Vector3::RotateVectors(xv, zv, -CameraRotationByDelta.gX(), xv, zv);
                resultRotation = Matrix(3, 3, { xv.gX(),xv.gY(),xv.gZ(),0,1,0,zv.gX(),zv.gY(),zv.gZ() }) * resultRotation;
                CameraRotationMatrix = resultRotation;

                InversedCameraRotationMatrix = CameraRotationMatrix.gInversedMatrix();
            }


            TEX0.Bind(0);
            TEX1.Bind(1);
            SP.Bind();

            Matrix ProjectionMatrix(4, 4, { 1 / ResolutionLength.gX(),0,0,0,0,1 / ResolutionLength.gY(),0,0,0,0,(far + near) / (far - near),1,0,0,2 * far * near / (near - far),0 });

            Object1RotationMatrix = Object1RotationMatrix * Matrix::RotateIn3DByAnglesXYZ(0.01f, 0.01f, 0);

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
            Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

            VA2.Bind();

            SP.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
            SP.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
            SP.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
            Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);

            Preset3D.sStencilTest_BaseMask(0);
            Preset3D.sStencilTest_ComparisonType(RenderingPresetEnumArguments::StencilTest::TypeOfComparison::NotEqual);
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
                Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

                VA2.Bind();

                SP_OUTLINE.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
                SP_OUTLINE.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
                SP_OUTLINE.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
                Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);
            }

            Preset3D.sStencilTest_BaseMask(0xff);
            Preset3D.sStencilTest_ComparisonType(RenderingPresetEnumArguments::StencilTest::TypeOfComparison::AlwaysPass);
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
            Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

            VA2.Bind();

            SP_NORMAL.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
            SP_NORMAL.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
            SP_NORMAL.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
            Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);



            FB.Unbind(Width,Height);

            QuadPreset.Bind();
            window.ClearColorBuffer();
            QUAD_SP.Bind();

            QUAD_SP.SetUniform1i("u_Texture", 0);

            FB_COLOR_TEX.Bind(0);

            QUAD_VA.Bind();

            Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, (int)quadVBD.size() / 2);

            QUAD_VA.Unbind();

            {//text
                for (unsigned int i = 0; i < 5; i++) {
                    TEXT_RENDERER.DrawText(L"wilson is pedophile",
                        Width, Height,
                        1,
                        -1.f + 2.f * ((float)i / 5), -1.f + 2.f * ((float)i / 5),
                        -1,-1,
                        0, TextRenderer::ClampingFuncs::None,
                        0, true, TextRenderer::ClampingFuncs::None);
                }
                
            }

            window.SwapScreenBuffers();
            window.ProcessEvents();
        }

        UninitializeKursovayaEngine2();
        return 0;
    }
    catch (KURSAVAYAENGINE2_CORE_ERRORS&) {
        UninitializeKursovayaEngine2();
        return 0;
    }
}