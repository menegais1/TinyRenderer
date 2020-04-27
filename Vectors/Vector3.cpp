//
// Created by menegais1 on 26/04/2020.
//

#include "Vector3.h"
#include <cmath>
#include <stdexcept>

template<class t>
Vector3<t>::Vector3(t x, t y, t z) : x(x), y(y), z(z) {

}

template<class t>
Vector3<t>::Vector3() : x(0), y(0), z(0) {

}

template<class t>
Vector3<t> Vector3<t>::operator+(Vector3<t> v2) {
    return Vector3<t>(x + v2.x, y + v2.y, z + v2.z);
}

template<class t>
Vector3<t> Vector3<t>::operator-(Vector3<t> v2) {
    return Vector3<t>(x - v2.x, y - v2.y, z - v2.z);
}

template<class t>
Vector3<t> Vector3<t>::operator*(t v) {
    return Vector3<t>(x * v, y * v, z * v);
}

template<class t>
Vector3<t> Vector3<t>::operator/(t v) {
    return Vector3<t>(x / v, y / v, z / v);
}

template<class t>
Vector3<t> Vector3<t>::operator+=(Vector3<t> v1) {
    x += v1.x;
    y += v1.y;
    z += v1.z;
}

template<class t>
Vector3<t> Vector3<t>::operator-=(Vector3<t> v1) {
    x -= v1.x;
    y -= v1.y;
    z -= v1.z;
}

template<class t>
Vector3<t> Vector3<t>::operator/=(t v) {
    x /= v;
    y /= v;
    z /= v;
}

template<class t>
Vector3<t> Vector3<t>::operator*=(t v) {
    x *= v;
    y *= v;
    z *= v;
}


template<class t>
t Vector3<t>::length() {
    return std::sqrt(x * x + y * y + z * z);
}

template<class t>
Vector3<t> Vector3<t>::unit() {
    t l = length();
    return Vector3<t>(x / l, y / l, z / l);
}

template<class t>
t Vector3<t>::dot(Vector3<t> v2) {
    return x * v2.x + y * v2.y + z * v2.z;
}

template<class t>
Vector3<t> Vector3<t>::cross(Vector3<t> v2) {
    return Vector3<t>(y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x);
}

template<class t>
t Vector3<t>::angle(Vector3<t> v2) {
    float cosA = dot(v2) / (length() * v2.length());
    if (cosA > 1 || cosA < -1) cosA = (int) cosA;
    return acos(cosA);
}

template<class t>
t &Vector3<t>::operator[](int idx) {
    if (idx == 0) return x;
    if (idx == 1) return y;
    if (idx == 2) return z;
    else
        throw std::out_of_range("Index out of bounds");
}

template
class Vector3<float>;

template
class Vector3<double>;

template
class Vector3<int>;
