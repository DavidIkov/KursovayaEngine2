#include"Vector2.h"
#include"Matrix.h"


Vector2 operator+(float num, const Vector2& vec) { return Vector2(vec.gX() + num, vec.gY() + num); };
Vector2 operator-(float num, const Vector2& vec) { return Vector2(vec.gX() - num, vec.gY() - num); };
Vector2 operator*(float num, const Vector2& vec) { return Vector2(vec.gX() * num, vec.gY() * num); };
Vector2 operator/(float num, const Vector2& vec) { return Vector2(vec.gX() / num, vec.gY() / num); };

Vector2 Vector2::atan(const Vector2& vec) {
	return { atanf(vec.gX()),atanf(vec.gY()) };
}

float& Vector2::operator[](unsigned int ind) {
	return Cords[ind];
}
float Vector2::operator[](unsigned int ind) const {
	return Cords[ind];
}

void Vector2::RotateVectors(const Vector2& xv, const Vector2& yv, float angle, Vector2& xvTOWRITE, Vector2& yvTOWRITE) {
	//a little problem is that function can receive same variables as xv and xvTOWRITE(similar with yv) so we need to do 1 copy to not break everything
	Vector2 savedXv = xv;
	xvTOWRITE = xv * cosf(angle) + yv * sinf(angle);
	yvTOWRITE = yv * cosf(angle) - savedXv * sinf(angle);
}

float Vector2::DotProduct(const Vector2& vec) const {
	return (vec.Cords[0] * Cords[0] + vec.Cords[1] * Cords[1]) / vec.Length() / Length();
}

Vector2::Vector2() {

}
Vector2::Vector2(float nX, float nY) {
	Cords[0] = nX;
	Cords[1] = nY;
}
Vector2::Vector2(float nAx) {
	Cords[0] = nAx;
	Cords[1] = nAx;
}

Vector2::Vector2(const Matrix&& mat) {
	Cords[0] = mat[0];
	Cords[1] = mat[1];
}

Vector2& Vector2::operator=(const Vector2& toCopy) {
	newLen = true;
	newUnit = true;
	Cords[0] = toCopy.Cords[0];
	Cords[1] = toCopy.Cords[1];
	return *this;
}

float Vector2::SqLength() const {
	if (newLen) {
		SqLen = Cords[0] * Cords[0] + Cords[1] * Cords[1];
		newLen = false;
	}
	return SqLen;
}
float Vector2::Length() const {
	if (newLen) {
		Len = sqrtf(Cords[0] * Cords[0] + Cords[1] * Cords[1]);
		newLen = false;
	}
	return Len;
}
Vector2 Vector2::Unit() const {
	if (newUnit) {
		if (newLen) Length();
		UnitX = Cords[0] / Len;
		UnitY = Cords[1] / Len;
		newUnit = false;
	}
	return Vector2(UnitX, UnitY);
}

void Vector2::sX(float nX) {
	Cords[0] = nX;
	newUnit = true;
	newLen = true;
}
float Vector2::gX() const {
	return Cords[0];
}
void Vector2::sY(float nY) {
	Cords[1] = nY;
	newUnit = true;
	newLen = true;
}
float Vector2::gY() const {
	return Cords[1];
}

Vector2 Vector2::operator+(float num) const { return Vector2(Cords[0] + num, Cords[1] + num); };
Vector2 Vector2::operator-(float num) const { return Vector2(Cords[0] - num, Cords[1] - num); };
Vector2 Vector2::operator*(float num) const { return Vector2(Cords[0] * num, Cords[1] * num); };
Vector2 Vector2::operator/(float num) const { return Vector2(Cords[0] / num, Cords[1] / num); };

void Vector2::operator+=(float num) { Cords[0] += num; Cords[1] += num; }
void Vector2::operator-=(float num) { Cords[0] -= num; Cords[1] -= num; }
void Vector2::operator*=(float num) { Cords[0] *= num; Cords[1] *= num; }
void Vector2::operator/=(float num) { Cords[0] /= num; Cords[1] /= num; }

Vector2 Vector2::operator+(const Vector2& vec) const { return Vector2(Cords[0] + vec.Cords[0], Cords[1] + vec.Cords[1]); };
Vector2 Vector2::operator-(const Vector2& vec) const { return Vector2(Cords[0] - vec.Cords[0], Cords[1] - vec.Cords[1]); };
Vector2 Vector2::operator*(const Vector2& vec) const { return Vector2(Cords[0] * vec.Cords[0], Cords[1] * vec.Cords[1]); };
Vector2 Vector2::operator/(const Vector2& vec) const { return Vector2(Cords[0] / vec.Cords[0], Cords[1] / vec.Cords[1]); };

void Vector2::operator+=(const Vector2& vec) { Cords[0] += vec.Cords[0]; Cords[1] += vec.Cords[1]; }
void Vector2::operator-=(const Vector2& vec) { Cords[0] -= vec.Cords[0]; Cords[1] -= vec.Cords[1]; }
void Vector2::operator*=(const Vector2& vec) { Cords[0] *= vec.Cords[0]; Cords[1] *= vec.Cords[1]; }
void Vector2::operator/=(const Vector2& vec) { Cords[0] /= vec.Cords[0]; Cords[1] /= vec.Cords[1]; }

Vector2 Vector2::operator+(const Matrix& mat) const { return Vector2(Cords[0] + mat[0], Cords[1] + mat[1]); };
Vector2 Vector2::operator-(const Matrix& mat) const { return Vector2(Cords[0] - mat[0], Cords[1] - mat[1]); };
Vector2 Vector2::operator*(const Matrix& mat) const { return Vector2(Cords[0] * mat[0], Cords[1] * mat[1]); };
Vector2 Vector2::operator/(const Matrix& mat) const { return Vector2(Cords[0] / mat[0], Cords[1] / mat[1]); };

void Vector2::operator+=(const Matrix& mat) { Cords[0] += mat[0]; Cords[1] += mat[1]; }
void Vector2::operator-=(const Matrix& mat) { Cords[0] -= mat[0]; Cords[1] -= mat[1]; }
void Vector2::operator*=(const Matrix& mat) { Cords[0] *= mat[0]; Cords[1] *= mat[1]; }
void Vector2::operator/=(const Matrix& mat) { Cords[0] /= mat[0]; Cords[1] /= mat[1]; }