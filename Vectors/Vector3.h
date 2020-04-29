//
// Created by menegais1 on 26/04/2020.
//

#ifndef TINYRENDERER_VECTOR3_H
#define TINYRENDERER_VECTOR3_H

#include "Matrix.h"

template<class t>
class Matrix;

template<class t>
struct Vector3 {
public:
    t x, y, z;

    Vector3(t x, t y, t z);

    Vector3();

    Vector3<t> operator+(Vector3<t> v2);

    Vector3<t> operator-(Vector3<t> v2);

    Vector3<t> operator-();

    Vector3<t> operator*(t v);

    Vector3<t> operator/(t v);

    Vector3<t> operator+=(Vector3<t> v1);

    Vector3<t> operator-=(Vector3<t> v1);

    Vector3<t> operator/=(t v);

    Vector3<t> operator*=(t v);

    t &operator[](int idx);

    bool operator==(const Vector3 &rhs) const;

    bool operator!=(const Vector3 &rhs) const;

    Vector3<t> &operator=(Matrix<double> rhs);

    void print();

    t length();

    Vector3<t> unit();

    t dot(Vector3<t> v2);

    t angle(Vector3<t> v2);

    Vector3<t> cross(Vector3<t> v2);

};

typedef Vector3<int> ivec3;
typedef Vector3<double> dvec3;
typedef Vector3<float> fvec3;

#endif //TINYRENDERER_VECTOR3_H
