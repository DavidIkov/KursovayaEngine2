#include"Shader.h"
#include"glad/glad.h"
#include"Tools/DebuggingTools.h"
#include<string>
#include"Tools/GLDebug.h"
#include"Tools/ReadFromFile.h"

unsigned int Shader::gID() const {
	if (Deleted) {
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER IS ALREADY DELETED, ACCESSING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
		return 0;
	}
	return ID;
}
Shader::Shader(const char* filePath, ShaderTypesEnum typ) {
	ShaderType = typ;
	ID = glCreateShader((typ == ShaderTypesEnum::Fragment) ? GL_FRAGMENT_SHADER : ((typ == ShaderTypesEnum::Vertex) ? GL_VERTEX_SHADER : GL_GEOMETRY_SHADER));
	std::string scode = ReadFromFile(filePath);
	const char* code = scode.c_str();
	glSC(glShaderSource(ID, 1, &code, 0));
}
Shader::Shader(ShaderTypesEnum typ, const char* code) {
	ShaderType = typ;
	ID = glCreateShader((typ == ShaderTypesEnum::Fragment) ? GL_FRAGMENT_SHADER : ((typ == ShaderTypesEnum::Vertex) ? GL_VERTEX_SHADER : GL_GEOMETRY_SHADER));
	glSC(glShaderSource(ID, 1, &code, 0));
}
Shader::Shader(Shader&& tempS) {
	memcpy(this, &tempS, sizeof(tempS));
	tempS.Deleted = true;
}
void Shader::Compile() {
	if (Compiled) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER IS ALREADY COMPILED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
	else {
		glSC(glCompileShader(ID));
		Compiled = true;
		{//check for compilation
			int success;
			char info[512];
			glSC(glGetShaderiv(ID, GL_COMPILE_STATUS, &success));
			if (!success) {
				glSC(glGetShaderInfoLog(ID, 512, 0, info));
				std::string msg = (ShaderType == ShaderTypesEnum::Fragment) ? "FRAGMENT " : ((ShaderType == ShaderTypesEnum::Vertex) ? "VERTEX " : "GEOMETRY ");
				msg += "SHADER COMPILATION ERROR: ";
				msg += info;
				DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, msg.c_str(), KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
			}
		}
	}
}
Shader::~Shader() {
	if (not Deleted) {// no need for warning becouse destructor will be called at end of scope anyway
		glSC(glDeleteShader(ID));
		Deleted = true;
	}
}
void Shader::Delete() {
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
	else this->~Shader();
}