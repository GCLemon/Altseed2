

#if defined(_M_X86) && defined(__x86__)
#include <emmintrin.h>
#endif
#include "Matrix44F.h"
#include "Vector3DF.h"

namespace altseed {

inline float Rsqrt(float x) {
#if defined(_M_X86) && defined(__x86__)
    _mm_store_ss(&x, _mm_rsqrt_ss(_mm_load_ss(&x)));
    return x;
#else
    float xhalf = 0.5f * x;
    int i = *(int*)&x;
    i = 0x5f3759df - (i >> 1);
    x = *(float*)&i;
    x = x * (1.5f - xhalf * x * x);
    x = x * (1.5f - xhalf * x * x);
    return x;
#endif
}

Vector3DF::Vector3DF() : X(0.0f), Y(0.0f), Z(0.0f) {}

Vector3DF::Vector3DF(float x, float y, float z) : X(x), Y(y), Z(z) {}

bool Vector3DF::operator==(const Vector3DF& o) { return X == o.X && Y == o.Y && Z == o.Z; }

bool Vector3DF::operator!=(const Vector3DF& o) { return !(X == o.X && Y == o.Y && Z == o.Z); }

bool Vector3DF::operator>(const Vector3DF& o) const {
    if (X != o.X) return X > o.X;
    if (Y != o.Y) return Y > o.Y;
    if (Z != o.Z) return Z > o.Z;
    return false;
}

bool Vector3DF::operator<(const Vector3DF& o) const {
    if (X != o.X) return X < o.X;
    if (Y != o.Y) return Y < o.Y;
    if (Z != o.Z) return Z < o.Z;
    return false;
}

Vector3DF Vector3DF::operator-() { return Vector3DF(-X, -Y, -Z); }

Vector3DF Vector3DF::operator+(const Vector3DF& o) const { return Vector3DF(X + o.X, Y + o.Y, Z + o.Z); }

Vector3DF Vector3DF::operator-(const Vector3DF& o) const { return Vector3DF(X - o.X, Y - o.Y, Z - o.Z); }

Vector3DF Vector3DF::operator*(const Vector3DF& o) const { return Vector3DF(X * o.X, Y * o.Y, Z * o.Z); }

Vector3DF Vector3DF::operator/(const Vector3DF& o) const { return Vector3DF(X / o.X, Y / o.Y, Z / o.Z); }

Vector3DF Vector3DF::operator*(const float& o) const { return Vector3DF(X * o, Y * o, Z * o); }

Vector3DF Vector3DF::operator/(const float& o) const { return Vector3DF(X / o, Y / o, Z / o); }

Vector3DF& Vector3DF::operator+=(const Vector3DF& o) {
    X += o.X;
    Y += o.Y;
    Z += o.Z;
    return *this;
}

Vector3DF& Vector3DF::operator-=(const Vector3DF& o) {
    X -= o.X;
    Y -= o.Y;
    Z -= o.Z;
    return *this;
}

Vector3DF& Vector3DF::operator*=(const float& o) {
    X *= o;
    Y *= o;
    Z *= o;
    return *this;
}

Vector3DF& Vector3DF::operator/=(const float& o) {
    X /= o;
    Y /= o;
    Z /= o;
    return *this;
}

float Vector3DF::Dot(const Vector3DF& v1, const Vector3DF& v2) { return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z; }

Vector3DF Vector3DF::Cross(const Vector3DF& v1, const Vector3DF& v2) {
    Vector3DF o;

    float x = v1.Y * v2.Z - v1.Z * v2.Y;
    float y = v1.Z * v2.X - v1.X * v2.Z;
    float z = v1.X * v2.Y - v1.Y * v2.X;
    o.X = x;
    o.Y = y;
    o.Z = z;
    return o;
}

Vector3DF Vector3DF::Subtract(Vector3DF v1, Vector3DF v2) {
    Vector3DF o = Vector3DF();
    o.X = v1.X - v2.X;
    o.Y = v1.Y - v2.Y;
    o.Z = v1.Z - v2.Z;
    return o;
}

float Vector3DF::Distance(const Vector3DF& v1, const Vector3DF& v2) {
    float dx = v1.X - v2.X;
    float dy = v1.Y - v2.Y;
    float dz = v1.Z - v2.Z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

Vector3DI Vector3DF::To3DI() const { return Vector3DI((int32_t)X, (int32_t)Y, (int32_t)Z); }

}  // namespace altseed
