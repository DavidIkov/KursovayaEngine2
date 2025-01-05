
#include<iostream>
#include"Tools/ErrorsSystem.h"
#include"KE2_Manager.h"
#include"Windows/Window.h"
#include"Graphics/Abstractions/TextRenderer.h"
#include"Graphics/Abstractions/Texture.h"
#include"Graphics/Abstractions/VertexBuffer.h"
#include"Graphics/Abstractions/VertexArray.h"
#include"Graphics/Abstractions/IndexBuffer.h"
#include"Graphics/Abstractions/RenderBuffer.h"
#include"Graphics/Abstractions/FrameBuffer.h"
#include"Graphics/Primitives/FrameBuffer.h"
#include"Graphics/Primitives/VertexArray.h"
#include"Graphics/Primitives/VertexBuffer.h"
#include"Graphics/Primitives/RenderBuffer.h"
#include"Graphics/Primitives/ShaderProgram.h"
#include"Graphics/Primitives/Texture.h"
#include"Graphics/Primitives/Renderer.h"
#include"Graphics/Primitives/IndexBuffer.h"
#include"Maths/Vector.h"
#include"Maths/Matrix.h"
#include"Tools/FileTypesReaders/Obj.h"
#include"Tools/Time.h"

namespace GP = KE2::Graphics::Primitives;
namespace GA = KE2::Graphics::Abstractions;

int main()
{

    
    try {

		KE2::Manager::KE2_LibraryStruct KE2_Library;

        KE2::Window::WindowClass::MonitorDataStruct MonitorData = KE2::Window::WindowClass::gPrimaryMonitorData();

        unsigned int Width = MonitorData.Size[0] / 2;
        unsigned int Height = MonitorData.Size[1] / 2;

        KE2::Window::WindowClass window(&Width, &Height, "haiiiii", false, -1);
        GA::TextRendererClass TEXT_RENDERER("Shaders/textNEW.vs", "Shaders/textNEW.fs");
        const GA::TextRendererClass::FontStruct& ArialFont = TEXT_RENDERER.AddFont(50, "Fonts/arial.ttf", L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"\
            "`abcdefghijklmnopqrstuvwxyz{|}~"\
            "ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßÿþýüûúùø÷öõôóòñðïîíìëêéèçæ¸åäãâáà");

        
        GA::FrameBufferClass FB(Vector2U(Width, Height));
        GA::TextureClass FB_COLOR_TEX(GP::TextureClass::DimensionsEnum::Two, Vector3U(Width, Height, 0), 0,
            GP::TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA,
            GP::TextureClass::SettingsStruct{ GP::TextureClass::SettingsStruct::WrapTypeEnum::ClampToEdge,GP::TextureClass::SettingsStruct::WrapTypeEnum::ClampToEdge,
            GP::TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Nearest,GP::TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Nearest,
            GP::TextureClass::SettingsStruct::DepthStencilReadModeEnum::None }
        );
        FB.AttachTexture(FB_COLOR_TEX, 0);
        /*TextureClass FB_DEPTH_STENCIL_TEX(Width, Height, nullptr, TextureClass::TypeEnum::Texture2D,
            GP::TextureClass::SettingsStruct{ GP::TextureClass::SettingsStruct::WrapTypeEnum::ClampToEdge,GP::TextureClass::SettingsStruct::WrapTypeEnum::ClampToEdge,
            GP::TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Nearest,GP::TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Nearest,
            GP::TextureClass::SettingsStruct::DepthStencilReadModeEnum::Stencil },
            TextureClass::DataSettingsClass{ TextureClass::DataSettingsClass::DataFormatOnGPU_Enum::DepthStencil,
            TextureClass::DataSettingsClass::DataFormatOnCPU_Enum::DepthStencil, TextureClass::DataSettingsClass::DataTypeInMemory_Enum::UnsignedInt_24_8 });
        FB.AttachTexture(FB_DEPTH_STENCIL_TEX.gID(), TextureClass::DataSettingsClass::DataFormatOnGPU_Enum::DepthStencil);*/
        GA::RenderBufferClass RB(Vector2U(Width, Height), GP::RenderBufferClass::BufferDataFormatEnum::DepthStencil);
        FB.AttachRenderBuffer(RB);

        FB.Finish();
        FB.Unbind();


        GP::RenderingPresetS Preset3D{
            GP::RenderingPresetS::ColorsS{ Vector4B(true),Vector4F(0.1f,0.2f,0.3f,0.f),ArrayView<unsigned>({ 1 })},
            GP::RenderingPresetS::BlendingS{true,Vector4F(0.f),GP::RenderingPresetS::BlendingS::BlendingFuncValE::Src0Alpha,
            GP::RenderingPresetS::BlendingS::BlendingFuncValE::OneMinusSrc0Alpha},
            GP::RenderingPresetS::FaceCullingS{true,GP::RenderingPresetS::FaceCullingS::FaceToCullE::Back,GP::RenderingPresetS::FaceCullingS::FaceDeterminationE::Clockwise},
            GP::RenderingPresetS::DepthTestS{true,true,GP::RenderingPresetS::DepthTestS::TypeOfComparisonE::LessOrEqual},
            GP::RenderingPresetS::StencilTestS{true,0xff,GP::RenderingPresetS::StencilTestS::ComparisonTypeE::AlwaysPass,1,0xff,
            GP::RenderingPresetS::StencilTestS::ActionsE::Keep,GP::RenderingPresetS::StencilTestS::ActionsE::Keep,GP::RenderingPresetS::StencilTestS::ActionsE::Replace},
        };
		GP::RenderingPresetS QuadPreset{ GP::RenderingPresetS::ColorsS{ Vector4B(true),Vector4F(0.f,0.f,0.f,1.f),ArrayView<unsigned>({ 1 })}, };
        
        GP::ShaderProgramClass Test_SP("Shaders/simpleQuad.vs", "Shaders/simpleQuad.fs");
        GA::VertexBufferClass Test_VB(GA::VertexBufferClass::BufferReadWriteModeEnum::StaticDraw, ArrayView<float>({ 0.25f,0.25f,-0.25f,0.25f,-0.25f,-0.25f,0.25f,-0.25f }));
        GA::VertexBufferClass Test_VB2(GA::VertexBufferClass::BufferReadWriteModeEnum::StaticDraw, ArrayView<float>({ 0.f,0.3f,0.6f,1.f }));
        GA::IndexBufferClass Test_IB(GA::IndexBufferClass::BufferReadWriteModeEnum::StaticDraw, ArrayView<unsigned int>({ 0u,1u,2u,2u,3u,0u }));
        Test_IB.Unbind();
		GA::VertexArrayClass Test_VA(ArrayView<GA::VertexArrayClass::AttributeDataStruct>({
            GA::VertexArrayClass::AttributeDataStruct{ Test_VB,false,0,2,0,sizeof(float) * 2, GA::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float, GA::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float },
            GA::VertexArrayClass::AttributeDataStruct{ Test_VB2,false,0,1,0,sizeof(float) * 1, GA::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float, GA::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float},
            }));

        GA::VertexBufferClass VB1(GA::VertexBufferClass::BufferReadWriteModeEnum::StaticDraw, "Models3D/koleso.obj");
		GA::VertexBufferClass VB2(GA::VertexBufferClass::BufferReadWriteModeEnum::StaticDraw, "Models3D/sphere.obj");

        GA::VertexArrayClass VA1(ArrayView<GA::VertexArrayClass::AttributeDataStruct>({
            GA::VertexArrayClass::AttributeDataStruct{ VB1,false,0,3,0,sizeof(float) * (3 + 3 + 3 + 2), GA::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float, GA::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float },
            GA::VertexArrayClass::AttributeDataStruct{ VB1,false,0,3,sizeof(float) * 3,sizeof(float) * (3 + 3 + 3 + 2), GA::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float, GA::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float},
            GA::VertexArrayClass::AttributeDataStruct{ VB1,false,0,3,sizeof(float) * (3 + 3),sizeof(float) * (3 + 3 + 3 + 2), GA::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float, GA::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float},
            GA::VertexArrayClass::AttributeDataStruct{ VB1,false,0,2,sizeof(float) * (3 + 3 + 3),sizeof(float) * (3 + 3 + 3 + 2), GA::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float, GA::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float},
            }));
		GA::VertexArrayClass VA2(ArrayView<GA::VertexArrayClass::AttributeDataStruct>({
            GA::VertexArrayClass::AttributeDataStruct{ VB2,false,0,3,0,sizeof(float) * (3 + 3 + 3 + 2), GA::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float, GA::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float },
            GA::VertexArrayClass::AttributeDataStruct{ VB2,false,0,3,sizeof(float) * 3,sizeof(float) * (3 + 3 + 3 + 2), GA::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float, GA::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float},
            GA::VertexArrayClass::AttributeDataStruct{ VB2,false,0,3,sizeof(float) * (3 + 3),sizeof(float) * (3 + 3 + 3 + 2), GA::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float, GA::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float},
            GA::VertexArrayClass::AttributeDataStruct{ VB2,false,0,2,sizeof(float) * (3 + 3 + 3),sizeof(float) * (3 + 3 + 3 + 2), GA::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float, GA::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float},
            }));

        unsigned int floatsAmountPerVertex = 3 + 3 + 3 + 2;// 0u; for (size_t i = 0; i < VA1.gAttributesData().gLen(); i++) floatsAmountPerVertex += VA1.gAttributeData(i).ComponentsAmount;

        GA::VertexBufferClass VB_QUAD(GA::VertexBufferClass::BufferReadWriteModeEnum::StaticDraw, ArrayView<float>({1,-1,1,1,-1,1,1,-1,-1,-1,-1,1}));
        GA::VertexArrayClass VA_QUAD(ArrayView<GA::VertexArrayClass::AttributeDataStruct>({
            GA::VertexArrayClass::AttributeDataStruct{ VB_QUAD,false,0,2,0,sizeof(float) * 2, GA::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float, GA::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float },
            }));


		GP::ShaderProgramClass SP("Shaders/full3d.vs", "Shaders/full3d.fs");
        
        GP::ShaderProgramClass  SP_OUTLINE("Shaders/outline.vs", "Shaders/outline.fs");

        GP::ShaderProgramClass SP_NORMAL("Shaders/normal.vs", "Shaders/normal.gs", "Shaders/normal.fs");

        GP::ShaderProgramClass QUAD_SP("Shaders/quad.vs", "Shaders/quad.fs");

        QUAD_SP.Bind();
        QUAD_SP.SetUniform1i("u_Texture", 0);

        SP.Bind();
        SP.SetUniform1i("u_tex1", 0);
        SP.SetUniform1i("u_tex2", 1);

        GA::TextureClass TEX0(GA::TextureClass::DimensionsEnum::Two, "Textures/blackFace.jpg", 2,
            GA::TextureClass::SettingsStruct{ GA::TextureClass::SettingsStruct::WrapTypeEnum::Repeat,GA::TextureClass::SettingsStruct::WrapTypeEnum::Repeat,
            GA::TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Linear,GA::TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Linear,
            GA::TextureClass::SettingsStruct::DepthStencilReadModeEnum::Depth }, GA::TextureClass::DataSettingsStruct{GA::TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA,
            GA::TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGB,GA::TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedByte});
        GA::TextureClass TEX1(GA::TextureClass::DimensionsEnum::Two, "Textures/simpleFace.png", 2,
            GA::TextureClass::SettingsStruct{ GA::TextureClass::SettingsStruct::WrapTypeEnum::Repeat,GA::TextureClass::SettingsStruct::WrapTypeEnum::Repeat,
            GA::TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Linear,GA::TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Linear,
            GA::TextureClass::SettingsStruct::DepthStencilReadModeEnum::Depth }, GA::TextureClass::DataSettingsStruct{GA::TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::RGBA,
            GA::TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::RGBA,GA::TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedByte});
        

        
        
        Vector3F Object1Position(0.f, 0.f, 2.f);
        Matrix33F Object1RotationMatrix = Mat3D_RotBaseF;
        Vector3F Object1Scale(0.5f, 0.5f, 0.5f);
        Vector3F Object2Position(2.f, 0.f, 3.f);
        Matrix33F Object2RotationMatrix = Mat3D_RotBaseF;
        Vector3F Object2Scale(2.f, 2.f, 2.f);

        Vector3F LightPosition(0.f, 0.f, 0.f);

        Vector3F CameraPosition(0.f, 0.f, 0.f);
        Matrix33F CameraRotationMatrix = Mat3D_RotBaseF;
        Matrix33F InversedCameraRotationMatrix = CameraRotationMatrix.GetInversedMatrix(CameraRotationMatrix.GetDeterminant());

        float CameraVerticalFov = 80.f / 180.f * 3.14f;

        float near = 0.1f;
        float far = 100;

        Vector2F ResolutionLength(tanf(CameraVerticalFov / 2) * Width / Height, tanf(CameraVerticalFov / 2));

        EventConnectionsHandlerClass EventsHandler;

        EventsHandler.ConnectToEvent(&window.gKeyboardHandle().gPressableKeyEvent(KeyboardClass::PressableKeysEnum::L), [&window](void* pressedDown) {
            if (*(bool*)pressedDown) {
                if (window.gCursorMode() == KE2::Window::WindowClass::CursorModes::Free) 
                    window.SetCursorMode(KE2::Window::WindowClass::CursorModes::LockedAndInvisible);
                else window.SetCursorMode(KE2::Window::WindowClass::CursorModes::Free);
            }
            
            });

        float TimeCounter = 0;
        float effectMult = 1;
        EventsHandler.ConnectToEvent(&window.gKeyboardHandle().gPressableKeyEvent(KeyboardClass::PressableKeysEnum::M), [&](void* pressedDown) {
            if (*(bool*)pressedDown) effectMult += 0.2f;
            });
		EventsHandler.ConnectToEvent(&window.gKeyboardHandle().gPressableKeyEvent(KeyboardClass::PressableKeysEnum::N), [&](void* pressedDown) {
            if (*(bool*)pressedDown) effectMult -= 0.2f;
            if (effectMult < 0) effectMult = 0;
            });

        unsigned int CurrentFPS_Counter = 0;
        unsigned int LastFPS_Counter = 0;

        float TargetFPS = 120.f;

        float previousFPS_Time = 1.f / TargetFPS;

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

                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::W)) localCamVelocity += Vector3F(0.f, 0.f, cameraSpeed);
                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::S)) localCamVelocity += Vector3F(0.f, 0.f, -cameraSpeed);
                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::A)) localCamVelocity += Vector3F(-cameraSpeed, 0.f, 0.f);
                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::D)) localCamVelocity += Vector3F(cameraSpeed, 0.f, 0.f);
                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::Q)) localCamVelocity += Vector3F(0.f, -cameraSpeed, 0.f);
                if (window.gKeyboardHandle().gPressableKeyState(KeyboardClass::PressableKeysEnum::E)) localCamVelocity += Vector3F(0.f, cameraSpeed, 0.f);

                float speedMult = 1;
                if (shiftEnabled) speedMult = 10;
                else if (altEnabled) speedMult = 1000;
                CameraPosition += CameraRotationMatrix * (localCamVelocity * speedMult);
            }

            {
                Vector2F CameraRotationByDelta = (Vector2F)MouseDelta / Vector2F((float)Width, (float)Height) / ResolutionLength;
                CameraRotationByDelta[0] = atanf(CameraRotationByDelta[0]); CameraRotationByDelta[1] = atanf(CameraRotationByDelta[1]);

                CameraRotationMatrix = CameraRotationMatrix.RotateIn3DByAnglesC<0, 1, 2>(CameraRotationByDelta[1], 0, 0);
                Vector3F xv(1.f, 0.f, 0.f);
                Vector3F zv(0.f, 0.f, 1.f);
                Matrix33F rotMat = Mat3D_RotBaseF;
                xv = rotMat.RotateVectorC<0, 2>(xv, -CameraRotationByDelta[0]);
                zv = rotMat.RotateVectorC<0, 2>(zv, -CameraRotationByDelta[0]);
                CameraRotationMatrix = CameraRotationMatrix * Matrix33F(xv[0], xv[1], xv[2], 0.f, 1.f, 0.f, zv[0], zv[1], zv[2]);


                InversedCameraRotationMatrix = CameraRotationMatrix.GetInversedMatrix(CameraRotationMatrix.GetDeterminant());
            }


            TEX0.Bind(0);
            TEX1.Bind(1);
            SP.Bind();

            Matrix44F ProjectionMatrix(1 / ResolutionLength[0], 0.f, 0.f, 0.f, 0.f, 1 / ResolutionLength[1], 0.f, 0.f, 0.f, 0.f, (far + near) / (far - near), 1.f, 0.f, 0.f, 2 * far * near / (near - far), 0.f);

            Object1RotationMatrix = Object1RotationMatrix.RotateIn3DByAnglesC<0, 1, 2>(0.01f, 0.01f, 0);

            SP.SetUniform3fv("u_LightPos", 1, &LightPosition[0]);
            SP.SetUniform3fv("u_CameraPosition", 1, &CameraPosition[0]);
            SP.SetUniformMatrix3fv("u_InversedCameraRotationMatrix", 1, false, &InversedCameraRotationMatrix[0]);
            SP.SetUniformMatrix4fv("u_ProjectionMatrix", 1, false, &ProjectionMatrix[0]);
            SP.SetUniform1f("u_VisualData.Shininess", 4.f);
            SP.SetUniform1f("u_VisualData.LightSourceReflectionMaxAngle", 40.f / 180.f * 3.14f);
            SP.SetUniform1f("u_VisualData.MinColorMultiplierForSurfaceLighting", 0.05f);

            FB.Bind();

            Preset3D.Update();
            FB.ClearAllBuffers();

            VA1.Bind();

            SP.SetUniform3fv("u_ObjectPosition", 1, &Object1Position[0]);
            SP.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object1RotationMatrix[0]);
            SP.SetUniform3fv("u_ObjectScale", 1, &Object1Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, VB1.gDataSizeInBytes() / sizeof(float) / floatsAmountPerVertex);

            VA2.Bind();

            SP.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
            SP.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
            SP.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, VB2.gDataSizeInBytes() / sizeof(float) / floatsAmountPerVertex);

            GP::RenderingPresetS::StencilTestS::UpdBaseMask(0);
            GP::RenderingPresetS::StencilTestS::UpdComparisonFunc(GP::RenderingPresetS::StencilTestS::ComparisonTypeE::NotEqual,Preset3D.StencilTest.ReferenceValue,Preset3D.StencilTest.Mask);
            GP::RenderingPresetS::DepthTestS::UpdEnabled(false);

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
                GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, VB1.gDataSizeInBytes() / sizeof(float) / floatsAmountPerVertex);

                VA2.Bind();

                SP_OUTLINE.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
                SP_OUTLINE.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
                SP_OUTLINE.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
                GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, VB2.gDataSizeInBytes() / sizeof(float) / floatsAmountPerVertex);

            }

            Preset3D.Update(Preset3D.StencilTest.Flags.BaseMask | Preset3D.StencilTest.Flags.ComparisonFunc | Preset3D.DepthTest.Flags.Enabled);

            SP_NORMAL.Bind();
            SP_NORMAL.SetUniform3fv("u_CameraPosition", 1, &CameraPosition[0]);
            SP_NORMAL.SetUniformMatrix3fv("u_InversedCameraRotationMatrix", 1, false, &InversedCameraRotationMatrix[0]);
            SP_NORMAL.SetUniformMatrix4fv("u_ProjectionMatrix", 1, false, &ProjectionMatrix[0]);

            VA1.Bind();

            SP_NORMAL.SetUniform3fv("u_ObjectPosition", 1, &Object1Position[0]);
            SP_NORMAL.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object1RotationMatrix[0]);
            SP_NORMAL.SetUniform3fv("u_ObjectScale", 1, &Object1Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, VB1.gDataSizeInBytes() / sizeof(float) / floatsAmountPerVertex);

            VA2.Bind();

            SP_NORMAL.SetUniform3fv("u_ObjectPosition", 1, &Object2Position[0]);
            SP_NORMAL.SetUniformMatrix3fv("u_ObjectRotationMatrix", 1, false, &Object2RotationMatrix[0]);
            SP_NORMAL.SetUniform3fv("u_ObjectScale", 1, &Object2Scale[0]);
            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, VB2.gDataSizeInBytes() / sizeof(float) / floatsAmountPerVertex);

			std::wstring camPosText = std::to_wstring(CameraPosition[0]) + L',' + std::to_wstring(CameraPosition[1]) + L',' + std::to_wstring(CameraPosition[2]) + L"; FPS: " + std::to_wstring(LastFPS_Counter);
            TEXT_RENDERER.RenderText(ArialFont, camPosText.c_str(), Vector2F(1.f, 1.f), Vector2F(1.f, 1.f), Vector2U(Width, Height), Vector2F(0.f, 0.05f), 1.f);

            FB.Unbind();

            QuadPreset.Update(~QuadPreset.Colors.Flags.ColorsOutputs);
            window.ClearColorBuffer();
            QUAD_SP.Bind();

            QUAD_SP.SetUniform1i("u_Texture", 0);
            QUAD_SP.SetUniformMatrix3fv("u_CameraRotation", 1, false, &CameraRotationMatrix[0]);
            QUAD_SP.SetUniform3fv("u_CameraPosition", 1, &CameraPosition[0]);
            QUAD_SP.SetUniform2fv("u_CameraResolutionSize", 1, &ResolutionLength[0]);
            QUAD_SP.SetUniform1f("u_EffectMult", effectMult);// (sinf(time) + 1) / 2 * 10 + 5);
            QUAD_SP.SetUniform1f("u_Time", TimeCounter);

            FB_COLOR_TEX.Bind(0);

            VA_QUAD.Bind();

            GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, 6);

            Test_SP.Bind();
            Test_VA.Bind();
            Test_IB.Bind();
            GP::RendererNamespace::DrawElements(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, 6, GP::RendererNamespace::IndexBufferDataTypeEnum::UnsignedInt);


			//TEXT_RENDERER.RenderText(ArialFont, L"english ðóññêèé ÉÆ¨!:(|&", Vector2F(-1,0), Vector2F(-1,0), Vector2U(Width, Height), Vector2F(1, 0), 0.5f);
            
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
    catch (KE2::ErrorsSystemNamespace::AnyError& err) { std::cout << "crashed with message {" << err.what() << "}" << std::endl; }
    catch (std::exception& exc) { std::cout << "crashed from stl exception with message {" << exc.what() << "}" << std::endl; }
    catch (...) { std::cout << "crashed with unknown message or source" << std::endl; }

    return 0;
}