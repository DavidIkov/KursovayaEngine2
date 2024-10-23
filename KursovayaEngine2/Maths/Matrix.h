#pragma once
#include"Vector.h"

/*
3d rotation standart: X:F,T; Y:R,F; Z:R,T
X,Y,Z means the rotation axis, and 2 second letter represent local X and Y vectors for rotation in 2d space
R-RightVec,T-TopVec,F-FrontVec
*/
constexpr unsigned int RotationStandart3D[6] = { 2,1,0,2,0,1 };

template<unsigned int SizeX, unsigned int SizeY, typename Type>
class Matrix {

	Type Nums[SizeX * SizeY] = { 0.f };

	template<typename...otherNumsTyp>
	void ConstructFromNums(const Type num, const otherNumsTyp...otherNums) {
		Nums[SizeX * SizeY - sizeof...(otherNums) - 1] = num;
		if constexpr (sizeof...(otherNums) != 0) ConstructFromNums(otherNums...);
	}

	template<unsigned int vecLen, typename...otherVecsTyp>
	void ConstructFromVecs(const Vector<vecLen, Type>& vec, const otherVecsTyp...otherVecs) {
		memcpy(&Nums[SizeY * (SizeX - sizeof...(otherVecs) - 1)], &vec[0], sizeof(Type) * vecLen);
		if constexpr (sizeof...(otherVecs) != 0) ConstructFromVecs<vecLen>(otherVecs...);
	}
public:
	template<unsigned int, unsigned int, typename>
	friend class Matrix;

	template<typename...NumsTyp>
	Matrix(const Type num, const NumsTyp...nums) {
		static_assert(sizeof...(nums) == (SizeX * SizeY - 1), "Too many numbers");
		ConstructFromNums(num, nums...);
	}
	//all vectors should be same length
	template<unsigned int vecLen, typename...VecsTyp>
	Matrix(const Vector<vecLen, Type>& vec, const VecsTyp...vecs) {
		static_assert(vecLen == SizeY, "Vector length is not the same as SizeY, cant construct matrix out of it");
		ConstructFromVecs<vecLen>(vec, vecs...);
	}
	Matrix(const Type num) {
		for (unsigned int i = 0; i < SizeX * SizeY; i++) Nums[i] = num;
	}
	Matrix(const Type* numsStart) {
		memcpy(&Nums[0], numsStart, sizeof(Type) * SizeX * SizeY);
	}
	Matrix() {

	}

	Matrix(const Matrix<SizeX, SizeY, Type>& matToCopy) {
		memcpy(this, &matToCopy, sizeof(matToCopy));
	}
	Matrix(Matrix<SizeX, SizeY, Type>&& matToCopy) {
		memcpy(this, &matToCopy, sizeof(matToCopy));
	}
	Matrix<SizeX, SizeY, Type>& operator=(const Matrix<SizeX, SizeY, Type>& matToCopy) {
		memcpy(this, &matToCopy, sizeof(matToCopy));
		return *this;
	}

	Type& operator[](const unsigned int ind) {
		return Nums[ind];
	}
	const Type& operator[](const unsigned int ind) const {
		return Nums[ind];
	}

	Type GetDeterminant() const {

		static_assert(SizeX == SizeY, "Determinant is only defined for square matrix");

		if constexpr (SizeX == 1) {
			return Nums[0];
		}
		else {

			Type* tempNums = new Type[(SizeX - 1) * (SizeY - 1)];
			int multiplier = 1;
			Type det = 0;
			for (unsigned int ai = 0; ai < SizeX; ai++) {
				for (unsigned int x = 0; x < SizeX; x++) {
					if (x == ai) continue;
					for (unsigned int y = 1; y < SizeY; y++) {
						unsigned int insertInd = y - 1 + (SizeY - 1) * (x > ai ? (x - 1) : x);
						tempNums[insertInd] = Nums[y + x * SizeY];
					}
				}
				det += Nums[ai * SizeY] * multiplier * Matrix<SizeX - 1, SizeX - 1, Type>(&tempNums[0]).GetDeterminant();
				multiplier = -multiplier;
			}
			delete[] tempNums;
			return det;
		}
	}


	Matrix<SizeX, SizeY, Type> GetInversedMatrix(Type det) const {

		static_assert(SizeX == SizeY, "Inversed matrix is only defined for square matrix becouse of determinant");

		Matrix<SizeX, SizeY, Type> retMat(0.f);

		Type tempNums[(SizeX - 1) * (SizeY - 1)] = { 0 };
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
				retMat.Nums[y + x * SizeY] = multipler * Matrix<SizeX - 1, SizeY - 1, Type>(tempNums).GetDeterminant() / det;
				multipler = -multipler;
			}
		}

		return retMat;
	}

	//used to normalize whole matrix, since after some rotations matrix can get a little broken becouse of Typeing point precision error
	void Normalize() {
		for (unsigned int x = 0; x < SizeX; x++) {
			Vector<SizeY, Type> v(&Nums[x * SizeY]);
			v = v.Normalize();
			memcpy(&Nums[x * SizeY], &v[0], sizeof(Type) * SizeY);
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

		Vector<3, Type> vecs[3] = { {&Nums[axis1 * 3]},{&Nums[axis2 * 3]},{0.f} };

		vecs[2] = vecs[0].CrossFL(vecs[1]);
		vecs[1] = vecs[2].CrossFL(vecs[0]);


		unsigned int axis3 = RotationStandart3D[axis1 * 2];
		if (axis3 == axis2) axis3 = RotationStandart3D[axis1 * 2 + 1];

		memcpy(&Nums[axis1 * 3], &vecs[0][0], sizeof(Type) * 3);
		memcpy(&Nums[axis2 * 3], &vecs[1][0], sizeof(Type) * 3);
		memcpy(&Nums[axis3 * 3], &vecs[2][0], sizeof(Type) * 3);
	}

	//all vectors are supposed to be length of 1
	//works only for Cartesian coordinates system
	template<unsigned int axisInd>
	Type GetLocalCordForAxisC(const Vector<SizeY, Type>& vec) const {

		static_assert(axisInd >= 0 and axisInd < SizeX, "Invalid axis");

		return Vector<SizeY, Type>(&Nums[SizeY * axisInd]).DotFL(vec);
	}

	//cant be used for just 1 axis at a time, have to deal with whole matrix, this will work with any gived matrix space but it comes at a cost...
	const Vector<SizeX, Type> GetLocalCordsForAxisU(const Vector<SizeY, Type>& vec) const {

		static_assert(SizeX == SizeY, "Getting local cords when matrix is not a square is not defined");

		return (Vector<SizeX, Type>)(GetInversedMatrix() * vec);

	}

	//"C" stands for Cartesian coordinates system
	//better to not use if you can optimize and dont use arccos, but if you cant then this is the function you need
	//be aware that acosf in this function can create "Typeing point error" very fast
	template<unsigned int axisX, unsigned int axisY>
	Vector<SizeY, Type> RotateVectorC(const Vector<SizeY, Type>& vec, const Type angle) const {

		static_assert(axisX < SizeX and axisX >= 0 and axisY < SizeX and axisY >= 0, "Invalid axis indexes");
		static_assert(axisX != axisY, "Axes cant be same");

		Type vecLen = vec.Length();

		Type localCordsX = GetLocalCordForAxisC<axisX>(vec) / vecLen;
		if (localCordsX < -1) localCordsX = -1; else if (localCordsX > 1) localCordsX = 1;
		Type localCordsY = GetLocalCordForAxisC<axisY>(vec) / vecLen;

		Type angleToX = acosf(localCordsX);
		//check if angle is negative
		if (localCordsY < 0) angleToX = -angleToX;

		angleToX += angle;

		Vector<SizeY, Type> xv(&Nums[SizeY * axisX]); Vector<SizeY, Type> yv(&Nums[SizeY * axisY]);

		return (xv * cosf(angleToX) + yv * sinf(angleToX)) * vecLen;
	}

	//axis1,axis2,axis3 are representing the order of rotation, X=0,Y=1,Z=2, so for example rotation by XYZ will be 0,1,2
	//will work only for Cartesian coordinate system(the "C" at end means Cartesian), it means that angle between all axes is pi/2
	//axis vectors are supposed to be length of 1
	template<unsigned int axis1, unsigned int axis2, unsigned int axis3>
	Matrix<SizeX, SizeY, Type> RotateIn3DByAnglesC(const Type xr, const Type yr, const Type zr) const {

		static_assert(SizeX == 3 and SizeY == 3, "Matrix is not 3x3, rotation is not defined");
		static_assert(axis1 != axis2 and axis1 != axis3 and axis2 != axis3, "You cant have repeating axis rotation order");
		static_assert(axis1 >= 0 and axis1 <= 3 and axis2 >= 0 and axis2 <= 3 and axis3 >= 0 and axis3 <= 3, "Incorrect axis set up");

		Matrix<SizeX, SizeY, Type> retMat(*this);

		const Type rots[3] = { xr,yr,zr };
		constexpr unsigned int order[3] = { axis1,axis2,axis3 };

#define rotVecMacr(i){\
		constexpr unsigned int xvi = RotationStandart3D[order[i] * 2]; constexpr unsigned int yvi = RotationStandart3D[order[i] * 2 + 1];\
		Vector<3, Type> xv(&retMat.Nums[SizeY * xvi]); Vector<3, Type> yv(&retMat.Nums[SizeY * yvi]);\
		Vector<3, Type> nxv = xv * cosf(rots[order[i]]) + yv * sinf(rots[order[i]]);\
		Vector<3, Type> nyv = xv * -sinf(rots[order[i]]) + yv * cosf(rots[order[i]]);\
		memcpy(&retMat.Nums[xvi * SizeX], &nxv[0], sizeof(Type) * 3);\
		memcpy(&retMat.Nums[yvi * SizeX], &nyv[0], sizeof(Type) * 3);}

		rotVecMacr(0);
		rotVecMacr(1);
		rotVecMacr(2);

#undef rotVecMacr

		return retMat;
	}


	operator Vector<SizeX * SizeY, Type>() {
		return Vector<SizeX * SizeY, Type>(&Nums[0]);
	}

	Matrix<SizeX, SizeY, Type> operator+(Type num) { Matrix<SizeX, SizeY, Type> retMat; for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] + num; return retMat; }
	Matrix<SizeX, SizeY, Type> operator-(Type num) { Matrix<SizeX, SizeY, Type> retMat; for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] - num; return retMat; }
	Matrix<SizeX, SizeY, Type> operator*(Type num) { Matrix<SizeX, SizeY, Type> retMat; for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] * num; return retMat; }
	Matrix<SizeX, SizeY, Type> operator/(Type num) { Matrix<SizeX, SizeY, Type> retMat; for (unsigned int i = 0; i < SizeX * SizeY; i++) retMat.Nums[i] = Nums[i] / num; return retMat; }

	template<unsigned int SizeX2>
	Matrix<SizeX2, SizeY, Type> operator*(const Matrix<SizeX2, SizeX, Type>& mat) const {
		Matrix<SizeX2, SizeY, Type> retMat;
		for (unsigned int y = 0; y < SizeY; y++) {
			for (unsigned int x = 0; x < SizeX2; x++) {
				Type sum = 0.f;
				for (unsigned int lo = 0; lo < SizeX; lo++) {
					sum += Nums[SizeY * lo + y] * mat.Nums[SizeX * x + lo];
				}
				retMat[x * SizeY + y] = sum;
			}
		}
		return retMat;
	}

	Vector<SizeY, Type> operator*(const Vector<SizeY, Type>& vec) const {
		return (Vector<SizeY, Type>)((*this) * Matrix<1, SizeY, Type>(&vec[0]));
	}


};

typedef Matrix<1, 1, float> Matrix11F;
typedef Matrix<1, 2, float> Matrix12F;
typedef Matrix<1, 3, float> Matrix13F;
typedef Matrix<1, 4, float> Matrix14F;
typedef Matrix<2, 1, float> Matrix21F;
typedef Matrix<2, 2, float> Matrix22F;
typedef Matrix<2, 3, float> Matrix23F;
typedef Matrix<2, 4, float> Matrix24F;
typedef Matrix<3, 1, float> Matrix31F;
typedef Matrix<3, 2, float> Matrix32F;
typedef Matrix<3, 3, float> Matrix33F;
typedef Matrix<3, 4, float> Matrix34F;
typedef Matrix<4, 1, float> Matrix41F;
typedef Matrix<4, 2, float> Matrix42F;
typedef Matrix<4, 3, float> Matrix43F;
typedef Matrix<4, 4, float> Matrix44F;

typedef Matrix<1, 1, double> Matrix11D;
typedef Matrix<1, 2, double> Matrix12D;
typedef Matrix<1, 3, double> Matrix13D;
typedef Matrix<1, 4, double> Matrix14D;
typedef Matrix<2, 1, double> Matrix21D;
typedef Matrix<2, 2, double> Matrix22D;
typedef Matrix<2, 3, double> Matrix23D;
typedef Matrix<2, 4, double> Matrix24D;
typedef Matrix<3, 1, double> Matrix31D;
typedef Matrix<3, 2, double> Matrix32D;
typedef Matrix<3, 3, double> Matrix33D;
typedef Matrix<3, 4, double> Matrix34D;
typedef Matrix<4, 1, double> Matrix41D;
typedef Matrix<4, 2, double> Matrix42D;
typedef Matrix<4, 3, double> Matrix43D;
typedef Matrix<4, 4, double> Matrix44D;

