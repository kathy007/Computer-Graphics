#include "v3.h"
#include "m33.h"

V3::V3(float x, float y, float z) {
  xyz[0] = x;
  xyz[1] = y;
  xyz[2] = z;
}

ostream& operator<<(ostream& os, V3 v) {
  os 
    << v.xyz[0] << " "
    << v.xyz[1] << " "
    << v.xyz[2];
  return os;
}

float& V3::operator[](int i) {
  return xyz[i];
}

V3 V3::operator+(V3 v1) {
  V3 ret;
  ret[0] = xyz[0] + v1[0];
  ret[1] = xyz[1] + v1[1];
  ret[2] = xyz[2] + v1[2];
  return ret;
}

V3 V3::operator-(V3 v1) {
  V3 ret;
  ret[0] = xyz[0] - v1[0];
  ret[1] = xyz[1] - v1[1];
  ret[2] = xyz[2] - v1[2];
  return ret;
}

float V3::operator*(V3 v1) {
  return 
    xyz[0]*v1[0] + 
    xyz[1]*v1[1] + 
    xyz[2]*v1[2];
}

V3 V3::operator/(float scf) {
  V3 ret(*this);
  ret[0] /= scf;
  ret[1] /= scf;
  ret[2] /= scf;
  return ret;
}

V3 V3::operator*(float scf) {
  V3 ret(*this);
  ret[0] *= scf;
  ret[1] *= scf;
  ret[2] *= scf;
  return ret;
}

V3 V3::operator&(V3 v1) { // cross product
	V3 ret;
	ret[0] = xyz[1]*v1[2] - xyz[2]*v1[1];
	ret[1] = xyz[2]*v1[0] - xyz[0]*v1[2];
	ret[2] = xyz[0]*v1[1] - xyz[1]*v1[0];
	return ret;
}

float V3::Length() {
	return sqrt(pow(xyz[0], 2) + pow(xyz[1], 2) + pow(xyz[2], 2)); 
}
void V3::Normalize() {
	float length = (*this).Length();
	xyz[0] = xyz[0] / length;
	xyz[1] = xyz[1] / length;
	xyz[2] = xyz[2] / length;
	for (int i = 0; i < 3; i ++) {
		if (xyz[i] == -0) {
			xyz[i] = 0;
		}
	}
}

V3 V3::TransformCoordinate(V3 newOrigin, V3 a, V3 b, V3 c) {
	V3 rows[3], ret;
	rows[0] = V3(a[0], a[1], a[2]);
	rows[1] = V3(b[0], b[1], b[2]);
	rows[2] = V3(c[0], c[1], c[2]);
	V3 v = (*this) - newOrigin;
	ret = V3(rows[0]*v, rows[1]*v, rows[2]*v);
	return ret;
}
V3 V3::RotatePoint(V3 n, V3 m, float scf) {
	V3 v;
	V3 a = m;
	a.Normalize();
	V3 b, c;
	V3 x = V3(1, 0, 0);
	V3 y = V3(0, 1, 0);
	b = (x*a < y*a) ? x&a : y&a;
	b.Normalize();
	c = a&b;
	c.Normalize();
	V3 P1 = this->TransformCoordinate(n, a, b, c);
	V3 P2 = M33(0, scf)*P1;
	V3 Pr = P2.TransformCoordinate(V3(0, 0, 0), V3(1, 0, 0), V3(0, 1, 0), V3(0, 0, 1));
	v = V3(Pr[0], Pr[1], Pr[2]);
	return v;
}
V3 V3::RotateVector(V3 n, V3 m, float scf) {
	return this->RotatePoint(n, m, scf) - V3(0, 0, 0).RotatePoint(n, m, scf);
}

void V3::SetFromColor(unsigned int color) {
  V3 &v = *this;
  v[0] = ((unsigned char*)&color)[0];
  v[1] = ((unsigned char*)&color)[1];
  v[2] = ((unsigned char*)&color)[2];
  v = v / 255.0f;
}

unsigned int V3::GetColor() {
  V3 &v = *this;
  unsigned int red = (int) (v[0]*255.0f+0.5f);
  unsigned int green = (int) (v[0]*255.0f+0.5f);
  unsigned int blue = (int) (v[0]*255.0f+0.5f);

  if (red < 0)
    red = 0;
  else if (red > 255)
    red = 255;
  if (green < 0)
    green = 0;
  else if (green > 255)
    green = 255;
  if (blue < 0)
    blue = 0;
  else if (blue > 255)
    blue = 255;

  unsigned int ret = 0xFF000000 + blue * 256 *256 + green * 256 + red;
  return ret;
}
