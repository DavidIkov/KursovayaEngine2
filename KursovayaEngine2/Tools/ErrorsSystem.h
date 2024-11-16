#pragma once
#include"DLL.h"
#include<string>
#include"UsefullMacros/ApplyMacroInLoop.h"


namespace KE2::ErrorsSystemNamespace {

	inline bool IgnoreWarnings = false;
	inline bool DebugBreakOnWarnings =
#if defined KE2_Debug
		true;
#else
		false;
#endif

	inline bool DebugBreakOnErrors = true;

	inline std::string LastErrorMessage;
	inline std::string LastWarningMessage;

	class _MessageCreatingClass {
	protected:
		std::string Msg;
	public:
		DLLTREATMENT _MessageCreatingClass& operator<<(const char* msg);
		DLLTREATMENT _MessageCreatingClass& operator<<(const std::string& msg);
#ifdef KE2_Windows
		DLLTREATMENT _MessageCreatingClass& operator<<(const wchar_t* msg);
		DLLTREATMENT _MessageCreatingClass& operator<<(const std::wstring& msg);
#endif
	};

	struct _EndOfWarningStruct {};
	inline _EndOfWarningStruct EndOfWarning;
	class _SendWarningClass :public _MessageCreatingClass {
	public:
		template<typename MsgT>
		_SendWarningClass& operator<<(MsgT&& msg) { _MessageCreatingClass::operator<<(msg); return *this; }

		DLLTREATMENT void operator>>(_EndOfWarningStruct);
	};
	inline _SendWarningClass SendWarning;


	//used to catch any KE2 errors by reference, not depending on actual enum type
	struct ErrorBase {};
	typedef ErrorBase AnyError;
	class _SendErrorClass :public _MessageCreatingClass {
		DLLTREATMENT void _ProcessError_NoThrow();
	public:
		template<typename MsgT>
		_SendErrorClass& operator<<(MsgT&& msg) { _MessageCreatingClass::operator<<(msg); return *this; }
		template<typename ErrWrapperTyp>
		void operator>>(ErrWrapperTyp errWrapper) {
			static_assert(std::is_base_of_v<ErrorBase, ErrWrapperTyp>, "error struct should be publicly derived from ErrorBase");
			_ProcessError_NoThrow();
			throw errWrapper;
		}
	};
	class _RethrowErrorClass {
	public:
		template<typename ErrWrapperTyp>
		void operator<<(ErrWrapperTyp errWrapper) {
			static_assert(std::is_base_of_v<ErrorBase, ErrWrapperTyp>, "error struct should be publicly derived from ErrorBase");
			throw errWrapper;
		}
	};
	inline _SendErrorClass SendError;
	inline _RethrowErrorClass RethrowError;


}

/*
//variables represented as (int,k)(char,m)(bool,g) etc
#define KE2_ErrorsSystem_CreateErrorWrapperStruct_Macro(wrapperName,variables) struct 

///... is actual errors names
#define KE2_ErrorsSystem_CreateErrorsEnum_Macro(EnumName,...) struct EnumName##WrapperStruct : KE2::ErrorsSystemNamespace::ErrorBase{\
enum _ErrorsEnum{ __VA_ARGS__ }; _ErrorsEnum Err; inline EnumName##WrapperStruct(_ErrorsEnum errEnum): Err(errEnum) {}; };\
typedef EnumName##WrapperStruct EnumName;
*/