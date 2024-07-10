#pragma once
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

	static void RotateVectors(const Vector3& xv, const Vector3& yv, float angle, Vector3& xvTOWRITE, Vector3& yvTOWRITE);

	Vector3();
	Vector3(float nX, float nY, float nZ);
	Vector3(float nn);
	Vector3(const Matrix&& mat);
	
	Vector3& operator=(const Vector3& toCopy);

	float& operator[](unsigned int ind);
	float operator[](unsigned int ind) const;

	float SqLength() const;
	float Length() const;
	Vector3 Unit() const;

	Vector3 Cross(const Vector3& vec) const;

	void sX(float nX);
	float gX() const;
	void sY(float nY);
	float gY() const;
	void sZ(float nZ);
	float gZ() const;

	Vector2 gvXY() const;
	Vector2 gvYX() const;
	Vector2 gvXZ() const;
	Vector2 gvZX() const;
	Vector2 gvYZ() const;
	Vector2 gvZY() const;


	Vector3 operator+(float num) const;
	Vector3 operator-(float num) const;
	Vector3 operator*(float num) const;
	Vector3 operator/(float num) const;

	void operator+=(float num);
	void operator-=(float num);
	void operator*=(float num);
	void operator/=(float num);

	Vector3 operator+(const Vector3& vec) const;
	Vector3 operator-(const Vector3& vec) const;
	Vector3 operator*(const Vector3& vec) const;
	Vector3 operator/(const Vector3& vec) const;

	void operator+=(const Vector3& vec);
	void operator-=(const Vector3& vec);
	void operator*=(const Vector3& vec);
	void operator/=(const Vector3& vec);

	Vector3 operator+(const Matrix& mat) const;
	Vector3 operator-(const Matrix& mat) const;
	Vector3 operator*(const Matrix& mat) const;
	Vector3 operator/(const Matrix& mat) const;

	void operator+=(const Matrix& mat);
	void operator-=(const Matrix& mat);
	void operator*=(const Matrix& mat);
	void operator/=(const Matrix& mat);
};

