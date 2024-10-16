#include"Shader.h"
#include"Tools/DebugRuntimeAssert.h"
#include"Tools/BinarySearch.h"

using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;
#define Assert_NotDeleted_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Deleted, "ShaderClass is deleted", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);

void ShaderClass::ShaderDataClass::_SaveShaderDataNames() {

	ShaderProgram.Bind();

    DynArr<GP::ShaderProgramClass::UniformData> data;
    ShaderProgram.GetUniformsData(&data);

    ShaderUniformsData.ChangeCapacity(data.gLength());

    std::string* lastMinStringPtr = nullptr;
    std::string* curComparingMinStringPtr = nullptr;
    unsigned int curComparingMinStringID = 0;

    for (unsigned int gsi = 0; gsi < data.gLength(); gsi++) {
        for (unsigned int csi = 0; csi < data.gLength(); csi++)
            if ((lastMinStringPtr == nullptr or data[csi].Name > *lastMinStringPtr) and (curComparingMinStringPtr == nullptr or data[csi].Name < *curComparingMinStringPtr)) {
                curComparingMinStringPtr = &data[csi].Name;
                curComparingMinStringID = data[csi].ID;
            }
        lastMinStringPtr = curComparingMinStringPtr;
        curComparingMinStringPtr = nullptr;

        ShaderUniformsData.InsertByConstructor(gsi, UniformDataStruct{ *lastMinStringPtr, curComparingMinStringID });
    }

}
ShaderClass::ShaderDataClass::ShaderDataClass(const wchar_t* vsPath, const wchar_t* fsPath) {

	GP::ShaderClass VS(vsPath, GP::ShaderClass::TypesEnum::Vertex);
    VS.Compile();
    ShaderProgram.AttachShader(VS.gID());

	GP::ShaderClass FS(fsPath, GP::ShaderClass::TypesEnum::Fragment);
    FS.Compile();
    ShaderProgram.AttachShader(FS.gID());

    ShaderProgram.LinkShaders();

    _SaveShaderDataNames();
}
ShaderClass::ShaderDataClass::ShaderDataClass(const wchar_t* vsPath, const wchar_t* gsPath, const wchar_t* fsPath) {

	GP::ShaderClass VS(vsPath, GP::ShaderClass::TypesEnum::Vertex);
    VS.Compile();
    ShaderProgram.AttachShader(VS.gID());

	GP::ShaderClass GS(gsPath, GP::ShaderClass::TypesEnum::Geometry);
    GS.Compile();
    ShaderProgram.AttachShader(GS.gID());

	GP::ShaderClass FS(fsPath, GP::ShaderClass::TypesEnum::Fragment);
    FS.Compile();
    ShaderProgram.AttachShader(FS.gID());

    ShaderProgram.LinkShaders();

    _SaveShaderDataNames();
}
ShaderClass::ShaderDataClass::ShaderDataClass(const ShaderDataClass&& toCopy):
    ShaderProgram(std::move(toCopy.ShaderProgram)), ShaderUniformsData(std::move(toCopy.ShaderUniformsData)) {
    GP::ShaderProgramClass::CFAC_FullAccess_Class k(ShaderProgram);
    toCopy.Deleted = true;
}
unsigned int ShaderClass::ShaderDataClass::GetUniformIdByName(const char* name) {
    unsigned int uniformInd = BinarySearch(&ShaderUniformsData[0], ShaderUniformsData.gLength(), name, 
        +[](const char* const& curName, const UniformDataStruct& uniformData)->bool {return curName < uniformData.Name; },
        +[](const char* const& curName, const UniformDataStruct& uniformData)->bool {return curName == uniformData.Name; });
    if (uniformInd == ShaderUniformsData.gLength() + 1) return 0;
    return ShaderUniformsData[uniformInd].ID;
}
void ShaderClass::ShaderDataClass::Bind() {
    ShaderProgram.Bind();
}
ShaderClass::CFAC_UniformFuncs_Class ShaderClass::ShaderDataClass::g_CFAC_UniformFuncs() {
    return CFAC_UniformFuncs_Class(ShaderProgram);
}
ShaderClass::ShaderDataClass::~ShaderDataClass() {
    if (not Deleted) {
        Deleted = true;
    }
}

DynArr<ShaderClass::ShaderDataClass> ShaderClass::ShadersStorage;

ShaderClass::ShaderClass(const wchar_t* vertexShaderPath, const wchar_t* fragmentShaderPath, void* ptrToCustomStorageOfShaderDataUpdaterFunc, void(*updShaderDataFuncPtr)(void*, ShaderClass*)) :
    UpdShaderDataFunc(updShaderDataFuncPtr), ShaderDataStalker(&ShadersStorage, ShadersStorage.gLength()), 
    PtrToCustomStorageOfShaderDataUpdaterFunc(ptrToCustomStorageOfShaderDataUpdaterFunc) {
    ShadersStorage.InsertByConstructor(ShadersStorage.gLength(), vertexShaderPath, fragmentShaderPath);
}
ShaderClass::ShaderClass(const wchar_t* vertexShaderPath, const wchar_t* geometryShaderPath, const wchar_t* fragmentShaderPath, void* ptrToCustomStorageOfShaderDataUpdaterFunc, void(*updShaderDataFuncPtr)(void*, ShaderClass*)) :
    UpdShaderDataFunc(updShaderDataFuncPtr), ShaderDataStalker(&ShadersStorage, ShadersStorage.gLength()),
    PtrToCustomStorageOfShaderDataUpdaterFunc(ptrToCustomStorageOfShaderDataUpdaterFunc) {
    ShadersStorage.InsertByConstructor(ShadersStorage.gLength(), vertexShaderPath, geometryShaderPath, fragmentShaderPath);
}

ShaderClass::ShaderClass(const ShaderClass& toCopy, void* ptrToCustomStorageOfShaderDataUpdaterFunc, void(*updShaderDataFuncPtr)(void*, ShaderClass*)) :
    UpdShaderDataFunc(updShaderDataFuncPtr), ShaderDataStalker(toCopy.ShaderDataStalker),
    PtrToCustomStorageOfShaderDataUpdaterFunc(ptrToCustomStorageOfShaderDataUpdaterFunc) {
}
ShaderClass::ShaderClass(const ShaderClass&& toCopy) :
    UpdShaderDataFunc(toCopy.UpdShaderDataFunc), ShaderDataStalker(std::move(toCopy.ShaderDataStalker)),
    PtrToCustomStorageOfShaderDataUpdaterFunc(toCopy.PtrToCustomStorageOfShaderDataUpdaterFunc) {
    toCopy.Deleted = true;
}
ShaderClass::~ShaderClass() {
    if (not Deleted) {
        Deleted = true;
        if (ShadersStorage.IsStalkersAmountForIndexEqualTo(ShaderDataStalker.gTargetInd(), 1))
            ShadersStorage.Remove(ShaderDataStalker.gTargetInd());
    }
}
void ShaderClass::operator=(const ShaderClass&& toCopy) {
    Delete();
    Deleted = false;
    toCopy.Deleted = true;
    UpdShaderDataFunc = toCopy.UpdShaderDataFunc;
    ShaderDataStalker = StalkerClass(std::move(toCopy.ShaderDataStalker));
    PtrToCustomStorageOfShaderDataUpdaterFunc = toCopy.PtrToCustomStorageOfShaderDataUpdaterFunc;
}
unsigned int ShaderClass::GetUniformIDByName(const char* name) {
    return ShaderDataStalker.GetTarget<ShaderDataClass>().GetUniformIdByName(name);
}
ShaderClass::CFAC_UniformFuncs_Class ShaderClass::g_CFAC_UniformFuncs() {
    return ShaderDataStalker.GetTarget<ShaderDataClass>().g_CFAC_UniformFuncs();
}
void ShaderClass::Bind() {
    ShaderDataStalker.GetTarget<ShaderDataClass>().Bind();
    if (UpdShaderDataFunc != nullptr) UpdShaderDataFunc(PtrToCustomStorageOfShaderDataUpdaterFunc, this);
}
void ShaderClass::Unbind() {
    GP::ShaderProgramClass::Unbind();
}
void ShaderClass::Delete() {
    this->~ShaderClass();
}
