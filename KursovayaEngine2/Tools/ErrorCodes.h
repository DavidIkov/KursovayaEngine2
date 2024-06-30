#pragma once

/*all the errors that can be thrown by engine*/
enum class KURSAVAYAENGINE2_CORE_ERRORS : unsigned int {
	


	/*glfw failed to create window for some reason*/
	FAILED_TO_CREATE_WINDOW = 0,

	/*glad failed to initialize for some reason*/
	FAILED_TO_INITIALIZE_GLAD = 1,

	/*shader failed to compile*/
	FAILED_TO_COMPILE_SHADER = 2,

	/*shader program failed to link*/
	FAILED_TO_LINK_SHADERS_IN_SHADER_PROGRAM = 3,

	/*stb_image failed to read file*/
	STB_IMAGE_FAILED_TO_READ_FILE = 4,

	/*you tried to access deleted shader's data*/
	ACCESING_DELETED_SHADER_DATA = 5,

	/*tried to compile already compiled shader*/
	ATTEMPING_TO_COMPILE_ALREADY_COMPILED_SHADER = 6,

	/*tried to delete already deleted shader*/
	ATTEMPING_TO_DELETE_ALREADY_DELETED_SHADER = 7,

	/*tried to access deleted shader program's data*/
	ACCESSING_DELETED_SHADER_PROGRAM_DATA = 8,

	/*tried to attach shader to linked shader program, you cant do that since it dosent make sense*/
	ATTEMPING_TO_ATTACH_SHADER_TO_ALREADY_LINKED_SHADER_PROGRAM = 9,

	/*tried to attach shader to deleted shader program*/
	ATTEMPING_TO_ATTACH_SHADER_TO_ALREADY_DELETED_SHADER_PROGRAM = 10,

	/*tried to link already linked shader... why?*/
	ATTEMPING_TO_LINK_ALREADY_LINKED_SHADER = 11,

	/*tried to delete already deleted shader program*/
	ATTEMPING_TO_DELETE_ALREADY_DELETED_SHADER_PROGRAM = 12,

	/**/
	ATTEMPING_TO_BIND_ALREADY_DELETED_SHADER_PROGRAM = 13,

	/**/
	ATTEMPING_TO_BIND_SHADER_PROGRAM_WHICH_IS_NOT_LINKED = 14,

	/**/
	ACCESSING_DELETED_TEXTURE_DATA = 15,

	/**/
	ATTEMPING_TO_DELETE_ALREADY_DELETED_TEXTURE = 16,

	/**/
	ATTEMPING_TO_BIND_ALREADY_DELETED_TEXTURE = 17,

	/**/
	ATTEMPING_TO_REMOVE_CONNECTION_FROM_EVENT_WITH_INVALID_KEY = 18,

	/**/
	ATTEMPING_TO_REMOVE_NON_EXISTANT_KEY_FROM_KEYS_SYSTEM = 19,

	/**/
	FAILED_TO_INITIALIZE_GLFW = 20,

	/**/
	FAILED_TO_OPEN_FILE = 21,

	/*when you create framebuffer it can be not fully set up*/
	FRAMEBUFFER_IS_NOT_COMPLETE = 22,

	/**/
	ATTEMPING_TO_DELETE_ALREADY_DELETED_FRAMEBUFFER = 23,

	/*you cant bind framebuffer which is incomplete, call "Finish" function to make it "complete"*/
	ATTEMPING_TO_BIND_INCOMPLETE_FRAMEBUFFER = 24,

	/**/
	ATTEMPING_TO_BIND_ALREADY_DELETED_FRAMEBUFFER = 25,

	/**/
	ATTEMPING_TO_CREATE_RENDERBUFFER_WITHOUT_ANY_BUFFERS = 26,

	/**/
	ATTEMPING_TO_DELETE_ALREADY_DELETED_RENDERBUFFER = 27,

	/**/
	ATTEMPING_TO_ATTACH_TO_FRAMEBUFFER_A_RENDERBUFFER_WHICH_DOSENT_HAVE_ANY_BUFFERS_ENABLED = 28,

	/*you cant create stencil buffer without depth buffer*/
	ATTEMPING_TO_CREATE_RENDERBUFFER_WITH_STENCIL_BUFFER_BUT_WITHOUT_DEPTH_BUFFER = 29,

	/*refer to error 29*/
	ATTEMPING_TO_ATTACH_TO_FRAMEBUFFER_A_RENDERBUFFER_WHICH_HAVE_STENCIL_BUFFER_BUT_HAVENT_GOT_DEPTH_BUFFER = 30,
};