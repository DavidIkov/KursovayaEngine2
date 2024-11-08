#include"Shader.h"
#include"glad/glad.h"
#include<string>
#include"Tools/GLDebug.h"
#include"OS_BasedStuff/FilesSystem.h"

using namespace KE2;
using namespace Graphics::Primitives;

#define Assert_NotDeleted_Macro if (Deleted) ErrorsSystemNamespace::SendError<<"Shader is already deleted">>ErrorsEnumWrapperStruct(ErrorsEnum::AlreadyDeleted);
#if defined KE2_Debug
#define Assert_NotCompiled_Macro if (Compiled) ErrorsSystemNamespace::SendError<<"Shader is already compiled">>ErrorsEnumWrapperStruct(ErrorsEnum::AlreadyCompiled);
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
		int success = 0; glSC(glGetShaderiv(ID, GL_COMPILE_STATUS, &success));
		if (success == false) {
			int infoLen = 0; glSC(glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &infoLen));
			std::string info(infoLen - 1, '\0');
			glSC(glGetShaderInfoLog(ID, infoLen, 0, (char*)info.c_str()));
			ErrorsSystemNamespace::SendError << "Failed to compile " <<
				((ShaderType == TypesEnum::Fragment) ? "fragment" : ((ShaderType == TypesEnum::Vertex) ? "vertex" : "geometry")) <<
				" shader, OpenGL returned a message: [" << info << "]" >> ErrorsEnumWrapperStruct(ErrorsEnum::FailedToCompile);
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