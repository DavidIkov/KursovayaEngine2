#pragma once
#include"DLL.h"
#include<vector>
#include<string>
//will return array of floats, first 3 will be position and another 3 will be normal
DLLTREATMENT std::vector<float> ReadObjFileType(const wchar_t* filePath);