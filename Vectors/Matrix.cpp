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
            std::cout << " " << m[y][x];
        }
        std::cout << std::endl;
    }
}

template<class T>
Matrix<T>::Matrix() {
    Matrix<T>(4, 4);
}

template<class T>
Matrix<T> Matrix<T>::invert() {
    if (rows != cols) return Matrix<T>();
    // augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
    Matrix result(rows, cols * 2);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[i][j] = m[i][j];
    for (int i = 0; i < rows; i++)
        result[i][i + cols] = 1;
    // first pass
    for (int i = 0; i < rows - 1; i++) {
        // normalize the first row
        for (int j = result.cols - 1; j >= 0; j--)
            result[i][j] /= result[i][i];
        for (int k = i + 1; k < rows; k++) {
            float coeff = result[k][i];
            for (int j = 0; j < result.cols; j++) {
                result[k][j] -= result[i][j] * coeff;
            }
        }
    }
    // normalize the last row
    for (int j = result.cols - 1; j >= rows - 1; j--)
        result[rows - 1][j] /= result[rows - 1][rows - 1];
    // second pass
    for (int i = rows - 1; i > 0; i--) {
        for (int k = i - 1; k >= 0; k--) {
            float coeff = result[k][i];
            for (int j = 0; j < result.cols; j++) {
                result[k][j] -= result[i][j] * coeff;
            }
        }
    }
    // cut the identity matrix back
    Matrix truncate(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            truncate[i][j] = result[i][j + cols];
    return truncate;
}

template<class T>
Matrix<T> Matrix<T>::operator*(Vector3<T> v) {
    Matrix<T> M = vectorToMatrix(v);
    return *this * M;
}

template<class T>
Matrix<T> Matrix<T>::vectorToMatrix(Vector3<T> vector) {
    Matrix<T> V(4, 1);
    V[0] = {vector.x};
    V[1] = {vector.y};
    V[2] = {vector.z};
    V[3] = {1};
    return V;
}

template<class T>
Vector3<T> Matrix<T>::matrixToVector(Matrix<T> M) {
    return Vector3<T>(M[0][0] / M[3][0], M[1][0] / M[3][0], M[2][0] / M[3][0]);
}

template<class T>
Matrix<T> Matrix<T>::identity(int size) {
    Matrix<T> M(size, size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            M.m[i][j] = 0;
            if (i == j)
                M.m[i][j] = 1;
        }
    }
    return M;
}

template
class Matrix<double>;

