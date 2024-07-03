#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image/stb_image.h>

#include<iostream>
#include"Tools/DebuggingTools.h"
#include"GraphicsPrimitives/Shader.h"
#include"GraphicsPrimitives/ShaderProgram.h"
#include"GraphicsPrimitives/Texture.h"
#include"Tools/GLDebug.h"
#include"Maths/Matrix.h"
#include"Maths/Vector3.h"
#include"Maths/Vector2.h"
#include<vector>
#include"Windows/Window.h"
#include"Tools/ErrorCodes.h"
#include"GraphicsPrimitives/FrameBuffer.h"
#include"GraphicsPrimitives/RenderBuffer.h"
#include"GraphicsPrimitives/RenderingPreset.h"
#include"Tools/ReadFromFile.h"
#include"Tools/FileTypesReaders/Obj.h"

#include"Windows/WindowsManager.h"

int main()
{
    
    stbi_set_flip_vertically_on_load(true);

    if (!glfwInit()) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO INITIALIZE GLFW", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_INITIALIZE_GLFW });
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    try {

        


        unsigned int Width = 900;
        unsigned int Height = 600;


        Window window(Width, Height, "haiiiii", false, 1);

        glSC(glViewport(0, 0, Width, Height));

        FrameBuffer FB(Width, Height);
        RenderBuffer RB(Width, Height, true, false);
        FB.AttachRenderBuffer(RB);
        FB.Finish();

        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FRAMEBUFFER IS NOT COMPLETE", KURSAVAYAENGINE2_CORE_ERRORS::FRAMEBUFFER_IS_NOT_COMPLETE });
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        RenderingPreset Preset3D(
            true, RenderingPresetEnumArguments::FaceCulling::FaceToCull::Back, RenderingPresetEnumArguments::FaceCulling::FaceDetermination::Clockwise,
            true, true, RenderingPresetEnumArguments::DepthTest::TypeOfComprasion::LessOrEqual,
            true, 0,0,0,0, RenderingPresetEnumArguments::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusSrcAlpha
        );
        RenderingPreset QuadPreset(
            false, RenderingPresetEnumArguments::FaceCulling::FaceToCull::Back, RenderingPresetEnumArguments::FaceCulling::FaceDetermination::Clockwise,
            false, true, RenderingPresetEnumArguments::DepthTest::TypeOfComprasion::LessOrEqual,
            false, 0, 0, 0, 0, RenderingPresetEnumArguments::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusSrcAlpha
        );

        unsigned int floatsAmountPerVertex = 3 + 2 + 3;
        std::vector<float> vertexBufferData = ReadObjFileType("Models3D/troll.obj");



        unsigned int VA_ID;
        glSC(glGenVertexArrays(1, &VA_ID));
        glSC(glBindVertexArray(VA_ID));

        unsigned int VB_ID;
        glSC(glGenBuffers(1, &VB_ID));
        glSC(glBindBuffer(GL_ARRAY_BUFFER, VB_ID));
        glSC(glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size() * sizeof(float), &(vertexBufferData[0]), GL_STATIC_DRAW));

        glSC(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floatsAmountPerVertex * sizeof(float), (void*)0));
        glSC(glEnableVertexAttribArray(0));
        glSC(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floatsAmountPerVertex * sizeof(float), (void*)(3 * sizeof(float))));
        glSC(glEnableVertexAttribArray(1));
        glSC(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, floatsAmountPerVertex * sizeof(float), (void*)(6 * sizeof(float))));
        glSC(glEnableVertexAttribArray(2));

        glSC(glBindVertexArray(0));

        std::vector<float> quadVBD({ 1,-1,1,1,-1,1,1,-1,-1,-1,-1,1 });

        unsigned int QUAD_VA_ID;
        glSC(glGenVertexArrays(1, &QUAD_VA_ID));
        glSC(glBindVertexArray(QUAD_VA_ID));

        unsigned int QUAD_VB_ID;
        glSC(glGenBuffers(1, &QUAD_VB_ID));
        glSC(glBindBuffer(GL_ARRAY_BUFFER, QUAD_VB_ID));
        glSC(glBufferData(GL_ARRAY_BUFFER, quadVBD.size() * sizeof(float), &(quadVBD[0]), GL_STATIC_DRAW));

        glSC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
        glSC(glEnableVertexAttribArray(0));

        glSC(glBindVertexArray(0));

        ShaderProgram SP;
        {
            Shader VS(ShaderTypesEnum::Vertex, "Shaders/test.vs");
            VS.Compile();
            SP.AttachShader(VS);

            Shader FS(ShaderTypesEnum::Fragment, "Shaders/test.fs");
            FS.Compile();
            SP.AttachShader(FS);

            SP.LinkShaders();
        }
        
        
        ShaderProgram QUAD_SP;
        {
            Shader VS(ShaderTypesEnum::Vertex, "Shaders/quad.vs");
            VS.Compile();
            QUAD_SP.AttachShader(VS);

            Shader FS(ShaderTypesEnum::Fragment, "Shaders/quad.fs");
            FS.Compile();
            QUAD_SP.AttachShader(FS);

            QUAD_SP.LinkShaders();
        }

        SP.Bind();
        glSC(glUniform1i(glGetUniformLocation(SP.gID(), "u_tex1"), 0));
        glSC(glUniform1i(glGetUniformLocation(SP.gID(), "u_tex2"), 1));

        Texture TEX0("Textures/blackFace.jpg");
        Texture TEX1("Textures/simpleFace.png");
        
        Vector3 Object1Position(0, 0, 3);
        Matrix Object1RotationMatrix(3, 3, { 1,0,0,0,1,0,0,0,1 });
        Vector3 Object1Scale(20,20,20);
        Vector3 Object2Position(2, 0, 3);
        Matrix Object2RotationMatrix(3, 3, { 1,0,0,0,1,0,0,0,1 });
        Vector3 Object2Scale(10,10,10);

        Vector3 LightPosition(0, 0, 0);

        Vector3 CameraPosition(0, 0, 0);
        Matrix CameraRotationMatrix(3, 3, { 1,0,0,0,1,0,0,0,1 });
        Matrix InversedCameraRotationMatrix = CameraRotationMatrix.gInversedMatrix();

        float CameraVerticalFov = 80.f / 180.f * 3.14f;

        float near = 0.1f;
        float far = 100;

        Vector2 ResolutionLength(tanf(CameraVerticalFov / 2) * Width / Height, tanf(CameraVerticalFov / 2));

        window._Keyboard.gPressableKeyEvent(PressableKeys::L).Connect([&window](bool pressedDown) {
            if (pressedDown) {
                if (window.gCursorMode() == CursorModes::Free) window.SetCursorMode(CursorModes::LockedAndInvisible);
                else window.SetCursorMode(CursorModes::Free);
            }
            
            });

        while (!window.WindowWaitingToClose())
        {
            window.StartUpdatingWindow();

            

            Vector2 MouseDelta;
            window.gCursorDelta(&MouseDelta);


            float cameraSpeed = 0.1f;
            if (window._Keyboard.gPressableKeyState(PressableKeys::W)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(0, 0, cameraSpeed);
            if (window._Keyboard.gPressableKeyState(PressableKeys::S)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(0, 0, -cameraSpeed);
            if (window._Keyboard.gPressableKeyState(PressableKeys::A)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(-cameraSpeed, 0, 0);
            if (window._Keyboard.gPressableKeyState(PressableKeys::D)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(cameraSpeed, 0, 0);
            if (window._Keyboard.gPressableKeyState(PressableKeys::Q)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(0, -cameraSpeed, 0);
            if (window._Keyboard.gPressableKeyState(PressableKeys::E)) CameraPosition = CameraPosition + CameraRotationMatrix * Vector3(0, cameraSpeed, 0);


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

            glSC(glUniform3fv(glGetUniformLocation(SP.gID(), "u_LightPos"), 1, &LightPosition[0]));
            glSC(glUniform3fv(glGetUniformLocation(SP.gID(), "u_CameraPosition"), 1, &CameraPosition[0]));
            glSC(glUniformMatrix3fv(glGetUniformLocation(SP.gID(), "u_InversedCameraRotationMatrix"), 1, GL_FALSE, &InversedCameraRotationMatrix[0]));
            glSC(glUniformMatrix4fv(glGetUniformLocation(SP.gID(), "u_ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0]));
            glSC(glUniform1f(glGetUniformLocation(SP.gID(), "u_VisualData.Shininess"), 4.f));
            glSC(glUniform1f(glGetUniformLocation(SP.gID(), "u_VisualData.LightSourceReflectionMaxAngle"), 40.f/180.f*3.14f));
            glSC(glUniform1f(glGetUniformLocation(SP.gID(), "u_VisualData.MinColorMultiplierForSurfaceLighting"), 0.05f));

            FB.Bind();

            Preset3D.Bind();
            glSC(glClearColor(0.f, 0.5f, 0.2f, 1.f));

            glSC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            glSC(glBindVertexArray(VA_ID));


            glSC(glUniform3fv(glGetUniformLocation(SP.gID(), "u_ObjectPosition"), 1, &Object1Position[0]));
            glSC(glUniformMatrix3fv(glGetUniformLocation(SP.gID(), "u_ObjectRotationMatrix"), 1, GL_FALSE, &Object1RotationMatrix[0]));
            glSC(glUniform3fv(glGetUniformLocation(SP.gID(), "u_ObjectScale"), 1, &Object1Scale[0]));
            glSC(glDrawArrays(GL_TRIANGLES, 0, (int)vertexBufferData.size() / floatsAmountPerVertex));

            glSC(glUniform3fv(glGetUniformLocation(SP.gID(), "u_ObjectPosition"), 1, &Object2Position[0]));
            glSC(glUniformMatrix3fv(glGetUniformLocation(SP.gID(), "u_ObjectRotationMatrix"), 1, GL_FALSE, &Object2RotationMatrix[0]));
            glSC(glUniform3fv(glGetUniformLocation(SP.gID(), "u_ObjectScale"), 1, &Object2Scale[0]));
            glSC(glDrawArrays(GL_TRIANGLES, 0, (int)vertexBufferData.size() / floatsAmountPerVertex));

            glSC(glBindVertexArray(0));

            FB.Unbind();
            QuadPreset.Bind();
            glSC(glClearColor(0.f, 0.f, 0.f, 1.f));

            glSC(glClear(GL_COLOR_BUFFER_BIT));
            QUAD_SP.Bind();

            glSC(glUniform1i(glGetUniformLocation(QUAD_SP.gID(), "u_Texture"), 0));

            FB.gTexture().Bind(0);

            glSC(glBindVertexArray(QUAD_VA_ID));
            
            glSC(glDrawArrays(GL_TRIANGLES, 0, (int)quadVBD.size() / 2));

            glSC(glBindVertexArray(0));

            window.EndUpdatingWindow();
        }

        glfwTerminate();
        return 0;
    }
    catch (KURSAVAYAENGINE2_CORE_ERRORS&) {
        glfwTerminate();
        return 0;
    }
}