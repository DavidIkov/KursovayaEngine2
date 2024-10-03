#pragma once
#if defined KE2_DLL
#define DLLTREATMENT __declspec(dllexport)
#else
#define DLLTREATMENT __declspec(dllimport)
#endif