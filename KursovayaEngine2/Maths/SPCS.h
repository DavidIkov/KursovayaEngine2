#pragma once
#include"DLL.h"
#include"Vector2.h"

///stands for SCALE or PIXEL COORDINATE SYSTEM
class SPCS {
	Vector2 Scale;
	Vector2 Pixel;
public:
	DLLTREATMENT SPCS();
	DLLTREATMENT SPCS(float nsX, float nsY, float npX, float npY);

	DLLTREATMENT const Vector2& gSV() const;
	DLLTREATMENT const Vector2& gPV() const;

	DLLTREATMENT void sSX(float nsX);
	DLLTREATMENT void sSY(float nsY);
	DLLTREATMENT void sPX(float npX);
	DLLTREATMENT void sPY(float npY);

	DLLTREATMENT float gSX() const;
	DLLTREATMENT float gSY() const;
	DLLTREATMENT float gPX() const;
	DLLTREATMENT float gPY() const;

	DLLTREATMENT SPCS operator+(const SPCS& cords) const;
	DLLTREATMENT SPCS operator-(const SPCS& cords) const;
};