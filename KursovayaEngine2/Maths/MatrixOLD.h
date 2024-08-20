#pragma once
#include"DLL.h"
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
	DLLTREATMENT unsigned int gSizeX() const;
	DLLTREATMENT unsigned int gSizeY() const;

	DLLTREATMENT float& operator[](unsigned int i) const;

	/*
	3d rotation standart: X:F,T; Y:R,F; Z:R,T
	X,Y,Z means the rotation axis, and 2 second letter represent local X and Y vectors for rotation in 2d space
	R-RightVec,T-TopVec,F-FrontVec
	*/

	DLLTREATMENT static Matrix RotateIn3DByAnglesXYZ(float xr, float yr, float zr);
	DLLTREATMENT static Matrix RotateIn3DByAnglesZYX(float xr, float yr, float zr);
	DLLTREATMENT static Matrix RotateIn3DByAnglesYZX(float xr, float yr, float zr);
	DLLTREATMENT static Matrix RotateIn3DByAnglesYXZ(float xr, float yr, float zr);
	DLLTREATMENT static Matrix RotateIn3DByAnglesZXY(float xr, float yr, float zr);
	DLLTREATMENT static Matrix RotateIn3DByAnglesXZY(float xr, float yr, float zr);

	DLLTREATMENT static Matrix RotateIn2DByAngle(float angle);

	DLLTREATMENT Matrix();
	DLLTREATMENT Matrix(unsigned int sx, unsigned int sy, const float(&&nums)[]);
	DLLTREATMENT Matrix(unsigned int sx, unsigned int sy, const float nums[]);
	DLLTREATMENT Matrix(unsigned int sx, unsigned int sy);
	DLLTREATMENT Matrix(unsigned int l, const Vector3*(&&nums)[]);
	DLLTREATMENT Matrix(unsigned int l, const Vector2*(&&nums)[]);
	DLLTREATMENT Matrix(const Matrix& mat);
	DLLTREATMENT Matrix& operator=(const Matrix& mat);
	DLLTREATMENT ~Matrix();

	DLLTREATMENT float gDeterminant() const;
	DLLTREATMENT const Matrix& gInversedMatrix() const;

	DLLTREATMENT bool IsIndInMatrix(unsigned int i) const;
	DLLTREATMENT unsigned int GetIndByXY(unsigned int x, unsigned int y) const;
	DLLTREATMENT bool IsXYInMatrix(unsigned int x, unsigned int y) const;
	DLLTREATMENT Matrix operator+(const Matrix& mat) const;
	DLLTREATMENT Matrix operator+(float num) const;
	DLLTREATMENT Matrix operator-(const Matrix& mat) const;
	DLLTREATMENT Matrix operator-(float num) const;

	DLLTREATMENT Matrix operator*(const Matrix& mat) const;
	DLLTREATMENT Matrix operator*(float num) const;

	DLLTREATMENT Matrix operator*(const Vector2& vec) const;
	DLLTREATMENT Matrix operator*(const Vector3& vec) const;
	
};