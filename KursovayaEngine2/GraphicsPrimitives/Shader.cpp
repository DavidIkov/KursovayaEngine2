#include"Shader.h"
#include"glad/glad.h"
#include"Tools/DebuggingTools.h"
#include<string>
#include"Tools/GLDebug.h"
#include"WinOS/FilesSystem.h"

using namespace GraphicsPrimitives;

unsigned int ShaderClass::gID() {
#if defined Debug
	if (Deleted) {
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER IS ALREADY DELETED, ACCESSING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
		return 0;
	}
#endif
	return ID;
}
ShaderClass::ShaderClass(const wchar_t* filePath, TypesEnum typ) {
#if defined Debug
	ShaderType = typ;
#endif
	glSC(ID = glCreateShader((typ == TypesEnum::Fragment) ? GL_FRAGMENT_SHADER : ((typ == TypesEnum::Vertex) ? GL_VERTEX_SHADER : GL_GEOMETRY_SHADER)));
	std::string scode = FilesSystemNamespace::SaveFileToString(filePath);
	const char* code = scode.c_str();
	glSC(glShaderSource(ID, 1, &code, 0));
}
ShaderClass::ShaderClass(TypesEnum typ, const char* code) {
#if defined Debug
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
#if defined Debug
	if (Compiled) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER IS ALREADY COMPILED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
	else 
#endif
	{
		glSC(glCompileShader(ID));
#if defined Debug
		Compiled = true;
		{//check for compilation
			int success;
			char info[512];
			glSC(glGetShaderiv(ID, GL_COMPILE_STATUS, &success));
			if (!success) {
				glSC(glGetShaderInfoLog(ID, 512, 0, info));
				std::string msg = (ShaderType == TypesEnum::Fragment) ? "FRAGMENT " : ((ShaderType == TypesEnum::Vertex) ? "VERTEX " : "GEOMETRY ");
				msg += "SHADER COMPILATION ERROR: ";
				msg += info;
				DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, msg.c_str(), KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
			}
		}
#endif
	}
}
ShaderClass::~ShaderClass() {
	if (not Deleted) {// no need for warning becouse destructor will be called at end of scope anyway
		glSC(glDeleteShader(ID));
		Deleted = true;
	}
}
void ShaderClass::Delete() {
#if defined Debug
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
	else 
#endif
		this->~ShaderClass();
}