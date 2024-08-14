#pragma once
#include"DLL.h"
#include<math.h>
class Matrix;


class Vector2 {
	float Cords[2] = { 0.f,0.f };
	mutable bool newLen = true;
	mutable float Len = 0.f;
	mutable float SqLen = 0.f;
	mutable bool newUnit = true;
	mutable float UnitX = 0.f;
	mutable float UnitY = 0.f;
public:

	DLLTREATMENT static Vector2 atan(const Vector2& vec);
	DLLTREATMENT static void RotateVectors(const Vector2& xv, const Vector2& yv, float angle, Vector2& xvTOWRITE, Vector2& yvTOWRITE);

	DLLTREATMENT Vector2();
	DLLTREATMENT Vector2(float nX, float nY);
	DLLTREATMENT Vector2(float nAx);
	DLLTREATMENT Vector2(const Matrix&& mat);
	
	DLLTREATMENT Vector2& operator=(const Vector2& toCopy);

	DLLTREATMENT float& operator[](unsigned int ind);
	DLLTREATMENT float operator[](unsigned int ind) const;

	DLLTREATMENT float SqLength() const;
	DLLTREATMENT float Length() const;
	DLLTREATMENT Vector2 Unit() const;

	DLLTREATMENT float DotProduct(const Vector2& vec) const;

	DLLTREATMENT void sX(float nX);
	DLLTREATMENT float gX() const;
	DLLTREATMENT void sY(float nY);
	DLLTREATMENT float gY() const;


	DLLTREATMENT Vector2 operator+(float num) const;
	DLLTREATMENT Vector2 operator-(float num) const;
	DLLTREATMENT Vector2 operator*(float num) const;
	DLLTREATMENT Vector2 operator/(float num) const;

	DLLTREATMENT void operator+=(float num);
	DLLTREATMENT void operator-=(float num);
	DLLTREATMENT void operator*=(float num);
	DLLTREATMENT void operator/=(float num);

	DLLTREATMENT Vector2 operator+(const Vector2& vec) const;
	DLLTREATMENT Vector2 operator-(const Vector2& vec) const;
	DLLTREATMENT Vector2 operator*(const Vector2& vec) const;
	DLLTREATMENT Vector2 operator/(const Vector2& vec) const;

	DLLTREATMENT void operator+=(const Vector2& vec);
	DLLTREATMENT void operator-=(const Vector2& vec);
	DLLTREATMENT void operator*=(const Vector2& vec);
	DLLTREATMENT void operator/=(const Vector2& vec);

	DLLTREATMENT Vector2 operator+(const Matrix& mat) const;
	DLLTREATMENT Vector2 operator-(const Matrix& mat) const;
	DLLTREATMENT Vector2 operator*(const Matrix& mat) const;
	DLLTREATMENT Vector2 operator/(const Matrix& mat) const;

	DLLTREATMENT void operator+=(const Matrix& mat);
	DLLTREATMENT void operator-=(const Matrix& mat);
	DLLTREATMENT void operator*=(const Matrix& mat);
	DLLTREATMENT void operator/=(const Matrix& mat);
};

