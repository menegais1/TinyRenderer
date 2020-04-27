//
// Created by menegais1 on 26/04/2020.
//

#ifndef TINYRENDERER_VECTOR2_H
#define TINYRENDERER_VECTOR2_H

template<class t>
struct Vector2 {
public:
    t x, y;

    Vector2(t x, t y);

    Vector2();

    Vector2<t> operator+(Vector2<t> v2);

    Vector2<t> operator-(Vector2<t> v2);

    Vector2<t> operator*(t v);

    Vector2<t> operator/(t v);

    Vector2<t> operator+=(Vector2<t> v1);

    Vector2<t> operator-=(Vector2<t> v1);

    Vector2<t> operator/=(t v);

    Vector2<t> operator*=(t v);

    bool operator==(Vector2<t> v);

    t &operator[](int idx);

    t length();

    Vector2<t> unit();

    t dot(Vector2<t> v2);

    Vector2<t> cross(Vector2<t> v2);
};

typedef Vector2<int> ivec2;
typedef Vector2<double> dvec2;
typedef Vector2<float> fvec2;
#endif //TINYRENDERER_VECTOR2_H
