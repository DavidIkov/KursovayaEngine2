#pragma once
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

	static Vector2 atan(const Vector2& vec);
	static void RotateVectors(const Vector2& xv, const Vector2& yv, float angle, Vector2& xvTOWRITE, Vector2& yvTOWRITE);

	Vector2();
	Vector2(float nX, float nY);
	Vector2(float nAx);
	Vector2(const Matrix&& mat);
	
	Vector2& operator=(const Vector2& toCopy);

	float& operator[](unsigned int ind);
	float operator[](unsigned int ind) const;

	float SqLength() const;
	float Length() const;
	Vector2 Unit() const;

	float DotProduct(const Vector2& vec) const;

	void sX(float nX);
	float gX() const;
	void sY(float nY);
	float gY() const;


	Vector2 operator+(float num) const;
	Vector2 operator-(float num) const;
	Vector2 operator*(float num) const;
	Vector2 operator/(float num) const;

	void operator+=(float num);
	void operator-=(float num);
	void operator*=(float num);
	void operator/=(float num);

	Vector2 operator+(const Vector2& vec) const;
	Vector2 operator-(const Vector2& vec) const;
	Vector2 operator*(const Vector2& vec) const;
	Vector2 operator/(const Vector2& vec) const;

	void operator+=(const Vector2& vec);
	void operator-=(const Vector2& vec);
	void operator*=(const Vector2& vec);
	void operator/=(const Vector2& vec);

	Vector2 operator+(const Matrix& mat) const;
	Vector2 operator-(const Matrix& mat) const;
	Vector2 operator*(const Matrix& mat) const;
	Vector2 operator/(const Matrix& mat) const;

	void operator+=(const Matrix& mat);
	void operator-=(const Matrix& mat);
	void operator*=(const Matrix& mat);
	void operator/=(const Matrix& mat);
};

