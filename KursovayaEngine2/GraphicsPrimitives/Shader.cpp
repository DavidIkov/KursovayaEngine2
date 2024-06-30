#include"Shader.h"
#include"glad/glad.h"
#include"Tools/DebuggingTools.h"
#include<string>
#include"Tools/GLDebug.h"
#include"Tools/ReadFromFile.h"

unsigned int Shader::gID() const {
	if (Deleted) {
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "SHADER IS ALREADY DELETED, ACCESSING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESING_DELETED_SHADER_DATA });
		return 0;
	}
	return ID;
}
Shader::Shader(ShaderTypesEnum typ, const char* filePath) {
	ShaderType = typ;
	ID = glCreateShader((typ == ShaderTypesEnum::Fragment) ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER);
	std::string scode = ReadFromFile(filePath);
	const char* code = scode.c_str();
	glSC(glShaderSource(ID, 1, &code, 0));
}
void Shader::Compile() {
	if (Compiled) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "SHADER IS ALREADY COMPILED", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_COMPILE_ALREADY_COMPILED_SHADER });
	else {
		glSC(glCompileShader(ID));
		Compiled = true;
		{//check for compilation
			int success;
			char info[512];
			glSC(glGetShaderiv(ID, GL_COMPILE_STATUS, &success));
			if (!success) {
				glSC(glGetShaderInfoLog(ID, 512, 0, info));
				std::string msg = (ShaderType == ShaderTypesEnum::Fragment) ? "FRAGMENT " : "VERTEX ";
				msg += "SHADER COMPILATION ERROR: ";
				msg += info;
				DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, msg.c_str(), KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_COMPILE_SHADER });
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
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "SHADER IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_DELETE_ALREADY_DELETED_SHADER });
	else this->~Shader();
}