#include"Vector3.h"
#include"Vector2.h"
#include"Matrix.h"


Vector3 operator+(float num, const Vector3& vec) { return Vector3(vec.gX() + num, vec.gY() + num, vec.gZ() + num); };
Vector3 operator-(float num, const Vector3& vec) { return Vector3(vec.gX() - num, vec.gY() - num, vec.gZ() - num); };
Vector3 operator*(float num, const Vector3& vec) { return Vector3(vec.gX() * num, vec.gY() * num, vec.gZ() * num); };
Vector3 operator/(float num, const Vector3& vec) { return Vector3(vec.gX() / num, vec.gY() / num, vec.gZ() / num); };

float& Vector3::operator[](unsigned int ind) {
	return Cords[ind];
}
float Vector3::operator[](unsigned int ind) const {
	return Cords[ind];
}

void Vector3::RotateVectors(const Vector3& xv, const Vector3& yv, float angle, Vector3& xvTOWRITE, Vector3& yvTOWRITE) {
	//a little problem is that function can receive same variables as xv and xvTOWRITE(similar with yv) so we need to do 1 copy to not break everything
	Vector3 savedXv = xv;
	xvTOWRITE = xv * cosf(angle) + yv * sinf(angle);
	yvTOWRITE = yv * cosf(angle) - savedXv * sinf(angle);
}

Vector2 Vector3::gvXY() const {
	return Vector2(Cords[0], Cords[1]);
}
Vector2 Vector3::gvYX() const {
	return Vector2(Cords[1], Cords[0]);
}
Vector2 Vector3::gvXZ() const {
	return Vector2(Cords[0], Cords[2]);
}
Vector2 Vector3::gvZX() const {
	return Vector2(Cords[2], Cords[0]);
}
Vector2 Vector3::gvYZ() const {
	return Vector2(Cords[1], Cords[2]);
}
Vector2 Vector3::gvZY() const {
	return Vector2(Cords[2], Cords[1]);
}

Vector3::Vector3() {

}
Vector3::Vector3(float nX, float nY, float nZ) {
	Cords[0] = nX;
	Cords[1] = nY;
	Cords[2] = nZ;
}
Vector3::Vector3(float nn) {
	Cords[0] = nn;
	Cords[1] = nn;
	Cords[2] = nn;
}

Vector3::Vector3(const Matrix&& mat) {
	Cords[0] = mat[0];
	Cords[1] = mat[1];
	Cords[2] = mat[2];
}

Vector3& Vector3::operator=(const Vector3& toCopy) {
	newLen = true;
	newUnit = true;
	Cords[0] = toCopy.Cords[0];
	Cords[1] = toCopy.Cords[1];
	Cords[2] = toCopy.Cords[2];
	return *this;
}

float Vector3::SqLength() const {
	if (newLen) {
		SqLen = Cords[0] * Cords[0] + Cords[1] * Cords[1] + Cords[2] * Cords[2];
		newLen = false;
	}
	return SqLen;
}
float Vector3::Length() const {
	if (newLen) {
		Len = sqrtf(Cords[0] * Cords[0] + Cords[1] * Cords[1] + Cords[2] * Cords[2]);
		newLen = false;
	}
	return Len;
}
Vector3 Vector3::Unit() const {
	if (newUnit) {
		if (newLen) Length();
		UnitX = Cords[0] / Len;
		UnitY = Cords[1] / Len;
		UnitZ = Cords[2] / Len;
		newUnit = false;
	}
	return Vector3(UnitX, UnitY, UnitZ);
}

Vector3 Vector3::Cross(const Vector3& vec) const {
	//v1.y*v2.z-v2.y*v1.z ; v1.z*v2.x-v2.z*v1.x ; v1.x*v2.y-v2.x*v1.y
	return Vector3(Cords[1] * vec.Cords[2] - vec.Cords[1] * Cords[2], Cords[2] * vec.Cords[0] - vec.Cords[2] * Cords[0], Cords[0] * vec.Cords[1] - vec.Cords[0] * Cords[1]);
}

void Vector3::sX(float nX) {
	Cords[0] = nX;
	newUnit = true;
	newLen = true;
}
float Vector3::gX() const {
	return Cords[0];
}
void Vector3::sY(float nY) {
	Cords[1] = nY;
	newUnit = true;
	newLen = true;
}
float Vector3::gY() const {
	return Cords[1];
}
void Vector3::sZ(float nZ) {
	Cords[2] = nZ;
	newUnit = true;
	newLen = true;
}
float Vector3::gZ() const {
	return Cords[2];
}

Vector3 Vector3::operator+(float num) const { return Vector3(Cords[0] + num, Cords[1] + num, Cords[2] + num); };
Vector3 Vector3::operator-(float num) const { return Vector3(Cords[0] - num, Cords[1] - num, Cords[2] - num); };
Vector3 Vector3::operator*(float num) const { return Vector3(Cords[0] * num, Cords[1] * num, Cords[2] * num); };
Vector3 Vector3::operator/(float num) const { return Vector3(Cords[0] / num, Cords[1] / num, Cords[2] / num); };

Vector3 Vector3::operator+(const Vector3& vec) const { return Vector3(Cords[0] + vec.Cords[0], Cords[1] + vec.Cords[1], Cords[2] + vec.Cords[2]); };
Vector3 Vector3::operator-(const Vector3& vec) const { return Vector3(Cords[0] - vec.Cords[0], Cords[1] - vec.Cords[1], Cords[2] - vec.Cords[2]); };
Vector3 Vector3::operator*(const Vector3& vec) const { return Vector3(Cords[0] * vec.Cords[0], Cords[1] * vec.Cords[1], Cords[2] * vec.Cords[2]); };
Vector3 Vector3::operator/(const Vector3& vec) const { return Vector3(Cords[0] / vec.Cords[0], Cords[1] / vec.Cords[1], Cords[2] / vec.Cords[2]); };

Vector3 Vector3::operator+(const Matrix& mat) const { return Vector3(Cords[0] + mat[0], Cords[1] + mat[1], Cords[2] + mat[2]); };
Vector3 Vector3::operator-(const Matrix& mat) const { return Vector3(Cords[0] - mat[0], Cords[1] - mat[1], Cords[2] - mat[2]); };
Vector3 Vector3::operator*(const Matrix& mat) const { return Vector3(Cords[0] * mat[0], Cords[1] * mat[1], Cords[2] * mat[2]); };
Vector3 Vector3::operator/(const Matrix& mat) const { return Vector3(Cords[0] / mat[0], Cords[1] / mat[1], Cords[2] / mat[2]); };