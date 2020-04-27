//
// Created by menegais1 on 26/04/2020.
//

#include "Vector3.h"
#include <cmath>

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
    return Vector3<t>();
}

template
class Vector3<float>;

template
class Vector3<double>;

template
class Vector3<int>;
