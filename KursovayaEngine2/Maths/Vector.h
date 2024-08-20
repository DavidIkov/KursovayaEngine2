#pragma once
#include"math.h"

template<unsigned int AxesAmount>
class Vector {

	float Axes[AxesAmount] = { 0.f };

private:
	void ConstructFromTemplate(const float num) {
		Axes[AxesAmount - 1] = num;
	}
	template<typename...otherNumsTyp>
	void ConstructFromTemplate(const float num, const otherNumsTyp...otherNums) {
		Axes[AxesAmount - sizeof...(otherNums) - 1] = num;
		ConstructFromTemplate(otherNums...);
	}
public:


	Vector(const float num) {
		for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] = num;
	}
	Vector(const float* numsStart) {
		memcpy(&Axes[0], numsStart, sizeof(float) * AxesAmount);
	}
	template<typename...NumsTyp>
	Vector(const float num, const NumsTyp...nums) {
		static_assert(sizeof...(nums) == (AxesAmount - 1), "Too many numbers");
		ConstructFromTemplate(num, nums...);
	}
	Vector() {

	}

	Vector(const Vector<AxesAmount>& vecToCopy) {
		memcpy(this, &vecToCopy, sizeof(vecToCopy));
	}
	Vector(Vector<AxesAmount>&& vecToCopy) {
		memcpy(this, &vecToCopy, sizeof(vecToCopy));
	}
	Vector<AxesAmount>& operator=(const Vector<AxesAmount>& vecToCopy) {
		memcpy(this, &vecToCopy, sizeof(vecToCopy));
		return *this;
	}


	float& operator[](const unsigned int ind) {
		return Axes[ind];
	}
	const float& operator[](const unsigned int ind) const {
		return Axes[ind];
	}

	//length but squared, square root didnt happen
	float LengthSQ() const {
		float sum = 0; for (unsigned int i = 0; i < AxesAmount; i++) sum += Axes[i] * Axes[i];
		return sum;
	}
	float Length() const { return sqrt(LengthSQ()); }

	Vector<AxesAmount> Normalize() const {
		return (*this) / Length();
	}
	//dot product with full length, means that it wasnt divided by length of both vectors
	float DotFL(const Vector<AxesAmount>& vec) const {
		float sum = 0;
		for (unsigned int i = 0; i < AxesAmount; i++) sum += vec.Axes[i] * Axes[i];
		return sum;
	}
	float Dot(const Vector<AxesAmount>& vec) const {
		return DotFL(vec) / vec.Length() / Length();
	}

	//cross product of full length, means that it wasnt deivided by length of both vectors
	Vector<AxesAmount> CrossFL(const Vector<AxesAmount>& vec) const {
		static_assert(AxesAmount == 3, "Cross is defined only for 3-axes vectors!");
		return Vector<AxesAmount>(Axes[1] * vec.Axes[2] - Axes[2] * vec.Axes[1], Axes[2] * vec.Axes[0] - Axes[0] * vec.Axes[2], Axes[0] * vec.Axes[1] - Axes[1] * vec.Axes[0]);
	}
	Vector<AxesAmount> Cross(const Vector<AxesAmount>& vec) const {
		return CrossFL(vec) / Length() / vec.Length();
	}


	Vector<AxesAmount> operator+(const float num) const { Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] + num; return retVec; }
	Vector<AxesAmount> operator-(const float num) const { Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] - num; return retVec; }
	Vector<AxesAmount> operator*(const float num) const { Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] * num; return retVec; }
	Vector<AxesAmount> operator/(const float num) const { Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] / num; return retVec; }

	Vector<AxesAmount> operator+(const Vector<AxesAmount>& vec) const {
		Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] + vec.Axes[i]; return retVec;
	}
	Vector<AxesAmount> operator-(const Vector<AxesAmount>& vec) const {
		Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] - vec.Axes[i]; return retVec;
	}
	Vector<AxesAmount> operator*(const Vector<AxesAmount>& vec) const {
		Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] * vec.Axes[i]; return retVec;
	}
	Vector<AxesAmount> operator/(const Vector<AxesAmount>& vec) const {
		Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] / vec.Axes[i]; return retVec;
	}

	Vector<AxesAmount> operator-() const { Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec.Axes[i] = -Axes[i]; return retVec; }

	void operator+=(const float num) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] += num; }
	void operator-=(const float num) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] -= num; }
	void operator*=(const float num) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] *= num; }
	void operator/=(const float num) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] /= num; }

	void operator+=(const Vector<AxesAmount>& vec) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] += vec.Axes[i]; }
	void operator-=(const Vector<AxesAmount>& vec) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] -= vec.Axes[i]; }
	void operator*=(const Vector<AxesAmount>& vec) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] *= vec.Axes[i]; }
	void operator/=(const Vector<AxesAmount>& vec) { for (unsigned int i = 0; i < AxesAmount; i++) Axes[i] /= vec.Axes[i]; }


};


template<unsigned int AxesAmount>
Vector<AxesAmount> operator+(const float num, const Vector<AxesAmount>& vec) {
	Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec[i] = num + vec[i]; return retVec;
}
template<unsigned int AxesAmount>
Vector<AxesAmount> operator-(const float num, const Vector<AxesAmount>& vec) {
	Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec[i] = num - vec[i]; return retVec;
}
template<unsigned int AxesAmount>
Vector<AxesAmount> operator*(const float num, const Vector<AxesAmount>& vec) {
	Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec[i] = num * vec[i]; return retVec;
}
template<unsigned int AxesAmount>
Vector<AxesAmount> operator/(const float num, const Vector<AxesAmount>& vec) {
	Vector<AxesAmount> retVec; for (unsigned int i = 0; i < AxesAmount; i++) retVec[i] = num / vec[i]; return retVec;
}
