//
// Created by menegais1 on 28/04/2020.
//

#include <iostream>
#include "Matrix.h"

template<class T>
Matrix<T>::Matrix(int rows, int cols) : rows(rows), cols(cols) {
    m = std::vector<std::vector<T>>(rows);
    for (int i = 0; i < rows; ++i) {
        m[i] = std::vector<T>(cols);
    }
}

template<class T>
std::vector<T> &Matrix<T>::operator[](int idx) {
    if (idx >= 0 && idx < rows) return m[idx];
}

template<class T>
Matrix<T> Matrix<T>::operator*(Matrix<T> Mat) {
    if (Mat.rows != this->cols) return Mat;
    Matrix<T> Result(rows, Mat.cols);

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < Mat.cols; ++x) {
            for (int i = 0; i < cols; ++i) {
                Result[y][x] += m[y][i] * Mat[i][x];
            }
        }
    }
    return Result;
}

template<class T>
void Matrix<T>::printMatrix() {
    std::cout << std::endl;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            std::cout <<" " << m[y][x];
        }
        std::cout << std::endl;
    }
}

template
class Matrix<float>;

