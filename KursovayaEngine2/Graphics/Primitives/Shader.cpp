#include"Shader.h"
#include"glad/glad.h"
#include"Tools/DebuggingTools.h"
#include<string>
#include"Tools/GLDebug.h"
#include"WinOS/FilesSystem.h"
#include"Tools/DebugRuntimeAssert.h"

using namespace Graphics::Primitives;

#define Assert_NotDeleted_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Deleted, "Shader is deleted", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#if defined KE2_Debug
#define Assert_NotCompiled_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Compiled, "Shader is already compiled", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#else
#define Assert_NotCompiled_Macro
#endif

ShaderClass::ShaderClass(const wchar_t* filePath, TypesEnum typ) {
#if defined KE2_Debug
	ShaderType = typ;
#endif
	glSC(ID = glCreateShader((typ == TypesEnum::Fragment) ? GL_FRAGMENT_SHADER : ((typ == TypesEnum::Vertex) ? GL_VERTEX_SHADER : GL_GEOMETRY_SHADER)));
	std::string scode = FilesSystemNamespace::SaveFileToString(filePath);
	const char* code = scode.c_str();
	glSC(glShaderSource(ID, 1, &code, 0));
}
ShaderClass::ShaderClass(TypesEnum typ, const char* code) {
#if defined KE2_Debug
	ShaderType = typ;
#endif
	glSC(ID = glCreateShader((typ == TypesEnum::Fragment) ? GL_FRAGMENT_SHADER : ((typ == TypesEnum::Vertex) ? GL_VERTEX_SHADER : GL_GEOMETRY_SHADER)));
	glSC(glShaderSource(ID, 1, &code, 0));
}
ShaderClass::ShaderClass(RespConstrFlag, const ShaderClass& toCopy) {
	memcpy(this, &toCopy, sizeof(ShaderClass));
	toCopy.Deleted = true;
}
ShaderClass::ShaderClass(const ShaderClass&& toCopy) {
	memcpy(this, &toCopy, sizeof(ShaderClass));
	toCopy.Deleted = true;
}
void ShaderClass::operator=(const ShaderClass&& toCopy) {
	memcpy(this, &toCopy, sizeof(ShaderClass));
	toCopy.Deleted = true;
}
void ShaderClass::Compile() {

	Assert_NotDeleted_Macro;
	Assert_NotCompiled_Macro;

	glSC(glCompileShader(ID));
#if defined KE2_Debug
	Compiled = true;
	{//check for compilation
		int success;
		char info[512];
		glSC(glGetShaderiv(ID, GL_COMPILE_STATUS, &success));
		if (!success) {
			glSC(glGetShaderInfoLog(ID, 512, 0, info));
			std::string msg = (ShaderType == TypesEnum::Fragment) ? "fragment " : ((ShaderType == TypesEnum::Vertex) ? "vertex " : "geometry ");
			msg += "shader compilation error: ";
			msg += info;
			DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, msg.c_str(), KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
		}
	}
#endif
}
unsigned int ShaderClass::gID() {
	Assert_NotDeleted_Macro;
	return ID;
}
ShaderClass::~ShaderClass() {
	if (not Deleted) {
		glSC(glDeleteShader(ID));
		Deleted = true;
	}
}
void ShaderClass::Delete() {
	Assert_NotDeleted_Macro;
	this->~ShaderClass();
}