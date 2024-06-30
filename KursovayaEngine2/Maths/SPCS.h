#pragma once
#include"Vector2.h"

///stands for SCALE or PIXEL COORDINATE SYSTEM
class SPCS {
	Vector2 Scale;
	Vector2 Pixel;
public:
	SPCS();
	SPCS(float nsX, float nsY, float npX, float npY);

	const Vector2& gSV() const;
	const Vector2& gPV() const;

	void sSX(float nsX);
	void sSY(float nsY);
	void sPX(float npX);
	void sPY(float npY);

	float gSX() const;
	float gSY() const;
	float gPX() const;
	float gPY() const;

	SPCS operator+(const SPCS& cords) const;
	SPCS operator-(const SPCS& cords) const;
};