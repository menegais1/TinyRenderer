//
// Created by menegais1 on 28/04/2020.
//

#ifndef TINYRENDERER_MATRIX_H
#define TINYRENDERER_MATRIX_H

#include <cstdlib>
#include <vector>
#include "Vector3.h"

template<class t>
class Vector3;

template<class T>
class Matrix {
public:
    std::vector<std::vector<T>> m;
    int rows, cols;

    Matrix<T>(int rows, int cols);

    Matrix<T>();

    std::vector<T> &operator[](int idx);

    Matrix<T> operator*(Matrix<T> Mat);

    Matrix<T> operator*(Vector3<T> v);

    static Matrix<T> vectorToMatrix(Vector3<T> vector);

    static Vector3<T> matrixToVector(Matrix<T> M);

    void printMatrix();

    Matrix<T> invert();
};


#endif //TINYRENDERER_MATRIX_H
