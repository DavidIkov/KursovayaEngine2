#pragma once
#if defined DLL
#define DLLTREATMENT __declspec(dllexport)
#else
#define DLLTREATMENT __declspec(dllimport)
#endif