//
// Created by menegais1 on 26/04/2020.
//

#include "Vector2.h"
#include <cmath>

template<class t>
Vector2<t>::Vector2(t x, t y) : x(x), y(y) {

}

template<class t>
Vector2<t>::Vector2() : x(0), y(0) {

}

template<class t>
Vector2<t> Vector2<t>::operator+(Vector2<t> v2) {
    return Vector2<t>(x + v2.x, y + v2.y);
}

template<class t>
Vector2<t> Vector2<t>::operator-(Vector2<t> v2) {
    return Vector2<t>(x - v2.x, y - v2.y);
}

template<class t>
Vector2<t> Vector2<t>::operator*(t v) {
    return Vector2<t>(x * v, y * v);
}

template<class t>
Vector2<t> Vector2<t>::operator/(t v) {
    return Vector2<t>(x / v, y / v);
}

template<class t>
Vector2<t> Vector2<t>::operator+=(Vector2<t> v1) {
    x += v1.x;
    y += v1.y;
}

template<class t>
Vector2<t> Vector2<t>::operator-=(Vector2<t> v1) {
    x -= v1.x;
    y -= v1.y;
}

template<class t>
Vector2<t> Vector2<t>::operator/=(t v) {
    x /= v;
    y /= v;
}

template<class t>
Vector2<t> Vector2<t>::operator*=(t v) {
    x *= v;
    y *= v;
}


template<class t>
t Vector2<t>::length() {
    return std::sqrt(x * x + y * y);
}

template<class t>
Vector2<t> Vector2<t>::unit() {
    t l = length();
    return Vector2<t>(x / l, y / l);
}

template<class t>
t Vector2<t>::dot(Vector2<t> v2) {
    return x * v2.x + y * v2.y;
}

template<class t>
Vector2<t> Vector2<t>::cross(Vector2<t> v2) {
    return Vector2<t>();
}

template class Vector2<float>;
template class Vector2<double>;
template class Vector2<int>;