#pragma once
#include<iostream>
class Vector2;
class Vector3;

//numeration is mirrored
class Matrix {
	unsigned int SizeX;
	unsigned int SizeY;
	float* Nums;

	mutable bool NeedToUpdateInversedMatrix = true;
	mutable Matrix* InversedMatrix;
	void UpdateInversedMatrix() const;

	mutable bool NeedToUpdateDeterminant = true;
	mutable float Determinant;
	void UpdateDeterminant() const;

public:
	unsigned int gSizeX() const;
	unsigned int gSizeY() const;

	float& operator[](unsigned int i) const;

	/*
	3d rotation standart: X:F,T; Y:R,F; Z:R,T
	X,Y,Z means the rotation axis, and 2 second letter represent local X and Y vectors for rotation in 2d space
	R-RightVec,T-TopVec,F-FrontVec
	*/

	static Matrix RotateIn3DByAnglesXYZ(float xr, float yr, float zr);
	static Matrix RotateIn3DByAnglesZYX(float xr, float yr, float zr);
	static Matrix RotateIn3DByAnglesYZX(float xr, float yr, float zr);
	static Matrix RotateIn3DByAnglesYXZ(float xr, float yr, float zr);
	static Matrix RotateIn3DByAnglesZXY(float xr, float yr, float zr);
	static Matrix RotateIn3DByAnglesXZY(float xr, float yr, float zr);

	static Matrix RotateIn2DByAngle(float angle);

	Matrix();
	Matrix(unsigned int sx, unsigned int sy, const float(&&nums)[]);
	Matrix(unsigned int sx, unsigned int sy, const float nums[]);
	Matrix(unsigned int sx, unsigned int sy);
	Matrix(unsigned int l, const Vector3*(&&nums)[]);
	Matrix(unsigned int l, const Vector2*(&&nums)[]);
	Matrix(const Matrix& mat);
	Matrix& operator=(const Matrix& mat);
	~Matrix();

	float gDeterminant() const;
	const Matrix& gInversedMatrix() const;

	bool IsIndInMatrix(unsigned int i) const;
	unsigned int GetIndByXY(unsigned int x, unsigned int y) const;
	bool IsXYInMatrix(unsigned int x, unsigned int y) const;
	Matrix operator+(const Matrix& mat) const;
	Matrix operator+(float num) const;
	Matrix operator-(const Matrix& mat) const;
	Matrix operator-(float num) const;

	Matrix operator*(const Matrix& mat) const;
	Matrix operator*(float num) const;

	Matrix operator*(const Vector2& vec) const;
	Matrix operator*(const Vector3& vec) const;
	
};