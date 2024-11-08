#include"Shader.h"
#include"Tools/BinarySearch.h"

using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

void ShaderClass::ShaderDataClass::_SaveShaderDataNames() {

    GP::ShaderProgramClass::Bind();

    DynArr<GP::ShaderProgramClass::UniformData> data;
    GP::ShaderProgramClass::GetUniformsData(&data);

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
    GP::ShaderProgramClass::AttachShader(VS.gID());

	GP::ShaderClass FS(fsPath, GP::ShaderClass::TypesEnum::Fragment);
    FS.Compile();
    GP::ShaderProgramClass::AttachShader(FS.gID());

    GP::ShaderProgramClass::LinkShaders();

    _SaveShaderDataNames();
}
ShaderClass::ShaderDataClass::ShaderDataClass(const wchar_t* vsPath, const wchar_t* gsPath, const wchar_t* fsPath) {

	GP::ShaderClass VS(vsPath, GP::ShaderClass::TypesEnum::Vertex);
    VS.Compile();
    GP::ShaderProgramClass::AttachShader(VS.gID());

	GP::ShaderClass GS(gsPath, GP::ShaderClass::TypesEnum::Geometry);
    GS.Compile();
    GP::ShaderProgramClass::AttachShader(GS.gID());

	GP::ShaderClass FS(fsPath, GP::ShaderClass::TypesEnum::Fragment);
    FS.Compile();
    GP::ShaderProgramClass::AttachShader(FS.gID());

    GP::ShaderProgramClass::LinkShaders();

    _SaveShaderDataNames();
}
ShaderClass::ShaderDataClass::ShaderDataClass(const ShaderDataClass&& toCopy):
    GP::ShaderProgramClass(std::move(toCopy)), ShaderUniformsData(std::move(toCopy.ShaderUniformsData)) {

}
unsigned int ShaderClass::ShaderDataClass::GetUniformIDByName(const char* name) {
    unsigned int uniformInd = BinarySearch(&ShaderUniformsData[0], ShaderUniformsData.gLength(), name, 
        +[](const char* const& curName, const UniformDataStruct& uniformData)->bool {return curName < uniformData.Name; },
        +[](const char* const& curName, const UniformDataStruct& uniformData)->bool {return curName == uniformData.Name; });
    if (uniformInd == ShaderUniformsData.gLength() + 1) return 0;
    return ShaderUniformsData[uniformInd].ID;
}
void ShaderClass::ShaderDataClass::Bind() {
    GP::ShaderProgramClass::Bind();
}
ShaderClass::CFAC_UniformFuncs_Class ShaderClass::ShaderDataClass::gCFAC_UniformFuncs() {
    return CFAC_UniformFuncs_Class(*this);
}
const GP::ShaderProgramClass& ShaderClass::ShaderDataClass::gPrimitiveShader() const {
    return *this;
}
ShaderClass::ShaderDataClass::~ShaderDataClass() {}

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
    return ShaderDataStalker.GetTarget<ShaderDataClass>().GetUniformIDByName(name);
}
ShaderClass::CFAC_UniformFuncs_Class ShaderClass::gCFAC_UniformFuncs() const {
    return ShaderDataStalker.GetTarget<ShaderDataClass>().gCFAC_UniformFuncs();
}
const GP::ShaderProgramClass& ShaderClass::gPrimitiveShader() const {
    return ShaderDataStalker.GetTarget<ShaderDataClass>().gPrimitiveShader();
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
