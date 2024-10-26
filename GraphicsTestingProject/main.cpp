
#include<iostream>
#include"Tools/DebuggingTools.h"
#include"KE2_Manager.h"
#include"Windows/Window.h"
#include"Graphics/Abstractions/TextRenderer.h"
#include"Graphics/Abstractions/Texture.h"
#include"Graphics/Abstractions/Shader.h"
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
#include"Tools/Time.h"

namespace GP = Graphics::Primitives;
namespace GA = Graphics::Abstractions;

unsigned int ProgramExitCode = 0;

int main()
{

    InitializeKE2();
    
    try {


        WindowClass::MonitorDataStruct MonitorData = WindowClass::gPrimaryMonitorData();

        unsigned int Width = MonitorData.Size[0] / 2;
        unsigned int Height = MonitorData.Size[1] / 2;

        WindowClass window(&Width, &Height, "haiiiii", false, 1);
        GA::TextRendererClass TEXT_RENDERER(L"Shaders/textNEW.vs", L"Shaders/textNEW.fs");
        StalkerClass ArialFont = TEXT_RENDERER.AddFont(50, "Fonts/arial.ttf", L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"\
            "`abcdefghijklmnopqrstuvwxyz{|}~"\
            "ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßÿþýüûúùø÷öõôóòñðïîíìëêéèçæ¸åäãâáà");

        
        GP::FrameBufferClass FB(Vector2U(Width, Height));
        GP::TextureClass FB_COLOR_TEX(GP::TextureClass::DimensionsEnum::Two, Vector3U(Width, Height, 0), nullptr,
            GP::TextureClass::SettingsStruct{ GP::TextureClass::SettingsStruct::WrapTypeEnum::ClampToEdge,GP::TextureClass::SettingsStruct::WrapTypeEnum::ClampToEdge,
            GP::TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Nearest,GP::TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Nearest,
            GP::TextureClass::SettingsStruct::DepthStencilReadModeEnum::Depth },
            GP::TextureClass::DataSettingsStruct{ GP::TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA,
            GP::TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGBA, GP::TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedByte }
        );
        FB.AttachTexture(FB_COLOR_TEX.gID(), GP::TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA);
        /*TextureClass FB_DEPTH_STENCIL_TEX(Width, Height, nullptr, TextureClass::TypeEnum::Texture2D,
            GP::TextureClass::SettingsStruct{ GP::TextureClass::SettingsStruct::WrapTypeEnum::ClampToEdge,GP::TextureClass::SettingsStruct::WrapTypeEnum::ClampToEdge,
            GP::TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Nearest,GP::TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Nearest,
            GP::TextureClass::SettingsStruct::DepthStencilReadModeEnum::Stencil },
            TextureClass::DataSettingsClass{ TextureClass::DataSettingsClass::DataFormatOnGPU_Enum::DepthStencil,
            TextureClass::DataSettingsClass::DataFormatOnCPU_Enum::DepthStencil, TextureClass::DataSettingsClass::DataTypeOnCPU_Enum::UnsignedInt_24_8 });
        FB.AttachTexture(FB_DEPTH_STENCIL_TEX.gID(), TextureClass::DataSettingsClass::DataFormatOnGPU_Enum::DepthStencil);*/
        GP::RenderBufferClass RB(Width, Height, true, true);
        FB.AttachRenderBuffer(RB.gID(), true, true);
        FB.Finish();
        FB.Unbind();
        FB.SetViewportSize(Vector2U(Width, Height));


        GP::RenderingPresetClass Preset3D(
            true, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise,
            true, true, GP::RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual,
            true,0xff, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::AlwaysPass,1,0xff, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
            GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Replace,
            true, 0,0,0,0, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.1f,0.2f,0.3f,0.f
        );
        GP::RenderingPresetClass QuadPreset(
            false, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise,
            false, true, GP::RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual,
            false, 0, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::Equal, 1, 255, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
            GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
            false, 0, 0, 0, 0, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.f,0.f,0.f,1.f
        );
        

        unsigned int floatsAmountPerVertex = 3 + 3 + 3 + 2;
        std::vector<float> VB1_DATA = ReadObjFileType(L"Models3D/koleso.obj");
        std::vector<float> VB2_DATA = ReadObjFileType(L"Models3D/sphere.obj");




        GP::VertexArrayClass VA1;

        GP::VertexBufferClass VB1;
        VB1.SetData((unsigned int)(VB1_DATA.size()*sizeof(float)), &VB1_DATA[0], GP::VertexBufferClass::BufferDataUsageEnum::StaticDraw);
        VB1.SetLayout(DynArr<GP::VertexBufferClass::LayoutDataStruct>(
            GP::VertexBufferClass::LayoutDataStruct{ 3,GP::VertexBufferClass::BufferDataTypeEnum::Float },
            GP::VertexBufferClass::LayoutDataStruct{ 3,GP::VertexBufferClass::BufferDataTypeEnum::Float },
            GP::VertexBufferClass::LayoutDataStruct{ 3,GP::VertexBufferClass::BufferDataTypeEnum::Float },
            GP::VertexBufferClass::LayoutDataStruct{ 2,GP::VertexBufferClass::BufferDataTypeEnum::Float }
        ));

        VA1.Unbind();

        GP::VertexArrayClass VA2;
        
        GP::VertexBufferClass VB2;
        VB2.SetData((unsigned int)(VB2_DATA.size() * sizeof(float)), &VB2_DATA[0], GP::VertexBufferClass::BufferDataUsageEnum::StaticDraw);
		VB2.SetLayout(DynArr<GP::VertexBufferClass::LayoutDataStruct>(
            GP::VertexBufferClass::LayoutDataStruct{ 3,GP::VertexBufferClass::BufferDataTypeEnum::Float },
            GP::VertexBufferClass::LayoutDataStruct{ 3,GP::VertexBufferClass::BufferDataTypeEnum::Float },
            GP::VertexBufferClass::LayoutDataStruct{ 3,GP::VertexBufferClass::BufferDataTypeEnum::Float },
            GP::VertexBufferClass::LayoutDataStruct{ 2,GP::VertexBufferClass::BufferDataTypeEnum::Float }
        ));
        VA2.Unbind();

        std::vector<float> quadVBD({ 1,-1,1,1,-1,1,1,-1,-1,-1,-1,1 });

        GP::VertexArrayClass QUAD_VA;

        GP::VertexBufferClass QUAD_VB;
        QUAD_VB.SetData((unsigned int)(quadVBD.size()*sizeof(float)), &quadVBD[0], GP::VertexBufferClass::BufferDataUsageEnum::StaticDraw);
        QUAD_VB.SetLayout(DynArr<GP::VertexBufferClass::LayoutDataStruct>(
            GP::VertexBufferClass::LayoutDataStruct{ 2,GP::VertexBufferClass::BufferDataTypeEnum::Float }
        ));

        QUAD_VA.Unbind();

		GA::ShaderClass SP(L"Shaders/full3d.vs", L"Shaders/full3d.fs", nullptr, nullptr);
                
        GA::ShaderClass SP_OUTLINE(L"Shaders/outline.vs", L"Shaders/outline.fs", nullptr, nullptr);

        GA::ShaderClass SP_NORMAL(L"Shaders/normal.vs", L"Shaders/normal.gs", L"Shaders/normal.fs", nullptr, nullptr);

        GA::ShaderClass QUAD_SP(L"Shaders/quad.vs", L"Shaders/quad.fs", nullptr, nullptr);

        QUAD_SP.Bind();
        QUAD_SP.g_CFAC_UniformFuncs()(QUAD_SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform1i, QUAD_SP.GetUniformIDByName("u_Texture"), 0);

        SP.Bind();
        SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform1i,SP.GetUniformIDByName("u_tex1"), 0);
        SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform1i, SP.GetUniformIDByName("u_tex2"), 1);

        GA::TextureClass TEX0(GP::TextureClass::DimensionsEnum::Two, "Textures/blackFace.jpg",
            GP::TextureClass::SettingsStruct{ GP::TextureClass::SettingsStruct::WrapTypeEnum::Repeat,GP::TextureClass::SettingsStruct::WrapTypeEnum::Repeat,
            GP::TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Linear,GP::TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Linear,
            GP::TextureClass::SettingsStruct::DepthStencilReadModeEnum::Depth }, GP::TextureClass::DataSettingsStruct{GP::TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA,
            GP::TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGB,GP::TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedByte});
        GA::TextureClass TEX1(GP::TextureClass::DimensionsEnum::Two, "Textures/simpleFace.png",
            GP::TextureClass::SettingsStruct{ GP::TextureClass::SettingsStruct::WrapTypeEnum::Repeat,GP::TextureClass::SettingsStruct::WrapTypeEnum::Repeat,
            GP::TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Linear,GP::TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Linear,
            GP::TextureClass::SettingsStruct::DepthStencilReadModeEnum::Depth }, GP::TextureClass::DataSettingsStruct{GP::TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA,
            GP::TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGBA,GP::TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedByte});
        

        
        
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

        float TimeCounter = 0;
        float effectMult = 1;
        bool fullHD_Render = false;
		EventsHandler.ConnectToEvent(&window.gKeyboardHandle().gPressableKeyEvent(KeyboardClass::PressableKeysEnum::K), [&](void* pressedDown) {
            if (*(bool*)pressedDown) fullHD_Render = not fullHD_Render;
            });


        EventsHandler.ConnectToEvent(&window.gKeyboardHandle().gPressableKeyEvent(KeyboardClass::PressableKeysEnum::M), [&](void* pressedDown) {
            if (*(bool*)pressedDown) effectMult += 0.2f;
            });
		EventsHandler.ConnectToEvent(&window.gKeyboardHandle().gPressableKeyEvent(KeyboardClass::PressableKeysEnum::N), [&](void* pressedDown) {
            if (*(bool*)pressedDown) effectMult -= 0.2f;
            if (effectMult < 0) effectMult = 0;
            });

        unsigned int CurrentFPS_Counter = 0;
        unsigned int LastFPS_Counter = 0;


        while (!window.WindowWaitingToClose()) {
            
            CurrentFPS_Counter++;
            window.UpdateMouseData();

            float FrameStartTime = TimeNamespace::GetTime();
            
            Object2Scale = Vector3F(sinf(TimeCounter)+1);

            Vector2I MouseDelta;
            window.gCursorDelta(&MouseDelta);


            float cameraSpeed = 0.04f;


            {
				bool shiftEnabled = window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::LeftShift);
				bool altEnabled = window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::LeftAlt);
                Vector3F localCamVelocity;

                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::W)) localCamVelocity += Vector3F(0, 0, cameraSpeed);
                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::S)) localCamVelocity += Vector3F(0, 0, -cameraSpeed);
                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::A)) localCamVelocity += Vector3F(-cameraSpeed, 0, 0);
                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::D)) localCamVelocity += Vector3F(cameraSpeed, 0, 0);
                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::Q)) localCamVelocity += Vector3F(0, -cameraSpeed, 0);
                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::E)) localCamVelocity += Vector3F(0, cameraSpeed, 0);

                float speedMult = 1;
                if (shiftEnabled) speedMult = 10;
                else if (altEnabled) speedMult = 1000;
                CameraPosition += CameraRotationMatrix * (localCamVelocity * speedMult);
            }

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

            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP.GetUniformIDByName("u_LightPos"), 1, &LightPosition[0]);
            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP.GetUniformIDByName("u_CameraPosition"), 1, &CameraPosition[0]);
            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix3fv, SP.GetUniformIDByName("u_InversedCameraRotationMatrix"), 1, false, &InversedCameraRotationMatrix[0]);
            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix4fv, SP.GetUniformIDByName("u_ProjectionMatrix"), 1, false, &ProjectionMatrix[0]);
            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform1f, SP.GetUniformIDByName("u_VisualData.Shininess"), 4.f);
            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform1f, SP.GetUniformIDByName("u_VisualData.LightSourceReflectionMaxAngle"), 40.f / 180.f * 3.14f);
            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform1f, SP.GetUniformIDByName("u_VisualData.MinColorMultiplierForSurfaceLighting"), 0.05f);

            FB.Bind(true);

            Preset3D.Bind();
            FB.ClearAllBuffers();

            VA1.Bind();

            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP.GetUniformIDByName("u_ObjectPosition"), 1, &Object1Position[0]);
            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix3fv, SP.GetUniformIDByName("u_ObjectRotationMatrix"), 1, false, &Object1RotationMatrix[0]);
            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP.GetUniformIDByName("u_ObjectScale"), 1, &Object1Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

            VA2.Bind();

            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP.GetUniformIDByName("u_ObjectPosition"), 1, &Object2Position[0]);
            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix3fv, SP.GetUniformIDByName("u_ObjectRotationMatrix"), 1, false, &Object2RotationMatrix[0]);
            SP.g_CFAC_UniformFuncs()(SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP.GetUniformIDByName("u_ObjectScale"), 1, &Object2Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);

            Preset3D.sStencilTest_BaseMask(0);
            Preset3D.sStencilTest_ComparisonType(GP::RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::NotEqual);
            Preset3D.sDepthTest_Enabled(false);

            {//outline


                SP_OUTLINE.Bind();
                SP_OUTLINE.g_CFAC_UniformFuncs()(SP_OUTLINE.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP_OUTLINE.GetUniformIDByName("u_CameraPosition"), 1, &CameraPosition[0]);
                SP_OUTLINE.g_CFAC_UniformFuncs()(SP_OUTLINE.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix3fv, SP_OUTLINE.GetUniformIDByName("u_InversedCameraRotationMatrix"), 1, false, &InversedCameraRotationMatrix[0]);
                SP_OUTLINE.g_CFAC_UniformFuncs()(SP_OUTLINE.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix4fv, SP_OUTLINE.GetUniformIDByName("u_ProjectionMatrix"), 1, false, &ProjectionMatrix[0]);
                SP_OUTLINE.g_CFAC_UniformFuncs()(SP_OUTLINE.g_CFAC_UniformFuncs().FuncPtrs.SetUniform1f, SP_OUTLINE.GetUniformIDByName("u_OutlineScale"), 0.1f);
                
                VA1.Bind();

                SP_OUTLINE.g_CFAC_UniformFuncs()(SP_OUTLINE.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP_OUTLINE.GetUniformIDByName("u_ObjectPosition"), 1, &Object1Position[0]);
                SP_OUTLINE.g_CFAC_UniformFuncs()(SP_OUTLINE.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix3fv, SP_OUTLINE.GetUniformIDByName("u_ObjectRotationMatrix"), 1, false, &Object1RotationMatrix[0]);
                SP_OUTLINE.g_CFAC_UniformFuncs()(SP_OUTLINE.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP_OUTLINE.GetUniformIDByName("u_ObjectScale"), 1, &Object1Scale[0]);
                GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

                VA2.Bind();

                SP_OUTLINE.g_CFAC_UniformFuncs()(SP_OUTLINE.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP_OUTLINE.GetUniformIDByName("u_ObjectPosition"), 1, &Object2Position[0]);
                SP_OUTLINE.g_CFAC_UniformFuncs()(SP_OUTLINE.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix3fv, SP_OUTLINE.GetUniformIDByName("u_ObjectRotationMatrix"), 1, false, &Object2RotationMatrix[0]);
                SP_OUTLINE.g_CFAC_UniformFuncs()(SP_OUTLINE.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP_OUTLINE.GetUniformIDByName("u_ObjectScale"), 1, &Object2Scale[0]);
                GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);
            }

            Preset3D.sStencilTest_BaseMask(0xff);
            Preset3D.sStencilTest_ComparisonType(GP::RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::AlwaysPass);
            Preset3D.sDepthTest_Enabled(true);

            SP_NORMAL.Bind();
            SP_NORMAL.g_CFAC_UniformFuncs()(SP_NORMAL.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP_NORMAL.GetUniformIDByName("u_CameraPosition"), 1, &CameraPosition[0]);
            SP_NORMAL.g_CFAC_UniformFuncs()(SP_NORMAL.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix3fv, SP_NORMAL.GetUniformIDByName("u_InversedCameraRotationMatrix"), 1, false, &InversedCameraRotationMatrix[0]);
            SP_NORMAL.g_CFAC_UniformFuncs()(SP_NORMAL.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix4fv, SP_NORMAL.GetUniformIDByName("u_ProjectionMatrix"), 1, false, &ProjectionMatrix[0]);

            VA1.Bind();

            SP_NORMAL.g_CFAC_UniformFuncs()(SP_NORMAL.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP_NORMAL.GetUniformIDByName("u_ObjectPosition"), 1, &Object1Position[0]);
            SP_NORMAL.g_CFAC_UniformFuncs()(SP_NORMAL.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix3fv, SP_NORMAL.GetUniformIDByName("u_ObjectRotationMatrix"), 1, false, &Object1RotationMatrix[0]);
            SP_NORMAL.g_CFAC_UniformFuncs()(SP_NORMAL.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP_NORMAL.GetUniformIDByName("u_ObjectScale"), 1, &Object1Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB1_DATA.size() / floatsAmountPerVertex);

            VA2.Bind();

            SP_NORMAL.g_CFAC_UniformFuncs()(SP_NORMAL.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP_NORMAL.GetUniformIDByName("u_ObjectPosition"), 1, &Object2Position[0]);
            SP_NORMAL.g_CFAC_UniformFuncs()(SP_NORMAL.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix3fv, SP_NORMAL.GetUniformIDByName("u_ObjectRotationMatrix"), 1, false, &Object2RotationMatrix[0]);
            SP_NORMAL.g_CFAC_UniformFuncs()(SP_NORMAL.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, SP_NORMAL.GetUniformIDByName("u_ObjectScale"), 1, &Object2Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)VB2_DATA.size() / floatsAmountPerVertex);


            FB.Unbind();
            FB.SetViewportSize(Vector2U(Width, Height));

            QuadPreset.Bind();
            window.ClearColorBuffer();
            QUAD_SP.Bind();

            QUAD_SP.g_CFAC_UniformFuncs()(QUAD_SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform1i, QUAD_SP.GetUniformIDByName("u_Texture"), 0);
            QUAD_SP.g_CFAC_UniformFuncs()(QUAD_SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniformMatrix3fv, QUAD_SP.GetUniformIDByName("u_CameraRotation"), 1, false, &CameraRotationMatrix[0]);
            QUAD_SP.g_CFAC_UniformFuncs()(QUAD_SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform3fv, QUAD_SP.GetUniformIDByName("u_CameraPosition"), 1, &CameraPosition[0]);
            QUAD_SP.g_CFAC_UniformFuncs()(QUAD_SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform2fv, QUAD_SP.GetUniformIDByName("u_CameraResolutionSize"), 1, &ResolutionLength[0]);
            QUAD_SP.g_CFAC_UniformFuncs()(QUAD_SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform1f, QUAD_SP.GetUniformIDByName("u_EffectMult"), effectMult);// (sinf(time) + 1) / 2 * 10 + 5);
            QUAD_SP.g_CFAC_UniformFuncs()(QUAD_SP.g_CFAC_UniformFuncs().FuncPtrs.SetUniform1f, QUAD_SP.GetUniformIDByName("u_Time"), TimeCounter);

            FB_COLOR_TEX.Bind(0);

            QUAD_VA.Bind();

            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, (int)quadVBD.size() / 2);

            QUAD_VA.Unbind();

			//TEXT_RENDERER.RenderText(ArialFont, L"english ðóññêèé ÉÆ¨!:(|&", Vector2F(-1,0), Vector2F(-1,0), Vector2U(Width, Height), Vector2F(1, 0), 0.5f);
            
            std::wstring camPosText = std::to_wstring(CameraPosition[0]) + L',' + std::to_wstring(CameraPosition[1]) + L',' + std::to_wstring(CameraPosition[2]) + L"; FPS: " + std::to_wstring(LastFPS_Counter);
            TEXT_RENDERER.RenderText(ArialFont, camPosText.c_str(), Vector2F(1, 1), Vector2F(1, 1), Vector2U(Width, Height), Vector2F(0, 0.05f), 1);

            window.SwapScreenBuffers();
            window.ProcessEvents();

            unsigned int prevWholeTimeNumber = (unsigned int)TimeCounter;
            TimeCounter += TimeNamespace::GetTime() - FrameStartTime;
            if ((unsigned int)TimeCounter != prevWholeTimeNumber) {
                LastFPS_Counter = CurrentFPS_Counter;
                CurrentFPS_Counter = 0;
            }
            
            
        }

    }
    catch (KURSAVAYAENGINE2_CORE_ERRORS& err) { ProgramExitCode = (unsigned int)err; }

	UninitializeKE2();
    return ProgramExitCode;
}