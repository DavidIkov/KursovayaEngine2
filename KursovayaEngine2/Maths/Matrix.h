#pragma once
#include"Vector.h"

/*
3d rotation standart: X:F,T; Y:R,F; Z:R,T
X,Y,Z means the rotation axis, and 2 second letter represent local X and Y vectors for rotation in 2d space
R-RightVec,T-TopVec,F-FrontVec
*/

template<unsigned int SizeX, unsigned int SizeY>
class Matrix {

	static constexpr unsigned int RotationStandart3D[6] = { 2,1,0,2,0,1 };

	float Nums[SizeX * SizeY] = { 0.f };

	void ConstructFromNums(const float num) {
		Nums[SizeX * SizeY - 1] = num;
	}
	template<typename...otherNumsTyp>
	void ConstructFromNums(const float num, const otherNumsTyp...otherNums) {
		Nums[SizeX * SizeY - sizeof...(otherNums) - 1] = num;
		ConstructFromNums(otherNums...);
	}

	template<unsigned int vecLen>
	void ConstructFromVecs(const Vector<vecLen>& vec) {
		memcpy(&Nums[SizeY * (SizeX - 1)], &vec[0], sizeof(float) * vecLen);
	}
	template<unsigned int vecLen, typename...otherVecsTyp>
	void ConstructFromVecs(const Vector<vecLen>& vec, const otherVecsTyp...otherVecs) {
		memcpy(&Nums[SizeY * (SizeX - sizeof...(otherVecs) - 1)], &vec[0], sizeof(float) * vecLen);
		ConstructFromVecs<vecLen>(otherVecs...);
	}
public:
	template<unsigned int SizeX2, unsigned int SizeY2>
	friend class Matrix;

	template<typename...NumsTyp>
	Matrix(const float num, const NumsTyp...nums) {
		static_assert(sizeof...(nums) == (SizeX * SizeY - 1), "Too many numbers");
		ConstructFromNums(num, nums...);
	}
	//all vectors should be same length
	template<unsigned int vecLen, typename...VecsTyp>
	Matrix(const Vector<vecLen>& vec, const VecsTyp...vecs) {
		static_assert(vecLen == SizeY, "Vector length is not the same as SizeY, cant construct matrix out of it");
		ConstructFromVecs<vecLen>(vec, vecs...);
	}
	Matrix(const float num) {
		for (unsigned int i = 0; i < SizeX * SizeY; i++) Nums[i] = num;
	}
	Matrix(const float* numsStart) {
		memcpy(&Nums[0], numsStart, sizeof(float) * SizeX * SizeY);
	}
	Matrix() {

	}

	Matrix(const Matrix<SizeX, SizeY>& matToCopy) {
		memcpy(this, &matToCopy, sizeof(matToCopy));
	}
	Matrix(Matrix<SizeX, SizeY>&& matToCopy) {
		memcpy(this, &matToCopy, sizeof(matToCopy));
	}
	Matrix<SizeX, SizeY>& operator=(const Matrix<SizeX, SizeY>& matToCopy) {
		memcpy(this, &matToCopy, sizeof(matToCopy));
		return *this;
	}

	float& operator[](const unsigned int ind) {
		return Nums[ind];
	}
	const float& operator[](const unsigned int ind) const {
		return Nums[ind];
	}

	float GetDeterminant() const {

		static_assert(SizeX == SizeY, "Determinant is only defined for square matrix");

		if constexpr (SizeX == 1) {
			return Nums[0];
		}
		else {

			float* tempNums = new float[(SizeX - 1) * (SizeY - 1)];
			int multiplier = 1;
			float det = 0;
			for (unsigned int ai = 0; ai < SizeX; ai++) {
				for (unsigned int x = 0; x < SizeX; x++) {
					if (x == ai) continue;
					for (unsigned int y = 1; y < SizeY; y++) {
						unsigned int insertInd = y - 1 + (SizeY - 1) * (x > ai ? (x - 1) : x);
						tempNums[insertInd] = Nums[y + x * SizeY];
					}
				}
				det += Nums[ai * SizeY] * multiplier * Matrix<SizeX - 1, SizeX - 1>(&tempNums[0]).GetDeterminant();
				multiplier = -multiplier;
			}
			delete[] tempNums;
			return det;
		}
	}


	Matrix<SizeX, SizeY> GetInversedMatrix(float det) const {

		static_assert(SizeX == SizeY, "Inversed matrix is only defined for square matrix becouse of determinant");

		Matrix<SizeX, SizeY> retMat(0.f);

		float tempNums[(SizeX - 1) * (SizeY - 1)] = { 0 };
		for (unsigned int x = 0; x < SizeX; x++) {
			for (unsigned int y = 0; y < SizeY; y++) {
				for (unsigned int lx = 0; lx < SizeX; lx++) {
					if (lx == y) continue;
					for (unsigned int ly = 0; ly < SizeY; ly++) {
						if (ly == x) continue;
						tempNums[((lx > y) ? (lx - 1) : lx) * (SizeX - 1) + ((ly > x) ? (ly - 1) : ly)] = Nums[ly + lx * SizeY];
					}
				}
				int multipler = ((x + y) % 2 == 0) ? 1 : -1;
				retMat.Nums[y + x * SizeY] = multipler * Matrix<SizeX - 1, SizeY - 1>(tempNums).GetDeterminant() / det;
				multipler = -multipler;
			}
		}

		return retMat;
	}

	//used to normalize whole matrix, since after some rotations matrix can get a little broken becouse of floating point precision error
	void Normalize() {
		for (unsigned int x = 0; x < SizeX; x++) {
			Vector<SizeY> v(&Nums[x * SizeY]);
			v = v.Normalize();
			memcpy(&Nums[x * SizeY], &v[0], sizeof(float) * SizeY);
		}
	}

	//used to fix the problem when matrix becomes not Cartesian after some rotations
	//no operations regarding length of vectors will be done!
	//axis are defined as folows X=0,Y=1,Z=2
	//the way cross fix works is defined by order of axis, first axis WONT change,
	//the cross will be taken between axis1 and axis2 so this will be axis3,
	//and after that axis2 will be a cross product between axis1 and axis3
	template<unsigned int axis1, unsigned int axis2>
	void CrossFix3D() {

		static_assert(SizeX == 3 and SizeY == 3, "Matrix is not 3x3, cross fix is not defined");
		static_assert(axis1 != axis2, "You cant have repeating axis rotation order");
		static_assert(axis1 >= 0 and axis1 <= 3 and axis2 >= 0 and axis2 <= 3, "Incorrect axis set up");

		Vector<3> vecs[3] = { {&Nums[axis1 * 3]},{&Nums[axis2 * 3]},{0.f} };

		vecs[2] = vecs[0].CrossFL(vecs[1]);
		vecs[1] = vecs[2].CrossFL(vecs[0]);

		for (unsigned int x = 0; x < 3; x++) memcpy(&Nums[x * 3], &vecs[x][0], sizeof(float) * 3);
	}

	//all vectors are supposed to be length of 1
	//works only for Cartesian coordinates system
	template<unsigned int axisInd>
	float GetLocalCordForAxisC(const Vector<SizeY>& vec) const {

		static_assert(axisInd >= 0 and axisInd < SizeX, "Invalid axis");

		return Vector<SizeY>(&Nums[SizeY * axisInd]).DotFL(vec);
	}

	//cant be used for just 1 axis at a time, have to deal with whole matrix, this will work with any gived matrix space but it comes at a cost...
	const Vector<SizeX> GetLocalCordsForAxisU(const Vector<SizeY>& vec) const {

		static_assert(SizeX == SizeY, "Getting local cords when matrix is not a square is not defined");

		return (Vector<SizeX>)(GetInversedMatrix() * vec);

	}

	//"C" stands for Cartesian coordinates system
	template<unsigned int axisX, unsigned int axisY>
	Vector<SizeY> RotateVectorC(const Vector<SizeY>& vec, const float angle) const {

		static_assert(axisX < SizeX and axisX >= 0 and axisY < SizeX and axisY >= 0, "Invalid axis indexes");
		static_assert(axisX != axisY, "Axes cant be same");

		float vecLen = vec.Length();

		float localCordsX = GetLocalCordForAxisC<axisX>(vec) / vecLen;
		if (localCordsX < -1) localCordsX = -1; else if (localCordsX > 1) localCordsX = 1;
		float localCordsY = GetLocalCordForAxisC<axisY>(vec) / vecLen;

		float angleToX = acosf(localCordsX);
		//check if angle is negative
		if (localCordsY < 0) angleToX = -angleToX;

		angleToX += angle;

		Vector<SizeY> xv(&Nums[SizeY * axisX]); Vector<SizeY> yv(&Nums[SizeY * axisY]);

		return (xv * cosf(angleToX) + yv * sinf(angleToX)) * vecLen;
	}

	//axis1,axis2,axis3 are representing the order of rotation, X=0,Y=1,Z=2, so for example rotation by XYZ will be 0,1,2
	//will work only for Cartesian coordinate system(the "C" at end means Cartesian), it means that angle between all axes is pi/2
	//axis vectors are supposed to be length of 1
	template<unsigned int axis1, unsigned int axis2, unsigned int axis3>
	Matrix<SizeX, SizeY> RotateIn3DByAnglesC(const float xr, const float yr, const float zr) const {

		static_assert(SizeX == 3 and SizeY == 3, "Matrix is not 3x3, rotation is not defined");
		static_assert(axis1 != axis2 and axis1 != axis3 and axis2 != axis3, "You cant have repeating axis rotation order");
		static_assert(axis1 >= 0 and axis1 <= 3 and axis2 >= 0 and axis2 <= 3 and axis3 >= 0 and axis3 <= 3, "Incorrect axis set up");

		Matrix<SizeX, SizeY> retMat(*this);

		const float rots[3] = { xr,yr,zr };
		constexpr unsigned int order[3] = { axis1,axis2,axis3 };

#define rotVecMacr(i){\
		constexpr unsigned int xvi = RotationStandart3D[order[i] * 2]; constexpr unsigned int yvi = RotationStandart3D[order[i] * 2 + 1];\
		Vector<3> nxv = retMat.RotateVectorC<xvi, yvi>(Vector<3>(&retMat.Nums[3 * xvi]), rots[order[i]]);\
		Vector<3> nyv = retMat.RotateVectorC<xvi, yvi>(Vector<3>(&retMat.Nums[3 * yvi]), rots[order[i]]);\
		memcpy(&retMat.Nums[xvi * SizeX], &nxv[0], sizeof(float) * 3);\
		memcpy(&retMat.Nums[yvi * SizeX], &nyv[0], sizeof(float) * 3);}

		/*rotVecMacr(0);
		rotVecMacr(1);
		rotVecMacr(2);*/
		{
			constexpr unsigned int xvi = RotationStandart3D[order[0] * 2]; constexpr unsigned int yvi = RotationStandart3D[order[0] * 2 + 1];
			Vector<3> nxv = retMat.RotateVectorC<xvi, yvi>(Vector<3>(&retMat.Nums[3 * xvi]), rots[order[0]]);
			Vector<3> nyv = retMat.RotateVectorC<xvi, yvi>(Vector<3>(&retMat.Nums[3 * yvi]), rots[order[0]]);
			memcpy(&retMat.Nums[xvi * SizeX], &nxv[0], sizeof(float) * 3);
			memcpy(&retMat.Nums[yvi * SizeX], &nyv[0], sizeof(float) * 3);
		}

		{
			constexpr unsigned int xvi = RotationStandart3D[order[1] * 2]; constexpr unsigned int yvi = RotationStandart3D[order[1] * 2 + 1];
			Vector<3> nxv = retMat.RotateVectorC<xvi, yvi>(Vector<3>(&retMat.Nums[3 * xvi]), rots[order[1]]);
			Vector<3> nyv = retMat.RotateVectorC<xvi, yvi>(Vector<3>(&retMat.Nums[3 * yvi]), rots[order[1]]);
			memcpy(&retMat.Nums[xvi * SizeX], &nxv[0], sizeof(float) * 3);
			memcpy(&retMat.Nums[yvi * SizeX], &nyv[0], sizeof(float) * 3);
		}
		{
			constexpr unsigned int xvi = RotationStandart3D[order[2] * 2]; constexpr unsigned int yvi = RotationStandart3D[order[2] * 2 + 1];
			Vector<3> nxv = retMat.RotateVectorC<xvi, yvi>(Vector<3>(&retMat.Nums[3 * xvi]), rots[order[2]]);
			Vector<3> nyv = retMat.RotateVectorC<xvi, yvi>(Vector<3>(&retMat.Nums[3 * yvi]), rots[order[2]]);
			memcpy(&retMat.Nums[xvi * SizeX], &nxv[0], sizeof(float) * 3);
			memcpy(&retMat.Nums[yvi * SizeX], &nyv[0], sizeof(float) * 3);
		}

#undef rotVecMacr

		return retMat;
	}


	operator Vector<SizeX* SizeY>() {
		return Vector<SizeX * SizeY>(&Nums[0]);
	}

	Matrix<SizeX, SizeY> operator+(float num) { Matrix<SizeX, SizeY> retMat; for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] + num; return retMat; }
	Matrix<SizeX, SizeY> operator-(float num) { Matrix<SizeX, SizeY> retMat; for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] - num; return retMat; }
	Matrix<SizeX, SizeY> operator*(float num) { Matrix<SizeX, SizeY> retMat; for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] * num; return retMat; }
	Matrix<SizeX, SizeY> operator/(float num) { Matrix<SizeX, SizeY> retMat; for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] / num; return retMat; }

	template<unsigned int SizeX2>
	Matrix<SizeX2, SizeY> operator*(const Matrix<SizeX2, SizeX>& mat) const {
		Matrix<SizeX2, SizeY> retMat;
		for (unsigned int y = 0; y < SizeY; y++) {
			for (unsigned int x = 0; x < SizeX2; x++) {
				float sum = 0.f;
				for (unsigned int lo = 0; lo < SizeX; lo++) {
					sum += Nums[SizeY * lo + y] * mat.Nums[SizeX * x + lo];
				}
				retMat[x * SizeY + y] = sum;
			}
		}
		return retMat;
	}

	Vector<SizeY> operator*(const Vector<SizeY>& vec) const {
		return (Vector<SizeY>)((*this) * Matrix<1, SizeY>(&vec[0]));
	}




};