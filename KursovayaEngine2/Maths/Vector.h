#pragma once
#include"math.h"

template<unsigned int AxesAmount, typename Type>
class Vector {

	Type Axes[AxesAmount] = { 0 };

	void ConstructFromTemplate(const Type num) {
		Axes[AxesAmount - 1] = num;
	}
	template<typename...otherNumsTyp>
	void ConstructFromTemplate(const Type num, const otherNumsTyp...otherNums) {
		Axes[AxesAmount - sizeof...(otherNums) - 1] = num;
		ConstructFromTemplate(otherNums...);
	}
public:

	template<unsigned int, typename>
	friend class Vector;


	Vector(const Type num) {
		for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] = num;
	}
	Vector(const Type* numsStart) {
		memcpy(&Axes[0], numsStart, sizeof(Type) * AxesAmount);
	}
	template<typename...NumsTyp>
	Vector(const Type num, const NumsTyp...nums) {
		static_assert(sizeof...(nums) == (AxesAmount - 1), "Too many or not enough numbers");
		ConstructFromTemplate(num, nums...);
	}
	Vector() {
		
	}

	Vector(const Vector<AxesAmount, Type>& vecToCopy) {
		memcpy(this, &vecToCopy, sizeof(vecToCopy));
	}
	Vector(Vector<AxesAmount, Type>&& vecToCopy) {
		memcpy(this, &vecToCopy, sizeof(vecToCopy));
	}
	Vector<AxesAmount, Type>& operator=(const Vector<AxesAmount, Type>& vecToCopy) {
		memcpy(this, &vecToCopy, sizeof(vecToCopy));
		return *this;
	}


	Type& operator[](const unsigned int ind) {
		return Axes[ind];
	}
	const Type& operator[](const unsigned int ind) const {
		return Axes[ind];
	}

	//length but squared, square root didnt happen
	Type LengthSQ() const {
		Type sum = 0; for (unsigned int i = 0; i < AxesAmount; i++) sum += Axes[i] * Axes[i];
		return sum;
	}
	Type Length() const { return sqrt(LengthSQ()); }

	Vector<AxesAmount, Type> Normalize() const {
		return (*this) / Length();
	}
	//dot product with full length, means that it wasnt divided by length of both vectors
	Type DotFL(const Vector<AxesAmount, Type>& vec) const {
		Type sum = 0;
		for (unsigned int i = 0; i < AxesAmount; i++) sum += vec.Axes[i] * Axes[i];
		return sum;
	}
	Type Dot(const Vector<AxesAmount, Type>& vec) const {
		return DotFL(vec) / vec.Length() / Length();
	}

	//cross product of full length, means that it wasnt deivided by length of both vectors
	Vector<AxesAmount, Type> CrossFL(const Vector<AxesAmount, Type>& vec) const {
		static_assert(AxesAmount == 3, "Cross is defined only for 3-axes vectors!");
		return Vector<AxesAmount, Type>(Axes[1] * vec.Axes[2] - Axes[2] * vec.Axes[1], Axes[2] * vec.Axes[0] - Axes[0] * vec.Axes[2], Axes[0] * vec.Axes[1] - Axes[1] * vec.Axes[0]);
	}
	Vector<AxesAmount, Type> Cross(const Vector<AxesAmount, Type>& vec) const {
		return CrossFL(vec) / Length() / vec.Length();
	}


	Vector<AxesAmount, Type> operator+(const Type num) const { Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] + num; return retVec; }
	Vector<AxesAmount, Type> operator-(const Type num) const { Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] - num; return retVec; }
	Vector<AxesAmount, Type> operator*(const Type num) const { Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] * num; return retVec; }
	Vector<AxesAmount, Type> operator/(const Type num) const { Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] / num; return retVec; }

	Vector<AxesAmount, Type> operator+(const Vector<AxesAmount, Type>& vec) const {
		Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] + vec.Axes[i]; return retVec;
	}
	Vector<AxesAmount, Type> operator-(const Vector<AxesAmount, Type>& vec) const {
		Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] - vec.Axes[i]; return retVec;
	}
	Vector<AxesAmount, Type> operator*(const Vector<AxesAmount, Type>& vec) const {
		Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] * vec.Axes[i]; return retVec;
	}
	Vector<AxesAmount, Type> operator/(const Vector<AxesAmount, Type>& vec) const {
		Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] / vec.Axes[i]; return retVec;
	}

	Vector<AxesAmount, Type> operator-() const { Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = -Axes[i]; return retVec; }

	void operator+=(const Type num) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] += num; }
	void operator-=(const Type num) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] -= num; }
	void operator*=(const Type num) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] *= num; }
	void operator/=(const Type num) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] /= num; }

	void operator+=(const Vector<AxesAmount, Type>& vec) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] += vec.Axes[i]; }
	void operator-=(const Vector<AxesAmount, Type>& vec) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] -= vec.Axes[i]; }
	void operator*=(const Vector<AxesAmount, Type>& vec) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] *= vec.Axes[i]; }
	void operator/=(const Vector<AxesAmount, Type>& vec) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] /= vec.Axes[i]; }

	template<typename Type2>
	constexpr operator Vector<AxesAmount, Type2>() const { Vector<AxesAmount, Type2> vec; for (unsigned int i = 0; i < AxesAmount; i++) vec.Axes[i] = (Type2)Axes[i]; return vec; }

};


template<unsigned int AxesAmount, typename Type>
Vector<AxesAmount, Type> operator+(const Type num, const Vector<AxesAmount, Type>& vec) {
	Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec[i] = num + vec[i]; return retVec;
}
template<unsigned int AxesAmount, typename Type>
Vector<AxesAmount, Type> operator-(const Type num, const Vector<AxesAmount, Type>& vec) {
	Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec[i] = num - vec[i]; return retVec;
}
template<unsigned int AxesAmount, typename Type>
Vector<AxesAmount, Type> operator*(const Type num, const Vector<AxesAmount, Type>& vec) {
	Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec[i] = num * vec[i]; return retVec;
}
template<unsigned int AxesAmount, typename Type>
Vector<AxesAmount, Type> operator/(const Type num, const Vector<AxesAmount, Type>& vec) {
	Vector<AxesAmount, Type> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec[i] = num / vec[i]; return retVec;
}


typedef Vector<1, int> Vector1I;
typedef Vector<2, int> Vector2I;
typedef Vector<3, int> Vector3I;
typedef Vector<4, int> Vector4I;

typedef Vector<1, unsigned int> Vector1U;
typedef Vector<2, unsigned int> Vector2U;
typedef Vector<3, unsigned int> Vector3U;
typedef Vector<4, unsigned int> Vector4U;

typedef Vector<1, float> Vector1F;
typedef Vector<2, float> Vector2F;
typedef Vector<3, float> Vector3F;
typedef Vector<4, float> Vector4F;

typedef Vector<1, double> Vector1D;
typedef Vector<2, double> Vector2D;
typedef Vector<3, double> Vector3D;
typedef Vector<4, double> Vector4D;