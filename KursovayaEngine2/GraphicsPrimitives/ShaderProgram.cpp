#include"ShaderProgram.h"
#include"glad/glad.h"
#include"Tools/DebuggingTools.h"
#include<vector>
#include<string>
#include"Tools/GLDebug.h"

unsigned int ShaderProgramClass::gID() const {
#if defined Debug
	if (Deleted) {
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER PROGRAM IS ALREADY DELETED, ACCESSING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
		return 0;
	}
#endif
	return ID;
}
ShaderProgramClass::ShaderProgramClass() {
	glSC(ID = glCreateProgram());
}
ShaderProgramClass::ShaderProgramClass(const ShaderProgramClass* toCopy) {
	memcpy(this, toCopy, sizeof(ShaderProgramClass));
	toCopy->Deleted = true;
}
ShaderProgramClass::ShaderProgramClass(const ShaderProgramClass&& toCopy) {
	memcpy(this, &toCopy, sizeof(ShaderProgramClass));
	toCopy.Deleted = true;
}
void ShaderProgramClass::operator=(const ShaderProgramClass&& toCopy) {
	memcpy(this, &toCopy, sizeof(ShaderProgramClass));
	toCopy.Deleted = true;
}
void ShaderProgramClass::AttachShader(const ShaderClass& SH) {
#if defined Debug
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "YOU CANT ATTACH SHADER TO SHADER PROGRAM WHEN ITS DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
	else if (Linked) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "YOU CANT ATTACH SHADER TO SHADER PROGRAM WHEN ITS ALREADY LINKED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
	else 
#endif
		glSC(glAttachShader(ID, SH.gID()));
}
void ShaderProgramClass::LinkShaders() {
#if defined Debug
	if (Linked) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER PROGRAM IS ALREADY LINKED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
	else 
#endif
	{
		glSC(glLinkProgram(ID));
#if defined Debug
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
#endif
	}
}
ShaderProgramClass::~ShaderProgramClass() {
	if (not Deleted) {// no need for warning becouse destructor will be called at end of scope anyway
		glSC(glDeleteProgram(ID));
		Deleted = true;
	}
}
void ShaderProgramClass::Delete() {
#if defined Debug
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER PROGRAM IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
	else 
#endif
		this->~ShaderProgramClass();
}

void ShaderProgramClass::Bind() const {
#if defined Debug
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER PROGRAM IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
	else if (not Linked) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "SHADER PROGRAM IS NOT LINKED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION }  );
	else 
#endif
		glSC(glUseProgram(ID));
}
void ShaderProgramClass::Unbind() {
	glSC(glUseProgram(0));
}





#if defined Debug
#define uniformCOPYPASTE(funcName, ...)\
if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"SHADER PROGRAM IS DELETED, YOU CANT CHANGE ITS UNIFORM",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });\
if (not Linked) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"SHADER PROGRAM IS NOT LINKED, YOU CANT CHANGE ITS UNIFORM",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });\
else {\
	glSC(glUseProgram(ID));\
	int location = glGetUniformLocation(ID, name);\
	if (location == -1) {\
		std::string msg; msg+="UNIFORM CALLED \""; msg+=name; msg+="\" NOT FOUND IN SHADER PROGRAM";\
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,msg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });\
	}\
	else glSC(funcName(glGetUniformLocation(ID, name), __VA_ARGS__));\
}
#else
#define uniformCOPYPASTE(funcName, ...) glSC(funcName(glGetUniformLocation(ID, name), __VA_ARGS__));
#endif

void ShaderProgramClass::SetUniform1f(const char* name, float v0) { uniformCOPYPASTE(glUniform1f, v0); }
void ShaderProgramClass::SetUniform2f(const char* name, float v0, float v1) { uniformCOPYPASTE(glUniform2f, v0, v1); }
void ShaderProgramClass::SetUniform3f(const char* name, float v0, float v1, float v2) { uniformCOPYPASTE(glUniform3f, v0, v1, v2); }
void ShaderProgramClass::SetUniform4f(const char* name, float v0, float v1, float v2, float v3) { uniformCOPYPASTE(glUniform4f, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1i(const char* name, int v0) { uniformCOPYPASTE(glUniform1i, v0); }
void ShaderProgramClass::SetUniform2i(const char* name, int v0, int v1) { uniformCOPYPASTE(glUniform2i, v0, v1); }
void ShaderProgramClass::SetUniform3i(const char* name, int v0, int v1, int v2) { uniformCOPYPASTE(glUniform3i, v0, v1, v2); }
void ShaderProgramClass::SetUniform4i(const char* name, int v0, int v1, int v2, int v3) { uniformCOPYPASTE(glUniform4i, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1ui(const char* name, unsigned int v0) { uniformCOPYPASTE(glUniform1ui, v0); }
void ShaderProgramClass::SetUniform2ui(const char* name, unsigned int v0, unsigned int v1) { uniformCOPYPASTE(glUniform2ui, v0, v1); }
void ShaderProgramClass::SetUniform3ui(const char* name, unsigned int v0, unsigned int v1, unsigned int v2) { uniformCOPYPASTE(glUniform3ui, v0, v1, v2); }
void ShaderProgramClass::SetUniform4ui(const char* name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) { uniformCOPYPASTE(glUniform4ui, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform1fv, count, value); }
void ShaderProgramClass::SetUniform2fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform2fv, count, value); }
void ShaderProgramClass::SetUniform3fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform3fv, count, value); }
void ShaderProgramClass::SetUniform4fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform4fv, count, value); }
void ShaderProgramClass::SetUniform1iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform1iv, count, value); }
void ShaderProgramClass::SetUniform2iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform2iv, count, value); }
void ShaderProgramClass::SetUniform3iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform3iv, count, value); }
void ShaderProgramClass::SetUniform4iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform4iv, count, value); }
void ShaderProgramClass::SetUniform1uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform1uiv, count, value); }
void ShaderProgramClass::SetUniform2uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform2uiv, count, value); }
void ShaderProgramClass::SetUniform3uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform3uiv, count, value); }
void ShaderProgramClass::SetUniform4uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform4uiv, count, value); }
void ShaderProgramClass::SetUniformMatrix2fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix2x3fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2x3fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3x2fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3x2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix2x4fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2x4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4x2fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4x2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3x4fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3x4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4x3fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4x3fv, count, transpose, value); }


#undef uniformCOPYPASTE