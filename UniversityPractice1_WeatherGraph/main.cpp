
#include<string>
#include"Quad.h"
#include"ClickTrigger.h"
#include"Graphics/Primitives/FrameBuffer.h"
#include"Graphics/Primitives/VertexBuffer.h"
#include"Graphics/Primitives/VertexBuffer.h"
#include"RussianConsole.h"
#include"KursovayaEngine2Manager.h"
#include"Graphics/Primitives/RenderingPreset.h"
#include"Graphics/Abstractions/TextRenderer.h"
#include"csv_json.h"
#include"WinOS/FileExplorerDialog.h"
#include"WinOS/wchar_t_Operations.h"
#include"Graphics/Primitives/Renderer.h"
#include"Tools/DebuggingTools.h"
#include"Tools/DynArr.h"
#include<iostream>
#include"Tools/RandomNumber.h"

using namespace Graphics::Abstractions;

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
    VertexArrayClass VA_TEX;
    VertexBufferClass VB_TEX;
    VertexArrayClass VA_GRAPH;
    VertexBufferClass VB_GRAPH;
    TextureClass FB_TEX;
    FrameBufferClass FB;

    TextureClass GRAPH_DATA_TEX;
};
std::vector<GraphicSettingsStruct*> GraphicsSettings;
float GraphicsSettingsOffset = 0;
void ClearGraphicsSettings() {
    for (unsigned int i = 0; i < GraphicsSettings.size(); i++) delete GraphicsSettings[i];
    GraphicsSettings.clear();
}


int main()
{

    InitializeKE2();
    
    EnableRussianConsole();

    try {

        EventConnectionsHandlerClass EventConsHandler;

        unsigned int Width = 900;
        unsigned int Height = 600;


        WindowClass window(Width, Height, "kursavayaengine2 window...", false, 10);

        TextRendererClass TEXT_RENDERER(L"Shaders/text.vs", L"Shaders/text.fs");
        StalkerClass ArialFont = TEXT_RENDERER.AddFont(50, "arial.ttf",
            L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"\
            "`abcdefghijklmnopqrstuvwxyz{|}~"\
            "�����Ũ����������������������������������������������������������");
            
        RenderingPresetClass Preset2D(
            false, RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back, RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise,
            false, true, RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual,
            false, 0xff, RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::AlwaysPass, 1, 0xff, RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
            RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep, RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Replace,
            true, 0, 0, 0, 0, RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.1f, 0.2f, 0.3f
        );
        RenderingPresetClass PresetGraphFB(
            false, RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back, RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise,
            false, true, RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual,
            false, 0xff, RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::AlwaysPass, 1, 0xff, RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
            RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep, RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Replace,
            false, 0, 0, 0, 0, RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha,
            0.1f, 0.2f, 0.3f
        );


        ShaderProgramClass GraphicsSP;
        {
            ShaderClass VS(L"Shaders/graphic.vs", ShaderClass::TypesEnum::Vertex);
            VS.Compile();
            GraphicsSP.AttachShader(VS);

            ShaderClass FS(L"Shaders/graphic.fs", ShaderClass::TypesEnum::Fragment);
            FS.Compile();
            GraphicsSP.AttachShader(FS);

            GraphicsSP.LinkShaders();
        }
        ShaderProgramClass SimpleImageSP;
        {
            ShaderClass VS(L"Shaders/simpleImage.vs", ShaderClass::TypesEnum::Vertex);
            VS.Compile();
            SimpleImageSP.AttachShader(VS);

            ShaderClass FS(L"Shaders/Quad2dImage.fs", ShaderClass::TypesEnum::Fragment);
            FS.Compile();
            SimpleImageSP.AttachShader(FS);

            SimpleImageSP.LinkShaders();
        }
        
        QuadsHandler QH(L"Shaders/Quad2d.vs", L"Shaders/Quad2d.fs");

        QuadsHandler::Quad TopQuad(QH);
        TopQuad.Position = Vector2F(0, 1);
        TopQuad.LocalOffset = Vector2F(0, 1);
        TopQuad.Size = Vector2F(1, Proportions::TopBarSize);
        TopQuad.Color = Vector3F(0.5f,0.1f,1.f);

        ClickTrigger TopTrigger(&window);
        TopTrigger.Position = Vector2F(1, 1);
        TopTrigger.LocalOffset = Vector2F(1, 1);
        TopTrigger.Size = Vector2F(1 - Proportions::TopBarLeftTextSize, Proportions::TopBarSize);

        QuadsHandler::Quad LeftQuad(QH);
        LeftQuad.Position = Vector2F(-1, 1 - 2 * Proportions::TopBarSize);
        LeftQuad.LocalOffset = Vector2F(-1, 1);
        LeftQuad.Size = Vector2F(Proportions::LeftBarSize, 1 - Proportions::TopBarSize);
        LeftQuad.Color = Vector3F(0.2f, 1.f, 0.7f);

        QuadsHandler QIH(L"Shaders/Quad2dImage.vs", L"Shaders/Quad2dImage.fs");

        QuadsHandler::ImageQuad AddGraphSettingPlus(QIH, TextureClass{TextureClass::DimensionsEnum::Two, "plus.png", TextureSettingsClass{
            TextureSettingsClass::WrapTypeEnum::ClampToEdge,TextureSettingsClass::WrapTypeEnum::ClampToEdge,
            TextureSettingsClass::DownscalingFilterFuncEnum::Linear,TextureSettingsClass::UpscalingFilterFuncEnum::Linear,
            TextureSettingsClass::DepthStencilReadModeEnum::Depth} });
        AddGraphSettingPlus.TexClampingType = QuadsHandler::ImageQuad::TextureClampingType::SquareByY;
        AddGraphSettingPlus.Size = Vector2F(Proportions::GraphicsSettingsBarSize, Proportions::GraphicSettingsSize);
        AddGraphSettingPlus.LocalOffset = Vector2F(0, 1);

        ClickTrigger GraphSettingPlusTrigger(&window);
        GraphSettingPlusTrigger.Size = Vector2F(Proportions::GraphicsSettingsBarSize, Proportions::GraphicSettingsSize);
        GraphSettingPlusTrigger.LocalOffset = Vector2F(-1, 1);

        std::function<void()> redraw([&]() {


            Preset2D.Bind();

            window.ClearAllBuffers();

            TopQuad.Draw();

            TEXT_RENDERER.RenderText(ArialFont, L"File name:",
                Vector2F(-1, 1 - Proportions::TopBarSize),
                Vector2F(-1, 0), Vector2U(Width, Height), Vector2F(Proportions::TopBarLeftTextSize, Proportions::TopBarSize), 1.f);

            TEXT_RENDERER.RenderText(ArialFont, CurrentFileName.c_str(),
                Vector2F(-1 + Proportions::TopBarLeftTextSize * 2, 1 - Proportions::TopBarSize),
                Vector2F(-1, 0), Vector2U(Width, Height), Vector2F(1 - Proportions::TopBarLeftTextSize, Proportions::TopBarSize), 1.f);


            LeftQuad.Draw();

            
            AddGraphSettingPlus.Draw(Width, Height);

            for (unsigned int gsi = 0; gsi < GraphicsSettings.size(); gsi++) {

                GraphicSettingsStruct* sets = GraphicsSettings[gsi];

                auto& devNamQ = sets->DeviceNameQuad;
                devNamQ.Draw();
                TEXT_RENDERER.RenderText(ArialFont, csv_json::Devices[sets->DeviceInd].Name.c_str(),
                    Vector2F(devNamQ.Position[0], devNamQ.Position[1] - devNamQ.Size[1]),
                    Vector2F(devNamQ.LocalOffset[0], 0), Vector2U(Width, Height), devNamQ.Size, 1.f);

                auto& dataNamQ = sets->DataNameQuad;
                dataNamQ.Draw();
                std::wstring wDataName; char_to_wchar(1251, csv_json::Devices[sets->DeviceInd].DataNames[sets->DataNameInd].DataName, wDataName);
                TEXT_RENDERER.RenderText(ArialFont, wDataName.c_str(),
                    Vector2F(dataNamQ.Position[0], dataNamQ.Position[1] - dataNamQ.Size[1]),
                    Vector2F(dataNamQ.LocalOffset[0], 0), Vector2U(Width, Height), dataNamQ.Size, 1.f);
#define dateDrawMacro(num,metric) {\
                auto& dateQRef = sets->Date##num##metric##Quad;\
                dateQRef.Draw();\
                std::wstring wideDate; std::string strDate = std::to_string(csv_json::Devices[sets->DeviceInd].Dates[sets->Date##num##Ind].Date.metric); char_to_wchar(1251, strDate, wideDate);\
                TEXT_RENDERER.RenderText(ArialFont, wideDate.c_str(),\
                    Vector2F(dateQRef.Position[0] - dateQRef.LocalOffset[0] * dateQRef.Size[0], dateQRef.Position[1] - dateQRef.Size[1]),\
                    Vector2F(0,0), Vector2U(Width,Height),dateQRef.Size,1.f);\
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

                sets->GraphStaterQuad.Color = csv_json::Devices[sets->DeviceInd].DataNames[sets->DataNameInd].IsNumber ? Vector3F(0, 1, 0) : Vector3F(1, 0, 0);
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
                    GraphicsSP.SetUniform3f("u_LineColor", sets->GraphColorQuad.Color[0], sets->GraphColorQuad.Color[1], sets->GraphColorQuad.Color[2]);
                    
                    float* nums = new float[datesAmount];

                    /*for (unsigned int i = 0; i < datesAmount; i++) {
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
                    }*/
                    for (unsigned int i = 0; i < datesAmount; i++) {

                        float val = csv_json::Devices[sets->DeviceInd].Dates[sets->Date1Ind + i].Data[sets->DataNameInd];
                        val = (val - minValue) / (maxValue - minValue);

                        nums[i] = val;
                        
                    }


                    sets->GRAPH_DATA_TEX.Bind();
                    sets->GRAPH_DATA_TEX.SetData(datesAmount, nums, TextureDataSettingsClass{ TextureDataSettingsClass::DataFormatOnGPU_Enum::Red,
                        TextureDataSettingsClass::DataFormatOnCPU_Enum::Red,TextureDataSettingsClass::DataTypeOnCPU_Enum::Float });

                    delete[] nums;

                    sets->FB.Bind();
                    sets->FB.ClearColorBuffer();
                    sets->VA_GRAPH.Bind();
                    sets->GRAPH_DATA_TEX.Bind(0);
                    RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, 6);
                    sets->VA_GRAPH.Unbind();
                    sets->FB.Unbind(Width, Height);
                }

                Preset2D.Bind();
                sets->VA_TEX.Bind();
                SimpleImageSP.Bind();
                sets->FB_TEX.Bind();
                RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, 6);
                sets->VA_TEX.Unbind();
            }

#undef dateDrawMacro

            });

        std::function<void()> UpdGraphicsAndSettingsView([&]() {

            AddGraphSettingPlus.Position = Vector2F(-1 + Proportions::GraphicsSettingsBarSize, 1 - 2 * (Proportions::TopBarSize + GraphicsSettings.size() * Proportions::GraphicSettingsSize));
            GraphSettingPlusTrigger.Position = Vector2F(-1, 1 - 2 * (Proportions::TopBarSize + GraphicsSettings.size() * Proportions::GraphicSettingsSize));

            for (unsigned int gsi = 0; gsi < GraphicsSettings.size(); gsi++) {
                GraphicSettingsStruct* settings = GraphicsSettings[gsi];

                float curY = Proportions::TopBarSize + gsi * Proportions::GraphicSettingsSize;

                settings->DeviceNameQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->DeviceNameTrigger.Position = settings->DeviceNameQuad.Position;

                curY += settings->DeviceNameQuad.Size[1];

                settings->DataNameQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->DataNameTrigger.Position = settings->DataNameQuad.Position;

                curY += settings->DataNameQuad.Size[1];

                settings->Date1YearQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date1YearTrigger.Position = settings->Date1YearQuad.Position;
                settings->Date1MonthQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date1MonthTrigger.Position = settings->Date1MonthQuad.Position;
                settings->Date1DayQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date1DayTrigger.Position = settings->Date1DayQuad.Position;
                settings->Date1HourQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date1HourTrigger.Position = settings->Date1HourQuad.Position;
                settings->Date1MinuteQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date1MinuteTrigger.Position = settings->Date1MinuteQuad.Position;
                settings->Date1SecondQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date1SecondTrigger.Position = settings->Date1SecondQuad.Position;

                curY += settings->Date1YearQuad.Size[1];

                settings->Date2YearQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date2YearTrigger.Position = settings->Date2YearQuad.Position;
                settings->Date2MonthQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date2MonthTrigger.Position = settings->Date2MonthQuad.Position;
                settings->Date2DayQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date2DayTrigger.Position = settings->Date2DayQuad.Position;
                settings->Date2HourQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date2HourTrigger.Position = settings->Date2HourQuad.Position;
                settings->Date2MinuteQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date2MinuteTrigger.Position = settings->Date2MinuteQuad.Position;
                settings->Date2SecondQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->Date2SecondTrigger.Position = settings->Date2SecondQuad.Position;
                
                curY += settings->Date2YearQuad.Size[1];

                settings->GraphStaterQuad.Position = Vector2F(-1, 1 - 2 * curY);

                settings->GraphColorQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->GraphColorTrigger.Position = settings->GraphColorQuad.Position;

                settings->GraphSettingDeleteQuad.Position = Vector2F(-1, 1 - 2 * curY);
                settings->GraphSettingDeleteTrigger.Position = settings->GraphSettingDeleteQuad.Position;
                
            }

            redraw();
            window.SwapScreenBuffers();

            });

        EventConsHandler.ConnectToEvent(&TopTrigger.gLeftMouseButtonEvent(), [&](void* pressedDown) {
            if (*(bool*)pressedDown) {
                std::cout << "������� �� ��� ��������� ����� ������ ���� ANCI!!!! ����� ������ ������� �����" << std::endl;
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
                        true,{},{},{},{},TextureClass{TextureClass::DimensionsEnum::Two, Vector3U(graphWidth,graphHeight,0),nullptr,
                        TextureSettingsClass{TextureSettingsClass::WrapTypeEnum::ClampToEdge,TextureSettingsClass::WrapTypeEnum::ClampToEdge,
                        TextureSettingsClass::DownscalingFilterFuncEnum::Nearest,TextureSettingsClass::UpscalingFilterFuncEnum::Nearest,
                        TextureSettingsClass::DepthStencilReadModeEnum::Depth},
                        TextureDataSettingsClass{TextureDataSettingsClass::DataFormatOnGPU_Enum::RGBA,
                        TextureDataSettingsClass::DataFormatOnCPU_Enum::RGBA,TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte}},
                        {graphWidth,graphHeight},TextureClass{TextureClass::DimensionsEnum::One, Vector3U(0,0,0),nullptr,
                        TextureSettingsClass{TextureSettingsClass::WrapTypeEnum::ClampToEdge,TextureSettingsClass::WrapTypeEnum::ClampToEdge,
                        TextureSettingsClass::DownscalingFilterFuncEnum::Linear,TextureSettingsClass::UpscalingFilterFuncEnum::Linear,
                        TextureSettingsClass::DepthStencilReadModeEnum::Depth},
                        TextureDataSettingsClass{TextureDataSettingsClass::DataFormatOnGPU_Enum::Red,
                        TextureDataSettingsClass::DataFormatOnCPU_Enum::Red,TextureDataSettingsClass::DataTypeOnCPU_Enum::Float}},

                    };
                    GraphicsSettings.push_back(sets);

                    sets->FB.AttachTexture(sets->FB_TEX.gID(), TextureDataSettingsClass::DataFormatOnGPU_Enum::RGBA);
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
                        sets->VB_TEX.Bind();
                        sets->VB_TEX.SetLayout(VertexBufferClass::BufferDataTypeEnum::Float, { 2,2 });
                        sets->VB_TEX.SetData(sizeof(data_tex), data_tex, VertexBufferClass::BufferDataUsageEnum::StaticDraw);
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
                        sets->VB_GRAPH.Bind();
                        sets->VB_GRAPH.SetLayout(VertexBufferClass::BufferDataTypeEnum::Float, { 2,2 });
                        sets->VB_GRAPH.SetData(sizeof(data_graph), data_graph, VertexBufferClass::BufferDataUsageEnum::StaticDraw);
                        sets->VA_GRAPH.Unbind();

                    }


                    {
                        sets->DeviceNameQuad.Size = Vector2F(Proportions::GraphicsSettingsBarSize, Proportions::GraphicSettingsSize * Proportions::GraphicSettingsDeviceNameSize);
                        sets->DeviceNameQuad.LocalOffset = Vector2F(-1, 1);
                        sets->DeviceNameQuad.Color = Vector3F(0.1f, 0.4f, 0.2f);
                        sets->DeviceNameTrigger.Size = sets->DeviceNameQuad.Size;
                        sets->DeviceNameTrigger.LocalOffset = Vector2F(-1, 1);

                        sets->DataNameQuad.Size = Vector2F(Proportions::GraphicsSettingsBarSize, Proportions::GraphicSettingsSize * Proportions::GraphicSettingsDataNameSize);
                        sets->DataNameQuad.LocalOffset = Vector2F(-1, 1);
                        sets->DataNameQuad.Color = Vector3F(0.1f, 0.2f, 0.4f);
                        sets->DataNameTrigger.Size = sets->DataNameQuad.Size;
                        sets->DataNameTrigger.LocalOffset = Vector2F(-1, 1);

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
                    sets->Date1##metric##Quad.Size = Vector2F(Proportions::GraphicsSettingsBarSize * size, Proportions::GraphicSettingsSize * Proportions::GraphicSettingsDateSettingsSize / 2);\
                    sets->Date1##metric##Quad.LocalOffset = Vector2F(-1 - dateXOffset / (sets->Date1##metric##Quad.Size[0] / 2), 1);\
                    sets->Date1##metric##Quad.Color = Vector3F(RandNum(0, 1), RandNum(0, 1), RandNum(0, 1));\
                    sets->Date1##metric##Trigger.Size = sets->Date1##metric##Quad.Size;\
                    sets->Date1##metric##Trigger.LocalOffset = sets->Date1##metric##Quad.LocalOffset;\
                    \
                    sets->Date2##metric##Quad.Size = Vector2F(Proportions::GraphicsSettingsBarSize * size, Proportions::GraphicSettingsSize * Proportions::GraphicSettingsDateSettingsSize / 2);\
                    sets->Date2##metric##Quad.LocalOffset = Vector2F(-1 - dateXOffset / (sets->Date1##metric##Quad.Size[0] / 2), 1);\
                    sets->Date2##metric##Quad.Color = Vector3F(RandNum(0, 1), RandNum(0, 1), RandNum(0, 1));\
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

                        sets->GraphStaterQuad.Size = Vector2F(Proportions::GraphicsSettingsBarSize * 0.1f, Proportions::GraphicSettingsSize *
                            (1 - Proportions::GraphicSettingsDeviceNameSize - Proportions::GraphicSettingsDataNameSize - Proportions::GraphicSettingsDateSettingsSize));
                        sets->GraphStaterQuad.LocalOffset = Vector2F(-1, 1);

                        xOff += sets->GraphStaterQuad.Size[0];

                        sets->GraphColorQuad.Color = Vector3F(RandNum(0, 1), RandNum(0, 1), RandNum(0, 1));
                        sets->GraphColorQuad.Size = Vector2F(Proportions::GraphicsSettingsBarSize * 0.7f, sets->GraphStaterQuad.Size[1]);
                        sets->GraphColorQuad.LocalOffset = Vector2F(-1 - xOff / (sets->GraphColorQuad.Size[0] / 2), 1);
                        sets->GraphColorTrigger.Size = sets->GraphColorQuad.Size;
                        sets->GraphColorTrigger.LocalOffset = sets->GraphColorQuad.LocalOffset;

                        xOff += sets->GraphColorQuad.Size[0];

                        sets->GraphSettingDeleteQuad.Color = Vector3F(1,0,0);
                        sets->GraphSettingDeleteQuad.Size = Vector2F(Proportions::GraphicsSettingsBarSize * 0.2f, sets->GraphStaterQuad.Size[1]);
                        sets->GraphSettingDeleteQuad.LocalOffset = Vector2F(-1 - xOff / (sets->GraphSettingDeleteQuad.Size[0] / 2), 1);
                        sets->GraphSettingDeleteTrigger.Size = sets->GraphSettingDeleteQuad.Size;
                        sets->GraphSettingDeleteTrigger.LocalOffset = sets->GraphSettingDeleteQuad.LocalOffset;

                        sets->GraphColorTrigger.gEventsConsHandler().ConnectToEvent(&sets->GraphColorTrigger.gLeftMouseButtonEvent(), [&, sets](void* pressedDown) {
                            if (*(bool*)pressedDown) {

                                sets->GraphColorQuad.Color = Vector3F(RandNum(0, 1), RandNum(0, 1), RandNum(0, 1));

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

    }
    catch (KURSAVAYAENGINE2_CORE_ERRORS&) {}

    ClearGraphicsSettings();
    UninitializeKE2();
    return 0;
}