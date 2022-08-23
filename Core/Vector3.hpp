/* Include guards */
// make it impossible to include this file
// twice inside one destination file.
// If Vector3_hpp is not defined, #define it, and
// execute the code until #endif in the end of
// this file. If's it's defined, do nothing.
// #pragma once does the same thing.
#ifndef Vector3_hpp
#define Vector3_hpp

#include <iostream>
#include <math.h>

/* Inline, ODR */
// * Inline function's body is inserted at the point of call
//   during compilation to increase efficiency.
// * Inline keyword is a request, not a command.
// * All functions defined inside a class body are implicitly
//   inline.
// * Inline functions can only be defined inside header file.
// * If the functions defined in header were not inlined, the
//   ODR (One Definition Rule) would be broken if this header
//   was included in multiple files because each file would
//   contain its own copy of function's definition, which
//   is not allowed by C++, and will result into linking error.
// * Another way of avoiding breaking ODR is to place the
//   functions' definitions into Vec3.cpp without inlining.
//
/* Member functions (methods) */
// * Have unrestricted access to class' members.
// * Have an implicit *this argument passed, therefore can
//   modify the object.
// * Can be const.
//
/* Non-member non-friend functions (free functions) */
// * Cannot access class' private members.
// * Do not have an implicit *this argument passed.
// * Can't be const.
//
/* Non-member friend functions */
// * Have unrestricted access to class' members.
// * Do not have an implicit *this argument passed.
// * Cannot be const.
//
/* `const` keyword in methods */
// * Const methods are methods with const *this argument,
//   which means they cannot modify current (this) object.
// * Only const methods can be called on const objects.

/* THEORY */
// * A vector can be compared to an instruction of how much
//   to move along each axis.
// * Unit vector is vector of length equal to 1: sqrt(x^2 + y^2 +z^2) = 1
// * Vector substraction: V1 - V2 = V1 + (-V2)
//   ^\
//   | \
//   V1 \
//   |   \ V1 - V2 (from end of V2 to end of V1)
//   |    \
//   |     \
//   *--V2-->
// * Vector multiplication (dot product, inner product): (V1 • V2)
//   is a scalar value (V1.x * V2.x + V1.y * V2.y + V1.z * V2.z)
//        /|
//       /
//   V1 /  |
//     /
//    /    |      V2
//   *-dot-*--------->
//   If V1 and V2 are unit vectors, dot product is the projection:
//   V1 • V2 = |V1| * cos(V1,V2) = cos(V1, V2)
//   |V| = sqrt(V • V).
//   If V2 is not a unit vector, projection of V1 on V2:
//   V1 • V2 = (|V1| * cos(V1,V2)) / |V2| = |V1| * cos(V1,V2)
// * Cross product (vector product): V1 x V2
//   In 3d space, cross product is a vector that is perpendicular
//   to both V1 and V2.
//   V1 x V2 = ||V1|| * ||V2|| * sin(V1,V2) * n, where n is normal
//   unit vector to V1 and V2.
//       ^
//       |
//       |
//       | V1 x V2
//       |
//       |
//       *-------------->
//      /|      V2
//     / |
// V1 /  |
//   /   | V2 x V1 = -V1 x V2
//  /    V

class Vector3 {
    double e[3]; // only accessable by members and friends
public:
    Vector3() {}; // implicitly inline since defined here
    // Member initialization lists are faster since they don't use
    // assignment and the default constructor is not called.
    Vector3(double e0, double e1, double e2): e{ e0, e1, e2 } {};

    /* Member functions */
    double x() const;
    double y() const;
    double z() const;
    double r() const;
    double g() const;
    double b() const;
    double length() const;
    double squaredLength() const;
    void makeUnitVector();

    /* Non-member functions */
    friend double dot(const Vector3 &v1, const Vector3 &v2);
    friend Vector3 cross(const Vector3 &v1, const Vector3 &v2);
    Vector3 unitVector(Vector3 v);

    /* Operators */
    const Vector3 &operator+() const; // Returns ref. but can be const since returns non-modif. (const)
    Vector3 operator-() const;
    double operator[](int i) const;
    double &operator[](int i); // Cannot be const since returns modifiable and non-const object
    // * Binary operators are implemented as non-members to
    //   maintain operator's symmetry (a + b = b + a) that's
    //   otherwise (with an implicit *this) hard to achieve.
    // * Binary operators are friends because require access
    //   to private arguments of this class.
    friend Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
    friend Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
    friend Vector3 operator*(const Vector3 &v1, const Vector3 &v2);
    friend Vector3 operator/(const Vector3 &v1, const Vector3 &v2);
    friend Vector3 operator*(double t, const Vector3 &v);
    friend Vector3 operator*(const Vector3 &v, double t);
    friend Vector3 operator/(Vector3 v, double t);
    // Stream extraction and insertion operators take a user defined
    // type as the right argument and must be implemented as non-members.
    friend std::istream &operator>>(std::istream &is, Vector3 &t);
    friend std::ostream &operator<<(std::ostream &os, const Vector3 &t);

    Vector3 &operator+=(const Vector3 &v);
    Vector3 &operator+=(const double t);
    Vector3 &operator-=(const Vector3 &v);
    Vector3 &operator*=(const Vector3 &v);
    Vector3 &operator*=(const double t);
    Vector3 &operator/=(const Vector3 &v);
    Vector3 &operator/=(const double t);
};

/* Member functions */
inline double Vector3::x() const { return e[0]; }
inline double Vector3::y() const { return e[1]; }
inline double Vector3::z() const { return e[2]; }
inline double Vector3::r() const { return e[0]; }
inline double Vector3::g() const { return e[1]; }
inline double Vector3::b() const { return e[2]; }

inline double Vector3::length() const {
    return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
}

inline double Vector3::squaredLength() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

inline void Vector3::makeUnitVector() {
    double k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
}

/* Non-member functions */
inline double dot(const Vector3 &v1, const Vector3 &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline Vector3 cross(const Vector3 &v1, const Vector3 &v2) {
    return Vector3(
        (v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
        (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
        (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0])
    );
}

inline Vector3 unitVector(Vector3 v) {
    return v / v.length();
}

/* Operators */
inline const Vector3 &Vector3::operator+() const { return *this; }
inline Vector3 Vector3::operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
inline double Vector3::operator[](int i) const { return e[i]; }
inline double &Vector3::operator[](int i) { return e[i]; }

inline Vector3 operator+(const Vector3 &v1, const Vector3 &v2) {
    return Vector3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vector3 operator-(const Vector3 &v1, const Vector3 &v2) {
    return Vector3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vector3 operator*(const Vector3 &v1, const Vector3 &v2) {
    return Vector3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline Vector3 operator/(const Vector3 &v1, const Vector3 &v2) {
    return Vector3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vector3 operator*(double t, const Vector3 &v) {
    return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator*(const Vector3 &v, double t) {
    return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator/(Vector3 v, double t) {
    return Vector3(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}

inline std::istream &operator>>(std::istream &is, Vector3 &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2]; // PPM format
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const Vector3 &t) {
    os << int(t.e[0]) << " " << int(t.e[1]) << " " << int(t.e[2]) << std::endl; // PPM format
    return os;
}

inline Vector3 &Vector3::operator+=(const Vector3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

inline Vector3 &Vector3::operator+=(const double t) {
    double k = 1.0/t;
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

inline Vector3 &Vector3::operator-=(const Vector3 &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline Vector3 &Vector3::operator*=(const Vector3 &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline Vector3 &Vector3::operator*=(const double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline Vector3 &Vector3::operator/=(const Vector3 &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline Vector3 &Vector3::operator/=(const double t) {
    double k = 1.0 / t;
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

#endif
