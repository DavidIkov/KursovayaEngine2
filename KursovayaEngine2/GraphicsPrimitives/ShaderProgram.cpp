#include"ShaderProgram.h"
#include"glad/glad.h"
#include"Tools/DebuggingTools.h"
#include<vector>
#include<string>
#include"Tools/GLDebug.h"

unsigned int ShaderProgram::gID() const {
	if (Deleted) {
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER PROGRAM IS ALREADY DELETED, ACCESSING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
		return 0;
	}
	return ID;
}
ShaderProgram::ShaderProgram() {
	glSC(ID = glCreateProgram());
}
void ShaderProgram::AttachShader(const Shader& SH) {
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "YOU CANT ATTACH SHADER TO SHADER PROGRAM WHEN ITS DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
	else if (Linked) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "YOU CANT ATTACH SHADER TO SHADER PROGRAM WHEN ITS ALREADY LINKED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
	else glSC(glAttachShader(ID, SH.gID()));
}
void ShaderProgram::LinkShaders() {
	if (Linked) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER PROGRAM IS ALREADY LINKED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
	else {
		glSC(glLinkProgram(ID));
		Linked = true;
		{//check for linking
			int success;
			char info[512];
			glSC(glGetProgramiv(ID, GL_LINK_STATUS, &success));
			if (!success) {
				glSC(glGetProgramInfoLog(ID, 512, 0, info));
				std::string msg = "SHADERS LINKING ERROR: ";
				msg += info;
				DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, msg.c_str(), KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
			}
		}
	}
}
ShaderProgram::~ShaderProgram() {
	if (not Deleted) {// no need for warning becouse destructor will be called at end of scope anyway
		glSC(glDeleteProgram(ID));
		Deleted = true;
	}
}
void ShaderProgram::Delete() {
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER PROGRAM IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
	else this->~ShaderProgram();
}

void ShaderProgram::Bind() const {
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER PROGRAM IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
	else if (not Linked) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER PROGRAM IS NOT LINKED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION }  );
	else glSC(glUseProgram(ID));
}
void ShaderProgram::Unbind() {
	glSC(glUseProgram(0));
}





#define uniformCOPYPASTE(funcName, ...)\
if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"SHADER PROGRAM IS DELETED, YOU CANT CHANGE ITS UNIFORM",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });\
if (not Linked) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"SHADER PROGRAM IS NOT LINKED, YOU CANT CHANGE ITS UNIFORM",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });\
else {\
	glSC(glUseProgram(ID));\
	int location = glGetUniformLocation(ID, name);\
	if (location == -1) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"UNIFORM NAME NOT FOUND IN SHADER PROGRAM",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });\
	else glSC(funcName(glGetUniformLocation(ID, name), __VA_ARGS__));\
}

void ShaderProgram::SetUniform1f(const char* name, float v0) { uniformCOPYPASTE(glUniform1f, v0); }
void ShaderProgram::SetUniform2f(const char* name, float v0, float v1) { uniformCOPYPASTE(glUniform2f, v0, v1); }
void ShaderProgram::SetUniform3f(const char* name, float v0, float v1, float v2) { uniformCOPYPASTE(glUniform3f, v0, v1, v2); }
void ShaderProgram::SetUniform4f(const char* name, float v0, float v1, float v2, float v3) { uniformCOPYPASTE(glUniform4f, v0, v1, v2, v3); }
void ShaderProgram::SetUniform1i(const char* name, int v0) { uniformCOPYPASTE(glUniform1i, v0); }
void ShaderProgram::SetUniform2i(const char* name, int v0, int v1) { uniformCOPYPASTE(glUniform2i, v0, v1); }
void ShaderProgram::SetUniform3i(const char* name, int v0, int v1, int v2) { uniformCOPYPASTE(glUniform3i, v0, v1, v2); }
void ShaderProgram::SetUniform4i(const char* name, int v0, int v1, int v2, int v3) { uniformCOPYPASTE(glUniform4i, v0, v1, v2, v3); }
void ShaderProgram::SetUniform1ui(const char* name, unsigned int v0) { uniformCOPYPASTE(glUniform1ui, v0); }
void ShaderProgram::SetUniform2ui(const char* name, unsigned int v0, unsigned int v1) { uniformCOPYPASTE(glUniform2ui, v0, v1); }
void ShaderProgram::SetUniform3ui(const char* name, unsigned int v0, unsigned int v1, unsigned int v2) { uniformCOPYPASTE(glUniform3ui, v0, v1, v2); }
void ShaderProgram::SetUniform4ui(const char* name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) { uniformCOPYPASTE(glUniform4ui, v0, v1, v2, v3); }
void ShaderProgram::SetUniform1fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform1fv, count, value); }
void ShaderProgram::SetUniform2fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform2fv, count, value); }
void ShaderProgram::SetUniform3fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform3fv, count, value); }
void ShaderProgram::SetUniform4fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform4fv, count, value); }
void ShaderProgram::SetUniform1iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform1iv, count, value); }
void ShaderProgram::SetUniform2iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform2iv, count, value); }
void ShaderProgram::SetUniform3iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform3iv, count, value); }
void ShaderProgram::SetUniform4iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform4iv, count, value); }
void ShaderProgram::SetUniform1uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform1uiv, count, value); }
void ShaderProgram::SetUniform2uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform2uiv, count, value); }
void ShaderProgram::SetUniform3uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform3uiv, count, value); }
void ShaderProgram::SetUniform4uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform4uiv, count, value); }
void ShaderProgram::SetUniformMatrix2fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2fv, count, transpose, value); }
void ShaderProgram::SetUniformMatrix3fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3fv, count, transpose, value); }
void ShaderProgram::SetUniformMatrix4fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4fv, count, transpose, value); }
void ShaderProgram::SetUniformMatrix2x3fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2x3fv, count, transpose, value); }
void ShaderProgram::SetUniformMatrix3x2fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3x2fv, count, transpose, value); }
void ShaderProgram::SetUniformMatrix2x4fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2x4fv, count, transpose, value); }
void ShaderProgram::SetUniformMatrix4x2fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4x2fv, count, transpose, value); }
void ShaderProgram::SetUniformMatrix3x4fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3x4fv, count, transpose, value); }
void ShaderProgram::SetUniformMatrix4x3fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4x3fv, count, transpose, value); }


#undef uniformCOPYPASTE