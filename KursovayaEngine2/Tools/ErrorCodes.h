#pragma once


/*
about "IMPOSSIBLE" keyword, it means that somnething should not be possible
if program is working correctly, well somnething may technically be possible
but it can also make no sence to call some function or access some data
*/

/*all the errors that can be thrown by engine*/
enum class KURSAVAYAENGINE2_CORE_ERRORS : unsigned int {

	NONE = 0,
	
	/*
	means that called function failed for some reason, this function is not KursavayaEngine2's,
	this may be some function from library and etc
	*/
	FAILED_THIRD_PARTY_FUNCTION = 1,

	/*
	for some reason library failed to initialize
	*/
	FAILED_TO_INITIALIZE_LIBRARY = 2,

	/*
	data which is not supposed to be accessible at current state of instance or just impossible to access
	*/
	ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA = 3,

	/*
	class can have functions which can be unnecesarry to call since it may break program
	*/
	TRYING_TO_CALL_UNNECESARY_FUNCTION = 4,

	/*
	class can have functions which cant be called since some part of class is not ready,
	or class by itself is not ready for some reason
	*/
	TRYING_TO_CALL_IMPOSSIBLE_FUNCTION = 5,

	/*
	trivial, arguments are just invalid or makes not sense
	*/
	TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS = 6,

	/*
	there is no way to determine whats the problem
	*/
	UNKNOWN = 7,

	

};