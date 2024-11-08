#pragma once
#if defined KE2_DLL
#define DLLTREATMENT __declspec(dllexport)
#elif defined KE2_User
#define DLLTREATMENT __declspec(dllimport)
#else
#error no KE2_User or KE2_DLL is defined,
#endif