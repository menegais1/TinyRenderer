//
// Created by menegais1 on 28/04/2020.
//

#ifndef TINYRENDERER_MATRIX_H
#define TINYRENDERER_MATRIX_H

#include <cstdlib>
#include <vector>

template<class T>
class Matrix {
public:
    std::vector<std::vector<T>> m;
    int rows, cols;
    Matrix<T>(int rows, int cols);

    std::vector<T> &operator[](int idx);

    Matrix<T> operator*(Matrix<T> Mat);

    void printMatrix();
};


#endif //TINYRENDERER_MATRIX_H
