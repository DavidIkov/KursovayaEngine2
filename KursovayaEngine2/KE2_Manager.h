#pragma once
#include"DLL.h"
#include"Tools/ErrorsSystem.h"

#if !defined KE2_DLL
#if !defined KE2_User
#error you need to define a global KE2_User
#endif
#endif
namespace KE2::Manager {
	struct ErrorsEnumWrapperStruct :ErrorsSystemNamespace::ErrorBase {
		enum ErrorsEnum {
			FailedToInitializeGLFW,
			KE2_InDebugButUserInEfficiency,
			KE2_InEfficiencyButUserInDebug,
			KE2_In_x64_ButUserIn_x86,
			KE2_In_x86_ButUserIn_x64,
			KE2_OnWindowsButUserOnLinux,
			KE2_OnLinuxButUserOnWindows,
		}; ErrorsEnum Error;
		inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
	}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;



	class KE2_LibraryStruct {

		DLLTREATMENT static constexpr bool _IsKE2_InDebugConfig() noexcept;
		DLLTREATMENT static constexpr bool _IsKE2_InEfficiencyConfig() noexcept;
		DLLTREATMENT static constexpr bool _IsKE2_In_x86_Architecture() noexcept;
		DLLTREATMENT static constexpr bool _IsKE2_In_x64_Architecture() noexcept;
		DLLTREATMENT static constexpr bool _IsKE2_OnLinuxPlatform() noexcept;
		DLLTREATMENT static constexpr bool _IsKE2_OnWindowsPlatform() noexcept;

		DLLTREATMENT static void _InitializeKE2();
		DLLTREATMENT static void _UninitializeKE2();
		
	public:
#if defined KE2_User
		inline KE2_LibraryStruct() {


			//checking that configs are same in project and a dll
#if !defined KE2_Debug and !defined KE2_Efficiency
#error you need to globally define KE2_Debug or KE2_Efficiency
#elif defined KE2_Debug and defined KE2_Efficiency
#error you have to define only one macro out of KE2_Debug and KE2_Efficiency
#elif defined KE2_Debug
			if (not _IsKE2_InDebugConfig())
				ErrorsSystemNamespace::SendError << "user is using KE2 in debug config but dll is in efficiency config" >> ErrorsEnumWrapperStruct(ErrorsEnum::KE2_InDebugButUserInEfficiency);
#elif defined KE2_Efficiency
			if (not _IsKE2_InEfficiencyConfig())
				ErrorsSystemNamespace::SendError << "user is using KE2 in efficiency config but dll is in debug config" >> ErrorsEnumWrapperStruct(ErrorsEnum::KE2_InEfficiencyButUserInDebug);
#endif

			//checking that architectures are same in project and a dll
#if !defined KE2_x64 and !defined KE2_x86
#error you need to globally define KE2_x64 or KE2_x86
#elif defined KE2_x64 and defined KE2_x86
#error you have to define only one macro out of KE2_x64 and KE2_x86
#elif defined KE2_x64
			if (not _IsKE2_In_x64_Architecture())
				ErrorsSystemNamespace::SendError << "project is using KE2 with x64 architecture but dll is using x86 architecture" >> ErrorsEnumWrapperStruct(ErrorsEnum::KE2_In_x64_ButUserIn_x86);
#elif defined KE2_x86
			if (not _IsKE2_In_x86_Architecture())
				ErrorsSystemNamespace::SendError << "project is using KE2 with x86 architecture but dll is using x64 architecture" >> ErrorsEnumWrapperStruct(ErrorsEnum::KE2_In_x86_ButUserIn_x64);
#endif

			//checking that platforms are same in project and a dll
#if !defined KE2_Windows and !defined KE2_Linux
#error you need to globally define KE2_Windows or KE2_Linux
#elif defined KE2_Windows  and defined KE2_Linux
#error you have to define only one macro out of KE2_Windows and KE2_Linux
#elif defined KE2_Windows
			if (not _IsKE2_OnWindowsPlatform())
				ErrorsSystemNamespace::SendError << "project is using KE2 for Windows but dll is for Linux" >> ErrorsEnumWrapperStruct(ErrorsEnum::KE2_OnWindowsButUserOnLinux);
#elif defined KE2_Linux
			if (not _IsKE2_OnLinuxPlatform())
				ErrorsSystemNamespace::SendError << "project is using KE2 for Linux but dll is for Windows" >> ErrorsEnumWrapperStruct(ErrorsEnum::KE2_OnLinuxButUserOnWindows);
#endif

			_InitializeKE2();

		}
		inline ~KE2_LibraryStruct() {
			_UninitializeKE2();
		}
#endif
	};
}
