#pragma once
#include"DLL.h"
#include<math.h>
class Matrix;
class Vector2;


class Vector3 {
	float Cords[3] = { 0.f,0.f,0.f };
	mutable bool newLen = true;
	mutable float Len = 0.f;
	mutable float SqLen = 0.f;
	mutable bool newUnit = true;
	mutable float UnitX = 0.f;
	mutable float UnitY = 0.f;
	mutable float UnitZ = 0.f;
public:

	DLLTREATMENT static void RotateVectors(const Vector3& xv, const Vector3& yv, float angle, Vector3& xvTOWRITE, Vector3& yvTOWRITE);

	DLLTREATMENT Vector3();
	DLLTREATMENT Vector3(float nX, float nY, float nZ);
	DLLTREATMENT Vector3(float nn);
	DLLTREATMENT Vector3(const Matrix&& mat);
	
	DLLTREATMENT Vector3& operator=(const Vector3& toCopy);

	DLLTREATMENT float& operator[](unsigned int ind);
	DLLTREATMENT float operator[](unsigned int ind) const;

	DLLTREATMENT float SqLength() const;
	DLLTREATMENT float Length() const;
	DLLTREATMENT Vector3 Unit() const;

	DLLTREATMENT Vector3 Cross(const Vector3& vec) const;

	DLLTREATMENT void sX(float nX);
	DLLTREATMENT float gX() const;
	DLLTREATMENT void sY(float nY);
	DLLTREATMENT float gY() const;
	DLLTREATMENT void sZ(float nZ);
	DLLTREATMENT float gZ() const;

	DLLTREATMENT Vector2 gvXY() const;
	DLLTREATMENT Vector2 gvYX() const;
	DLLTREATMENT Vector2 gvXZ() const;
	DLLTREATMENT Vector2 gvZX() const;
	DLLTREATMENT Vector2 gvYZ() const;
	DLLTREATMENT Vector2 gvZY() const;


	DLLTREATMENT Vector3 operator+(float num) const;
	DLLTREATMENT Vector3 operator-(float num) const;
	DLLTREATMENT Vector3 operator*(float num) const;
	DLLTREATMENT Vector3 operator/(float num) const;

	DLLTREATMENT void operator+=(float num);
	DLLTREATMENT void operator-=(float num);
	DLLTREATMENT void operator*=(float num);
	DLLTREATMENT void operator/=(float num);

	DLLTREATMENT Vector3 operator+(const Vector3& vec) const;
	DLLTREATMENT Vector3 operator-(const Vector3& vec) const;
	DLLTREATMENT Vector3 operator*(const Vector3& vec) const;
	DLLTREATMENT Vector3 operator/(const Vector3& vec) const;

	DLLTREATMENT void operator+=(const Vector3& vec);
	DLLTREATMENT void operator-=(const Vector3& vec);
	DLLTREATMENT void operator*=(const Vector3& vec);
	DLLTREATMENT void operator/=(const Vector3& vec);

	DLLTREATMENT Vector3 operator+(const Matrix& mat) const;
	DLLTREATMENT Vector3 operator-(const Matrix& mat) const;
	DLLTREATMENT Vector3 operator*(const Matrix& mat) const;
	DLLTREATMENT Vector3 operator/(const Matrix& mat) const;

	DLLTREATMENT void operator+=(const Matrix& mat);
	DLLTREATMENT void operator-=(const Matrix& mat);
	DLLTREATMENT void operator*=(const Matrix& mat);
	DLLTREATMENT void operator/=(const Matrix& mat);
};

