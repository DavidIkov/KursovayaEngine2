
#include<string>
#include"Quad.h"
#include"ClickTrigger.h"
#include"GraphicsPrimitives/FrameBuffer.h"
#include"GraphicsPrimitives/VertexBuffer.h"
#include"GraphicsPrimitives/VertexBuffer.h"
#include"RussianConsole.h"
#include"KursovayaEngine2Manager.h"
#include"GraphicsPrimitives/RenderingPreset.h"
#include"GraphicsAbstractions/TextRenderer.h"
#include"csv_json.h"
#include"WinOS/FileExplorerDialog.h"
#include"WinOS/wchar_t_Operations.h"
#include"GraphicsPrimitives/Renderer.h"
#include"Tools/DebuggingTools.h"
#include<iostream>
#include"Tools/RandomNumber.h"


#define MAX_DATES_IN_GRAPH 200

namespace Proportions {
    float TopBarSize = 0.1f;
    float TopBarLeftTextSize = 0.2f;
    float LeftBarSize = 0.2f;

    float GraphicsSettingsBarSize = 0.2f;
    float GraphicSettingsSize = 0.1f;

    //from size of GraphicSettingsSize
    float GraphicSettingsDeviceNameSize = 0.225f;
    float GraphicSettingsDataNameSize = 0.225f;
    float GraphicSettingsDateSettingsSize = 0.45f;
    
    
};

std::wstring CurrentFileName;

//bad design since each time we add setting to vector a lot of stuff getting deleted/recreated but whatever
struct GraphicSettingsStruct {

    unsigned int DeviceInd;
    QuadsHandler::Quad DeviceNameQuad;
    ClickTrigger DeviceNameTrigger;

    unsigned int DataNameInd;
    QuadsHandler::Quad DataNameQuad;
    ClickTrigger DataNameTrigger;

    unsigned int Date1Ind;
    QuadsHandler::Quad Date1YearQuad;
    ClickTrigger Date1YearTrigger;
    QuadsHandler::Quad Date1MonthQuad;
    ClickTrigger Date1MonthTrigger;
    QuadsHandler::Quad Date1DayQuad;
    ClickTrigger Date1DayTrigger;
    QuadsHandler::Quad Date1HourQuad;
    ClickTrigger Date1HourTrigger;
    QuadsHandler::Quad Date1MinuteQuad;
    ClickTrigger Date1MinuteTrigger;
    QuadsHandler::Quad Date1SecondQuad;
    ClickTrigger Date1SecondTrigger;

    unsigned int Date2Ind;
    QuadsHandler::Quad Date2YearQuad;
    ClickTrigger Date2YearTrigger;
    QuadsHandler::Quad Date2MonthQuad;
    ClickTrigger Date2MonthTrigger;
    QuadsHandler::Quad Date2DayQuad;
    ClickTrigger Date2DayTrigger;
    QuadsHandler::Quad Date2HourQuad;
    ClickTrigger Date2HourTrigger;
    QuadsHandler::Quad Date2MinuteQuad;
    ClickTrigger Date2MinuteTrigger;
    QuadsHandler::Quad Date2SecondQuad;
    ClickTrigger Date2SecondTrigger;


    QuadsHandler::Quad GraphStaterQuad;

    QuadsHandler::Quad GraphColorQuad;
    ClickTrigger GraphColorTrigger;

    QuadsHandler::Quad GraphSettingDeleteQuad;
    ClickTrigger GraphSettingDeleteTrigger;


    bool RequiresGraphUpdate;
    VertexArray VA_TEX;
    VertexBuffer VB_TEX;
    VertexArray VA_GRAPH;
    VertexBuffer VB_GRAPH;
    Texture FB_TEX;
    FrameBuffer FB;
};
std::vector<GraphicSettingsStruct*> GraphicsSettings;
float GraphicsSettingsOffset = 0;
void ClearGraphicsSettings() {
    for (unsigned int i = 0; i < GraphicsSettings.size(); i++) delete GraphicsSettings[i];
    GraphicsSettings.clear();
}


int main()
{

    InitializeKursovayaEngine2();

    EnableRussianConsole();

    try {

        EventConnectionsHandlerClass EventConsHandler;

        unsigned int Width = 900;
        unsigned int Height = 600;


        Window window(Width, Height, "kursavayaengine2 window...", false, 10);


        TextRenderer TEXT_RENDERER("arial.ttf", "Shaders/text.vs", "Shaders/text.fs");

        RenderingPreset Preset2D(
            false, RenderingPresetEnumArguments::FaceCulling::FaceToCull::Back, RenderingPresetEnumArguments::FaceCulling::FaceDetermination::Clockwise,
            false, true, RenderingPresetEnumArguments::DepthTest::TypeOfComparison::LessOrEqual,
            false, 0xff, RenderingPresetEnumArguments::StencilTest::TypeOfComparison::AlwaysPass, 1, 0xff, RenderingPresetEnumArguments::StencilTest::Actions::Keep,
            RenderingPresetEnumArguments::StencilTest::Actions::Keep, RenderingPresetEnumArguments::StencilTest::Actions::Replace,
            true, 0, 0, 0, 0, RenderingPresetEnumArguments::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.1f, 0.2f, 0.3f
        );
        RenderingPreset PresetGraphFB(
            false, RenderingPresetEnumArguments::FaceCulling::FaceToCull::Back, RenderingPresetEnumArguments::FaceCulling::FaceDetermination::Clockwise,
            false, true, RenderingPresetEnumArguments::DepthTest::TypeOfComparison::LessOrEqual,
            false, 0xff, RenderingPresetEnumArguments::StencilTest::TypeOfComparison::AlwaysPass, 1, 0xff, RenderingPresetEnumArguments::StencilTest::Actions::Keep,
            RenderingPresetEnumArguments::StencilTest::Actions::Keep, RenderingPresetEnumArguments::StencilTest::Actions::Replace,
            false, 0, 0, 0, 0, RenderingPresetEnumArguments::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.1f, 0.2f, 0.3f
        );

        ShaderProgram GraphicsSP;
        {
            Shader VS("Shaders/graphic.vs", ShaderTypesEnum::Vertex);
            VS.Compile();
            GraphicsSP.AttachShader(VS);

            Shader FS("Shaders/graphic.fs", ShaderTypesEnum::Fragment);
            FS.Compile();
            GraphicsSP.AttachShader(FS);

            GraphicsSP.LinkShaders();
        }
        ShaderProgram SimpleImageSP;
        {
            Shader VS("Shaders/simpleImage.vs", ShaderTypesEnum::Vertex);
            VS.Compile();
            SimpleImageSP.AttachShader(VS);

            Shader FS("Shaders/Quad2dImage.fs", ShaderTypesEnum::Fragment);
            FS.Compile();
            SimpleImageSP.AttachShader(FS);

            SimpleImageSP.LinkShaders();
        }
        
        QuadsHandler QH("Shaders/Quad2d.vs", "Shaders/Quad2d.fs");

        QuadsHandler::Quad TopQuad(QH);
        TopQuad.Position = Vector<2>(0, 1);
        TopQuad.LocalOffset = Vector<2>(0, 1);
        TopQuad.Size = Vector<2>(1, Proportions::TopBarSize);
        TopQuad.Color = Vector<3>(0.5f,0.1f,1.f);

        ClickTrigger TopTrigger(&window);
        TopTrigger.Position = Vector<2>(1, 1);
        TopTrigger.LocalOffset = Vector<2>(1, 1);
        TopTrigger.Size = Vector<2>(1 - Proportions::TopBarLeftTextSize, Proportions::TopBarSize);

        QuadsHandler::Quad LeftQuad(QH);
        LeftQuad.Position = Vector<2>(-1, 1 - 2 * Proportions::TopBarSize);
        LeftQuad.LocalOffset = Vector<2>(-1, 1);
        LeftQuad.Size = Vector<2>(Proportions::LeftBarSize, 1 - Proportions::TopBarSize);
        LeftQuad.Color = Vector<3>(0.2f, 1.f, 0.7f);

        QuadsHandler QIH("Shaders/Quad2dImage.vs", "Shaders/Quad2dImage.fs");

        QuadsHandler::ImageQuad AddGraphSettingPlus(QIH, Texture{ "plus.png",Texture::TexParameters{
            TextureWrapType::ClampToEdge,TextureWrapType::ClampToEdge,TextureDownscalingFilterFunc::Linear,TextureUpscalingFilterFunc::Linear,TextureDepthStencilReadMode::Depth} });
        AddGraphSettingPlus.TexClampingType = QuadsHandler::ImageQuad::TextureClampingType::SquareByY;
        AddGraphSettingPlus.Size = Vector<2>(Proportions::GraphicsSettingsBarSize, Proportions::GraphicSettingsSize);
        AddGraphSettingPlus.LocalOffset = Vector<2>(0, 1);

        ClickTrigger GraphSettingPlusTrigger(&window);
        GraphSettingPlusTrigger.Size = Vector<2>(Proportions::GraphicsSettingsBarSize, Proportions::GraphicSettingsSize);
        GraphSettingPlusTrigger.LocalOffset = Vector<2>(-1, 1);

        std::function<void()> redraw([&]() {


            Preset2D.Bind();

            window.ClearAllBuffers();

            TopQuad.Draw();

            TEXT_RENDERER.DrawText(L"File name:",
                Width, Height,
                1,
                -1, 1 - Proportions::TopBarSize,
                -1, 0,
                Proportions::TopBarLeftTextSize, TextRenderer::ClampingFuncs::AlwaysScaleTillClamp,
                Proportions::TopBarSize, false, TextRenderer::ClampingFuncs::AlwaysScaleTillClamp);

            TEXT_RENDERER.DrawText(CurrentFileName,
                Width, Height,
                1,
                -1 + Proportions::TopBarLeftTextSize * 2, 1 - Proportions::TopBarSize,
                -1, 0,
                1 - Proportions::TopBarLeftTextSize, TextRenderer::ClampingFuncs::AlwaysScaleTillClamp,
                Proportions::TopBarSize, false, TextRenderer::ClampingFuncs::AlwaysScaleTillClamp);



            LeftQuad.Draw();

            
            AddGraphSettingPlus.Draw(Width, Height);

            for (unsigned int gsi = 0; gsi < GraphicsSettings.size(); gsi++) {

                GraphicSettingsStruct* sets = GraphicsSettings[gsi];

                auto& devNamQ = sets->DeviceNameQuad;
                devNamQ.Draw();
                TEXT_RENDERER.DrawText(csv_json::Devices[sets->DeviceInd].Name,
                    Width, Height,
                    1,
                    devNamQ.Position[0], devNamQ.Position[1]- devNamQ.Size[1],
                    devNamQ.LocalOffset[0], 0,
                    devNamQ.Size[0], TextRenderer::ClampingFuncs::AlwaysScaleTillClamp,
                    devNamQ.Size[1], false, TextRenderer::ClampingFuncs::AlwaysScaleTillClamp);


                auto& dataNamQ = sets->DataNameQuad;
                dataNamQ.Draw();
                std::wstring wDataName; char_to_wchar(1251, csv_json::Devices[sets->DeviceInd].DataNames[sets->DataNameInd].DataName, wDataName);
                TEXT_RENDERER.DrawText(wDataName,
                    Width, Height,
                    1,
                    dataNamQ.Position[0], dataNamQ.Position[1] - dataNamQ.Size[1],
                    dataNamQ.LocalOffset[0], 0,
                    dataNamQ.Size[0], TextRenderer::ClampingFuncs::AlwaysScaleTillClamp,
                    dataNamQ.Size[1], false, TextRenderer::ClampingFuncs::AlwaysScaleTillClamp);
                
#define dateDrawMacro(num,metric) {\
                auto& dateQRef = sets->Date##num##metric##Quad;\
                dateQRef.Draw();\
                std::wstring wideDate; std::string strDate = std::to_string(csv_json::Devices[sets->DeviceInd].Dates[sets->Date##num##Ind].Date.metric); char_to_wchar(1251, strDate, wideDate);\
                TEXT_RENDERER.DrawText(wideDate,\
                    Width, Height,\
                    1,\
                    dateQRef.Position[0] - dateQRef.LocalOffset[0] * dateQRef.Size[0], dateQRef.Position[1] - dateQRef.Size[1],\
                    0, 0,\
                    dateQRef.Size[0], TextRenderer::ClampingFuncs::AlwaysScaleTillClamp,\
                    dateQRef.Size[1], false, TextRenderer::ClampingFuncs::AlwaysScaleTillClamp);\
                }
                dateDrawMacro(1, Year);
                dateDrawMacro(2, Year);
                dateDrawMacro(1, Month);
                dateDrawMacro(2, Month);
                dateDrawMacro(1, Day);
                dateDrawMacro(2, Day);
                dateDrawMacro(1, Hour);
                dateDrawMacro(2, Hour);
                dateDrawMacro(1, Minute);
                dateDrawMacro(2, Minute);
                dateDrawMacro(1, Second);
                dateDrawMacro(2, Second);

                sets->GraphStaterQuad.Color = csv_json::Devices[sets->DeviceInd].DataNames[sets->DataNameInd].IsNumber ? Vector<3>(0, 1, 0) : Vector<3>(1, 0, 0);
                sets->GraphStaterQuad.Draw();

                sets->GraphStaterQuad.Draw();
                sets->GraphColorQuad.Draw();
                sets->GraphSettingDeleteQuad.Draw();

                float maxValue = -FLT_MAX;
                float minValue = FLT_MAX;
                if (csv_json::Devices[sets->DeviceInd].DataNames[sets->DataNameInd].IsNumber and sets->RequiresGraphUpdate) {

                    PresetGraphFB.Bind();
                    sets->RequiresGraphUpdate = false;
                    for (unsigned int di = sets->Date1Ind; di < (sets->Date2Ind + 1); di++) {
                        maxValue = fmaxf(maxValue, csv_json::Devices[sets->DeviceInd].Dates[di].Data[sets->DataNameInd]);
                        minValue = fminf(minValue, csv_json::Devices[sets->DeviceInd].Dates[di].Data[sets->DataNameInd]);
                    }


                    GraphicsSP.Bind();
                    unsigned int datesAmount = sets->Date2Ind - sets->Date1Ind + 1;
                    GraphicsSP.SetUniform1i("GraphDataLength", datesAmount);
                    GraphicsSP.SetUniform3f("u_LineColor", sets->GraphColorQuad.Color[0], sets->GraphColorQuad.Color[1], sets->GraphColorQuad.Color[2]);
                    GraphicsSP.SetUniform1f("u_PixelSizeX", 1.f / (float)Width / (1.f - Proportions::GraphicsSettingsBarSize));
                    
                    long long unsigned int datesDiff = (csv_json::Devices[sets->DeviceInd].Dates[sets->Date2Ind].Date - csv_json::Devices[sets->DeviceInd].Dates[sets->Date1Ind].Date).GetUniversalTime();
                    
                    for (unsigned int i = 0; i < datesAmount; i++) {
                        std::string unifName = "GraphData[";
                        unifName += std::to_string(i);
                        unifName += "].";

                        std::string tStr = unifName + "t";
                        GraphicsSP.SetUniform1f(tStr.c_str(),
                            (float)(csv_json::Devices[sets->DeviceInd].Dates[sets->Date1Ind + i].Date - csv_json::Devices[sets->DeviceInd].Dates[sets->Date1Ind].Date).GetUniversalTime() / datesDiff);

                        float val = csv_json::Devices[sets->DeviceInd].Dates[sets->Date1Ind + i].Data[sets->DataNameInd];
                        val = (val - minValue) / (maxValue - minValue);

                        std::string vStr = unifName + "v";
                        GraphicsSP.SetUniform1f(vStr.c_str(), val);
                    }

                    sets->FB.ClearColorBuffer();
                    sets->VA_GRAPH.Bind();
                    Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, 6);
                    sets->VA_GRAPH.Unbind();
                    sets->FB.Unbind(Width, Height);
                }

                Preset2D.Bind();
                sets->VA_TEX.Bind();
                SimpleImageSP.Bind();
                sets->FB_TEX.Bind(0);
                Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, 6);
                sets->VA_TEX.Unbind();
            }

#undef dateDrawMacro

            });

        std::function<void()> UpdGraphicsAndSettingsView([&]() {

            AddGraphSettingPlus.Position = Vector<2>(-1 + Proportions::GraphicsSettingsBarSize, 1 - 2 * (Proportions::TopBarSize + GraphicsSettings.size() * Proportions::GraphicSettingsSize));
            GraphSettingPlusTrigger.Position = Vector<2>(-1, 1 - 2 * (Proportions::TopBarSize + GraphicsSettings.size() * Proportions::GraphicSettingsSize));

            for (unsigned int gsi = 0; gsi < GraphicsSettings.size(); gsi++) {
                GraphicSettingsStruct* settings = GraphicsSettings[gsi];

                float curY = Proportions::TopBarSize + gsi * Proportions::GraphicSettingsSize;

                settings->DeviceNameQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->DeviceNameTrigger.Position = settings->DeviceNameQuad.Position;

                curY += settings->DeviceNameQuad.Size[1];

                settings->DataNameQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->DataNameTrigger.Position = settings->DataNameQuad.Position;

                curY += settings->DataNameQuad.Size[1];

                settings->Date1YearQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date1YearTrigger.Position = settings->Date1YearQuad.Position;
                settings->Date1MonthQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date1MonthTrigger.Position = settings->Date1MonthQuad.Position;
                settings->Date1DayQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date1DayTrigger.Position = settings->Date1DayQuad.Position;
                settings->Date1HourQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date1HourTrigger.Position = settings->Date1HourQuad.Position;
                settings->Date1MinuteQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date1MinuteTrigger.Position = settings->Date1MinuteQuad.Position;
                settings->Date1SecondQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date1SecondTrigger.Position = settings->Date1SecondQuad.Position;

                curY += settings->Date1YearQuad.Size[1];

                settings->Date2YearQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date2YearTrigger.Position = settings->Date2YearQuad.Position;
                settings->Date2MonthQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date2MonthTrigger.Position = settings->Date2MonthQuad.Position;
                settings->Date2DayQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date2DayTrigger.Position = settings->Date2DayQuad.Position;
                settings->Date2HourQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date2HourTrigger.Position = settings->Date2HourQuad.Position;
                settings->Date2MinuteQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date2MinuteTrigger.Position = settings->Date2MinuteQuad.Position;
                settings->Date2SecondQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->Date2SecondTrigger.Position = settings->Date2SecondQuad.Position;
                
                curY += settings->Date2YearQuad.Size[1];

                settings->GraphStaterQuad.Position = Vector<2>(-1, 1 - 2 * curY);

                settings->GraphColorQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->GraphColorTrigger.Position = settings->GraphColorQuad.Position;

                settings->GraphSettingDeleteQuad.Position = Vector<2>(-1, 1 - 2 * curY);
                settings->GraphSettingDeleteTrigger.Position = settings->GraphSettingDeleteQuad.Position;
                
            }

            redraw();
            window.SwapScreenBuffers();

            });

        EventConsHandler.ConnectToEvent(&TopTrigger.gLeftMouseButtonEvent(), [&](void* pressedDown) {
            if (*(bool*)pressedDown) {
                std::cout << "памятка то что кодировка файла должна быть ANCI!!!! иначе слетят русские буквы" << std::endl;
                DebuggingTools::IgnoreWarns = true;
                std::wstring filePath = FileExplorerDialog::OpenDialog({ {L"csv file",L"csv"},{L"json file", L"json"} });
                DebuggingTools::IgnoreWarns = false;

                if (not filePath.empty()) {

                    ClearGraphicsSettings();

                    unsigned int lastDot = (unsigned int)filePath.find_last_of('.');
                    std::wstring fileType = filePath.substr(lastDot + 1, filePath.size() - lastDot - 1);
                    if (fileType == L"csv") csv_json::ReadFromCSV(filePath.c_str());
                    else if (fileType == L"json") csv_json::ReadFromJSON(filePath.c_str());

                    unsigned int start = (unsigned int)filePath.find_last_of('\\') + 1;
                    CurrentFileName = filePath.substr(start, filePath.length() - start);

                    UpdGraphicsAndSettingsView();
                }
            }
            });

        

        EventConsHandler.ConnectToEvent(&GraphSettingPlusTrigger.gLeftMouseButtonEvent(), [&](void* pressedDown) {
            if (*(bool*)pressedDown) {
                if (csv_json::Devices.size() != 0) {

                    unsigned int graphWidth = (unsigned int)(Width - Width * Proportions::GraphicsSettingsBarSize);
                    unsigned int graphHeight = (unsigned int)(Height - Height * Proportions::TopBarSize);

                    GraphicSettingsStruct* sets = new GraphicSettingsStruct{
                        0,{QH},{&window},0,{QH},{&window},
                        0,//date1
                        {QH},{&window},{QH},{&window},{QH},{&window},{QH},{&window},{QH},{&window},{QH},{&window},
                        0,//date2
                        {QH},{&window},{QH},{&window},{QH},{&window},{QH},{&window},{QH},{&window},{QH},{&window},
                        {QH},{QH},{&window},{QH},{&window},
                        true,{},{},{},{},{graphWidth,graphHeight,TextureStorageType::RGBA,
                        Texture::TexParameters{ TextureWrapType::ClampToEdge,TextureWrapType::ClampToEdge,
                        TextureDownscalingFilterFunc::Nearest,TextureUpscalingFilterFunc::Nearest,TextureDepthStencilReadMode::Depth }},
                        {graphWidth,graphHeight}
                    };
                    GraphicsSettings.push_back(sets);

                    sets->FB.AttachTexture(sets->FB_TEX);
                    sets->FB.Finish();

                    {
                        float data_tex[] = {
                            -1 + Proportions::GraphicsSettingsBarSize * 2,-1,0,0,
                            -1 + Proportions::GraphicsSettingsBarSize * 2,1 - Proportions::TopBarSize * 2,0,1,
                            1,1 - Proportions::TopBarSize * 2,1,1,
                            1,1 - Proportions::TopBarSize * 2,1,1,
                            1,-1,1,0,
                            -1 + Proportions::GraphicsSettingsBarSize * 2,-1,0,0
                        };
                        sets->VA_TEX.Bind();
                        sets->VB_TEX.SetLayout({ 2,2 });
                        sets->VB_TEX.SetData(data_tex, sizeof(data_tex), VertexBufferDataUsage::StaticDraw);
                        sets->VA_TEX.Unbind();

                        float data_graph[] = {
                            -1,-1,0,0,
                            -1,1,0,1,
                            1,1,1,1,
                            1,1,1,1,
                            1,-1,1,0,
                            -1,-1,0,0
                        };
                        sets->VA_GRAPH.Bind();
                        sets->VB_GRAPH.SetLayout({ 2,2 });
                        sets->VB_GRAPH.SetData(data_graph, sizeof(data_graph), VertexBufferDataUsage::StaticDraw);
                        sets->VA_GRAPH.Unbind();

                    }


                    {
                        sets->DeviceNameQuad.Size = Vector<2>(Proportions::GraphicsSettingsBarSize, Proportions::GraphicSettingsSize * Proportions::GraphicSettingsDeviceNameSize);
                        sets->DeviceNameQuad.LocalOffset = Vector<2>(-1, 1);
                        sets->DeviceNameQuad.Color = Vector<3>(0.1f, 0.4f, 0.2f);
                        sets->DeviceNameTrigger.Size = sets->DeviceNameQuad.Size;
                        sets->DeviceNameTrigger.LocalOffset = Vector<2>(-1, 1);

                        sets->DataNameQuad.Size = Vector<2>(Proportions::GraphicsSettingsBarSize, Proportions::GraphicSettingsSize * Proportions::GraphicSettingsDataNameSize);
                        sets->DataNameQuad.LocalOffset = Vector<2>(-1, 1);
                        sets->DataNameQuad.Color = Vector<3>(0.1f, 0.2f, 0.4f);
                        sets->DataNameTrigger.Size = sets->DataNameQuad.Size;
                        sets->DataNameTrigger.LocalOffset = Vector<2>(-1, 1);

                        sets->DeviceNameTrigger.gEventsConsHandler().ConnectToEvent(&sets->DeviceNameTrigger.gLeftMouseButtonEvent(), [&, sets](void* pressedDown) {
                            if (*(bool*)pressedDown) {

                                sets->DataNameInd = 0;
                                sets->Date1Ind = 0;
                                sets->Date2Ind = 0;
                                if (sets->DeviceInd == 0) sets->DeviceInd = (unsigned int)csv_json::Devices.size() - 1;
                                else sets->DeviceInd--;

                                sets->RequiresGraphUpdate = true;

                                redraw();
                                window.SwapScreenBuffers();
                            }
                            });
                        sets->DeviceNameTrigger.gEventsConsHandler().ConnectToEvent(&sets->DeviceNameTrigger.gRightMouseButtonEvent(), [&, sets](void* pressedDown) {
                            if (*(bool*)pressedDown) {

                                sets->DataNameInd = 0;
                                sets->Date1Ind = 0;
                                sets->Date2Ind = 0;
                                if (sets->DeviceInd == csv_json::Devices.size() - 1) sets->DeviceInd = 0;
                                else sets->DeviceInd++;

                                sets->RequiresGraphUpdate = true;

                                redraw();
                                window.SwapScreenBuffers();
                            }
                            });

                        sets->DataNameTrigger.gEventsConsHandler().ConnectToEvent(&sets->DataNameTrigger.gLeftMouseButtonEvent(), [&, sets](void* pressedDown) {
                            if (*(bool*)pressedDown) {

                                if (sets->DataNameInd == 0) sets->DataNameInd = (unsigned int)csv_json::Devices[sets->DeviceInd].DataNames.size() - 1;
                                else sets->DataNameInd--;

                                sets->RequiresGraphUpdate = true;

                                redraw();
                                window.SwapScreenBuffers();
                            }
                            });
                        sets->DataNameTrigger.gEventsConsHandler().ConnectToEvent(&sets->DataNameTrigger.gRightMouseButtonEvent(), [&, sets](void* pressedDown) {
                            if (*(bool*)pressedDown) {

                                if (sets->DataNameInd == csv_json::Devices[sets->DeviceInd].DataNames.size() - 1) sets->DataNameInd = 0;
                                else sets->DataNameInd++;

                                sets->RequiresGraphUpdate = true;

                                redraw();
                                window.SwapScreenBuffers();
                            }
                            });
                    }


                    {
                        float dateXOffset = 0;
#define quadAndTriggerSetUpMacro(metric,size){\
                    sets->Date1##metric##Quad.Size = Vector<2>(Proportions::GraphicsSettingsBarSize * size, Proportions::GraphicSettingsSize * Proportions::GraphicSettingsDateSettingsSize / 2);\
                    sets->Date1##metric##Quad.LocalOffset = Vector<2>(-1 - dateXOffset / (sets->Date1##metric##Quad.Size[0] / 2), 1);\
                    sets->Date1##metric##Quad.Color = Vector<3>(RandNum(0, 1), RandNum(0, 1), RandNum(0, 1));\
                    sets->Date1##metric##Trigger.Size = sets->Date1##metric##Quad.Size;\
                    sets->Date1##metric##Trigger.LocalOffset = sets->Date1##metric##Quad.LocalOffset;\
                    \
                    sets->Date2##metric##Quad.Size = Vector<2>(Proportions::GraphicsSettingsBarSize * size, Proportions::GraphicSettingsSize * Proportions::GraphicSettingsDateSettingsSize / 2);\
                    sets->Date2##metric##Quad.LocalOffset = Vector<2>(-1 - dateXOffset / (sets->Date1##metric##Quad.Size[0] / 2), 1);\
                    sets->Date2##metric##Quad.Color = Vector<3>(RandNum(0, 1), RandNum(0, 1), RandNum(0, 1));\
                    sets->Date2##metric##Trigger.Size = sets->Date2##metric##Quad.Size;\
                    sets->Date2##metric##Trigger.LocalOffset = sets->Date2##metric##Quad.LocalOffset;\
                    \
                    dateXOffset += sets->Date1##metric##Quad.Size[0];\
                    }

                        quadAndTriggerSetUpMacro(Year, 2.f / 7.f);
                        quadAndTriggerSetUpMacro(Month, 1.f / 7.f);
                        quadAndTriggerSetUpMacro(Day, 1.f / 7.f);
                        quadAndTriggerSetUpMacro(Hour, 1.f / 7.f);
                        quadAndTriggerSetUpMacro(Minute, 1.f / 7.f);
                        quadAndTriggerSetUpMacro(Second, 1.f / 7.f);

#undef quadAndTriggerSetUpMacro
                    }

                    {
                        
#define datesTriggerMacro(num,metric,metricInd){\
                        EventConsHandler.ConnectToEvent(&sets->Date##num##metric##Trigger.gLeftMouseButtonEvent(), [&, sets](void* pressedDown) {\
                            const unsigned int maxDateVals[5] = { 12,30,23,59,59 };\
                            if (*(bool*)pressedDown) {\
                                unsigned int oldDate1=sets->Date1Ind; unsigned int oldDate2=sets->Date2Ind;\
                                csv_json::DeviceDataStruct::DateWidthDataStruct::DateStruct newDate = csv_json::Devices[sets->DeviceInd].Dates[sets->Date##num##Ind].Date;\
                                for (unsigned int i = metricInd; i < 6; i++) {\
                                    int* cint = &newDate.Second - i;\
                                    (*cint)--;\
                                    if (*cint < 0) {\
                                        *cint += maxDateVals[4 - i];\
                                    }\
                                    else if (i != 5 and *cint > (int)maxDateVals[4 - i]) {\
                                        *cint -= maxDateVals[4 - i];\
                                    }\
                                    else break;\
                                }\
                                unsigned int fi = 0;\
                                for (int i = sets->Date##num##Ind - 1; i >= 0; i--) {\
                                    if (csv_json::Devices[sets->DeviceInd].Dates[i].Date > newDate) continue;\
                                    else { fi = i; break; }\
                                }\
                                if (fi != (csv_json::Devices[sets->DeviceInd].Dates.size() - 1) and (fi + 1) != sets->Date##num##Ind and (csv_json::Devices[sets->DeviceInd].Dates[fi + 1].Date - newDate) < (newDate - csv_json::Devices[sets->DeviceInd].Dates[fi].Date)) fi++;\
                                sets->Date##num##Ind = fi;\
                                sets->Date2Ind=(sets->Date2Ind > sets->Date1Ind) ? sets->Date2Ind : sets->Date1Ind;\
                                if ((sets->Date2Ind-sets->Date1Ind)>=MAX_DATES_IN_GRAPH)\
                                    sets->Date2Ind=sets->Date1Ind+MAX_DATES_IN_GRAPH-1;\
                                sets->RequiresGraphUpdate = ((oldDate1!=sets->Date1Ind) or (oldDate2!=sets->Date2Ind));\
                                redraw();\
                                window.SwapScreenBuffers();\
                            }\
                            });\
                        EventConsHandler.ConnectToEvent(&sets->Date##num##metric##Trigger.gRightMouseButtonEvent(), [&, sets](void* pressedDown) {\
                            const unsigned int maxDateVals[5] = { 12,30,23,59,59 };\
                            if (*(bool*)pressedDown) {\
                                unsigned int oldDate1=sets->Date1Ind; unsigned int oldDate2=sets->Date2Ind;\
                                csv_json::DeviceDataStruct::DateWidthDataStruct::DateStruct newDate = csv_json::Devices[sets->DeviceInd].Dates[sets->Date##num##Ind].Date;\
                                for (unsigned int i = metricInd; i < 6; i++) {\
                                    int* cint = &newDate.Second - i;\
                                    (*cint)++;\
                                    if (*cint < 0) {\
                                        *cint += maxDateVals[4 - i];\
                                    }\
                                    else if (i != 5 and *cint > (int)maxDateVals[4 - i]) {\
                                        *cint -= maxDateVals[4 - i];\
                                    }\
                                    else break;\
                                }\
                                unsigned int fi = (unsigned int)csv_json::Devices[sets->DeviceInd].Dates.size() - 1;\
                                for (unsigned int i = sets->Date##num##Ind + 1; i < csv_json::Devices[sets->DeviceInd].Dates.size(); i++) {\
                                    if (csv_json::Devices[sets->DeviceInd].Dates[i].Date < newDate) continue;\
                                    else { fi = i; break; }\
                                }\
                                if (fi != 0 and (fi - 1) != sets->Date##num##Ind and (newDate - csv_json::Devices[sets->DeviceInd].Dates[fi - 1].Date) < (csv_json::Devices[sets->DeviceInd].Dates[fi].Date - newDate)) fi--;\
                                sets->Date##num##Ind = fi;\
                                sets->Date2Ind=(sets->Date2Ind > sets->Date1Ind) ? sets->Date2Ind : sets->Date1Ind;\
                                bool curDiffRes=((sets->Date2Ind-sets->Date1Ind)<MAX_DATES_IN_GRAPH);\
                                if ((sets->Date2Ind-sets->Date1Ind)>=MAX_DATES_IN_GRAPH)\
                                    sets->Date2Ind=sets->Date1Ind+MAX_DATES_IN_GRAPH-1;\
                                sets->RequiresGraphUpdate = ((oldDate1!=sets->Date1Ind) or (oldDate2!=sets->Date2Ind));\
                                redraw();\
                                window.SwapScreenBuffers();\
                            }\
                            });\
                        }

                        datesTriggerMacro(1, Second, 0);
                        datesTriggerMacro(2, Second, 0);
                        datesTriggerMacro(1, Minute, 1);
                        datesTriggerMacro(2, Minute, 1);
                        datesTriggerMacro(1, Hour, 2);
                        datesTriggerMacro(2, Hour, 2);
                        datesTriggerMacro(1, Day, 3);
                        datesTriggerMacro(2, Day, 3);
                        datesTriggerMacro(1, Month, 4);
                        datesTriggerMacro(2, Month, 4);
                        datesTriggerMacro(1, Year, 5);
                        datesTriggerMacro(2, Year, 5);


#undef datesTriggerMacro
                    }

                    {
                        float xOff = 0;

                        sets->GraphStaterQuad.Size = Vector<2>(Proportions::GraphicsSettingsBarSize * 0.1f, Proportions::GraphicSettingsSize *
                            (1 - Proportions::GraphicSettingsDeviceNameSize - Proportions::GraphicSettingsDataNameSize - Proportions::GraphicSettingsDateSettingsSize));
                        sets->GraphStaterQuad.LocalOffset = Vector<2>(-1, 1);

                        xOff += sets->GraphStaterQuad.Size[0];

                        sets->GraphColorQuad.Color = Vector<3>(RandNum(0, 1), RandNum(0, 1), RandNum(0, 1));
                        sets->GraphColorQuad.Size = Vector<2>(Proportions::GraphicsSettingsBarSize * 0.7f, sets->GraphStaterQuad.Size[1]);
                        sets->GraphColorQuad.LocalOffset = Vector<2>(-1 - xOff / (sets->GraphColorQuad.Size[0] / 2), 1);
                        sets->GraphColorTrigger.Size = sets->GraphColorQuad.Size;
                        sets->GraphColorTrigger.LocalOffset = sets->GraphColorQuad.LocalOffset;

                        xOff += sets->GraphColorQuad.Size[0];

                        sets->GraphSettingDeleteQuad.Color = Vector<3>(1,0,0);
                        sets->GraphSettingDeleteQuad.Size = Vector<2>(Proportions::GraphicsSettingsBarSize * 0.2f, sets->GraphStaterQuad.Size[1]);
                        sets->GraphSettingDeleteQuad.LocalOffset = Vector<2>(-1 - xOff / (sets->GraphSettingDeleteQuad.Size[0] / 2), 1);
                        sets->GraphSettingDeleteTrigger.Size = sets->GraphSettingDeleteQuad.Size;
                        sets->GraphSettingDeleteTrigger.LocalOffset = sets->GraphSettingDeleteQuad.LocalOffset;

                        sets->GraphColorTrigger.gEventsConsHandler().ConnectToEvent(&sets->GraphColorTrigger.gLeftMouseButtonEvent(), [&, sets](void* pressedDown) {
                            if (*(bool*)pressedDown) {

                                sets->GraphColorQuad.Color = Vector<3>(RandNum(0, 1), RandNum(0, 1), RandNum(0, 1));

                                sets->RequiresGraphUpdate = true;

                                redraw();
                                window.SwapScreenBuffers();
                            }
                            });

                        sets->GraphSettingDeleteTrigger.gEventsConsHandler().ConnectToEvent(&sets->GraphSettingDeleteTrigger.gLeftMouseButtonEvent(), [&, sets](void* pressedDown) {
                            if (*(bool*)pressedDown) {

                                for (unsigned int i = 0; i < GraphicsSettings.size(); i++) {
                                    if (GraphicsSettings[i] == sets) {

                                        GraphicSettingsStruct* toDeletePtr = GraphicsSettings[i];
                                        GraphicsSettings.erase(GraphicsSettings.begin() + i);
                                        
                                        UpdGraphicsAndSettingsView();

                                        delete toDeletePtr;

                                        return;
                                    }
                                }
                            }
                            });
                    }

                    UpdGraphicsAndSettingsView();

                }
            }
            });
        
        UpdGraphicsAndSettingsView();
        redraw();
        window.SwapScreenBuffers();

        while (!window.WindowWaitingToClose())
        {

            window.UpdateMouseData();

            window.WaitTillEvent();
        }


        ClearGraphicsSettings();
        UninitializeKursovayaEngine2();
        return 0;
    }
    catch (KURSAVAYAENGINE2_CORE_ERRORS&) {
        ClearGraphicsSettings();
        UninitializeKursovayaEngine2();
        return 0;
    }
}