#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vector.hpp"
#include <cassert>
#include <stdexcept>

class Matrix {
private:
    int mNumRows;
    int mNumCols;
    double** mData;

public:
    // Constructors
    Matrix(int numRows, int numCols);
    Matrix(const Matrix& other);
    ~Matrix();

    // Assignment operator
    Matrix& operator=(const Matrix& other);

    // Unary operators
    Matrix operator-() const;

    // Binary operators
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(double scalar) const;
    Matrix operator*(const Matrix& other) const;
    Vector operator*(const Vector& vec) const;
    friend Matrix operator*(double scalar, const Matrix& mat);

    // Access operators
    double& operator()(int i, int j);
    const double& operator()(int i, int j) const;

    // Get dimensions
    int getRows() const { return mNumRows; }
    int getCols() const { return mNumCols; }

    // Matrix operations
    double determinant() const;
    Matrix inverse() const;
    Matrix pseudoInverse() const;
};

#endif