#include"ShaderProgram.h"
#include"glad/glad.h"
#include"Tools/DebuggingTools.h"
#include<vector>
#include<string>
#include"Tools/GLDebug.h"

unsigned int ShaderProgram::gID() const {
	if (Deleted) {
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "SHADER PROGRAM IS ALREADY DELETED, ACCESSING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_DELETED_SHADER_PROGRAM_DATA });
		return 0;
	}
	return ID;
}
ShaderProgram::ShaderProgram() {
	glSC(ID = glCreateProgram());
}
void ShaderProgram::AttachShader(const Shader& SH) {
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "YOU CANT ATTACH SHADER TO SHADER PROGRAM WHEN ITS DELETED", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_ATTACH_SHADER_TO_ALREADY_DELETED_SHADER_PROGRAM });
	else if (Linked) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "YOU CANT ATTACH SHADER TO SHADER PROGRAM WHEN ITS ALREADY LINKED", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_ATTACH_SHADER_TO_ALREADY_LINKED_SHADER_PROGRAM });
	else glSC(glAttachShader(ID, SH.gID()));
}
void ShaderProgram::LinkShaders() {
	if (Linked) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "SHADER PROGRAM IS ALREADY LINKED", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_LINK_ALREADY_LINKED_SHADER });
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
				DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, msg.c_str(), KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_LINK_SHADERS_IN_SHADER_PROGRAM });
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
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "SHADER PROGRAM IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_DELETE_ALREADY_DELETED_SHADER_PROGRAM });
	else this->~ShaderProgram();
}

void ShaderProgram::Bind() const {
	if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "SHADER PROGRAM IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_BIND_ALREADY_DELETED_SHADER_PROGRAM });
	else if (not Linked) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "SHADER PROGRAM IS NOT LINKED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_BIND_SHADER_PROGRAM_WHICH_IS_NOT_LINKED }  );
	else glSC(glUseProgram(ID));
}
void ShaderProgram::Unbind() {
	glSC(glUseProgram(0));
}