#include "Matrix.hpp"
#include "Vector.hpp"
#include <cassert>
#include <cmath>
#include <stdexcept>

Matrix::Matrix(int numRows, int numCols) : mNumRows(numRows), mNumCols(numCols) {
    assert(numRows > 0 && numCols > 0);
    mData = new double*[numRows];
    for (int i = 0; i < numRows; ++i) {
        mData[i] = new double[numCols]();
    }
}

Matrix::Matrix(const Matrix& other) : mNumRows(other.mNumRows), mNumCols(other.mNumCols) {
    mData = new double*[mNumRows];
    for (int i = 0; i < mNumRows; ++i) {
        mData[i] = new double[mNumCols];
        for (int j = 0; j < mNumCols; ++j) {
            mData[i][j] = other.mData[i][j];
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < mNumRows; ++i) {
        delete[] mData[i];
    }
    delete[] mData;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        for (int i = 0; i < mNumRows; ++i) {
            delete[] mData[i];
        }
        delete[] mData;
        mNumRows = other.mNumRows;
        mNumCols = other.mNumCols;
        mData = new double*[mNumRows];
        for (int i = 0; i < mNumRows; ++i) {
            mData[i] = new double[mNumCols];
            for (int j = 0; j < mNumCols; ++j) {
                mData[i][j] = other.mData[i][j];
            }
        }
    }
    return *this;
}

Matrix Matrix::operator-() const {
    Matrix result(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i) {
        for (int j = 0; j < mNumCols; ++j) {
            result.mData[i][j] = -mData[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix& other) const {
    assert(mNumRows == other.mNumRows && mNumCols == other.mNumCols);
    Matrix result(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i) {
        for (int j = 0; j < mNumCols; ++j) {
            result.mData[i][j] = mData[i][j] + other.mData[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    assert(mNumRows == other.mNumRows && mNumCols == other.mNumCols);
    Matrix result(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i) {
        for (int j = 0; j < mNumCols; ++j) {
            result.mData[i][j] = mData[i][j] - other.mData[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i) {
        for (int j = 0; j < mNumCols; ++j) {
            result.mData[i][j] = mData[i][j] * scalar;
        }
    }
    return result;
}

Matrix operator*(double scalar, const Matrix& mat) {
    return mat * scalar;
}

Matrix Matrix::operator*(const Matrix& other) const {
    assert(mNumCols == other.mNumRows);
    Matrix result(mNumRows, other.mNumCols);
    for (int i = 0; i < mNumRows; ++i) {
        for (int j = 0; j < other.mNumCols; ++j) {
            for (int k = 0; k < mNumCols; ++k) {
                result.mData[i][j] += mData[i][k] * other.mData[k][j];
            }
        }
    }
    return result;
}

Vector Matrix::operator*(const Vector& vec) const {
    assert(mNumCols == vec.size());
    Vector result(mNumRows);
    for (int i = 1; i <= mNumRows; ++i) {
        for (int j = 1; j <= mNumCols; ++j) {
            result(i) += mData[i - 1][j - 1] * vec(j);
        }
    }
    return result;
}

double& Matrix::operator()(int i, int j) {
    if (i < 1 || i > mNumRows || j < 1 || j > mNumCols) {
        throw std::out_of_range("Matrix index out of range (1-based)");
    }
    return mData[i - 1][j - 1];
}

const double& Matrix::operator()(int i, int j) const {
    if (i < 1 || i > mNumRows || j < 1 || j > mNumCols) {
        throw std::out_of_range("Matrix index out of range (1-based)");
    }
    return mData[i - 1][j - 1];
}

double Matrix::determinant() const {
    assert(mNumRows == mNumCols);
    if (mNumRows == 1) return mData[0][0];
    if (mNumRows == 2) {
        return mData[0][0] * mData[1][1] - mData[0][1] * mData[1][0];
    }
    double det = 0.0;
    Matrix temp(mNumRows - 1, mNumCols - 1);
    for (int j = 0; j < mNumCols; ++j) {
        for (int i = 1; i < mNumRows; ++i) {
            for (int k = 0, col = 0; k < mNumCols; ++k) {
                if (k == j) continue;
                temp.mData[i - 1][col++] = mData[i][k];
            }
        }
        det += (j % 2 == 0 ? 1 : -1) * mData[0][j] * temp.determinant();
    }
    return det;
}

Matrix Matrix::inverse() const {
    assert(mNumRows == mNumCols);
    double det = determinant();
    if (std::abs(det) < 1e-10) throw std::runtime_error("Matrix is singular");
    Matrix result(mNumRows, mNumCols);
    Matrix temp(mNumRows - 1, mNumCols - 1);
    for (int i = 0; i < mNumRows; ++i) {
        for (int j = 0; j < mNumCols; ++j) {
            for (int r = 0, row = 0; r < mNumRows; ++r) {
                if (r == i) continue;
                for (int c = 0, col = 0; c < mNumCols; ++c) {
                    if (c == j) continue;
                    temp.mData[row][col++] = mData[r][c];
                }
                ++row;
            }
            result.mData[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * temp.determinant() / det;
        }
    }
    return result;
}

Matrix Matrix::pseudoInverse() const {
    Matrix At(mNumCols, mNumRows);
    for (int i = 0; i < mNumCols; ++i) {
        for (int j = 0; j < mNumRows; ++j) {
            At.mData[i][j] = mData[j][i];
        }
    }
    Matrix AtA = At * (*this);
    try {
        Matrix AtA_inv = AtA.inverse();
        return AtA_inv * At;
    } catch (const std::runtime_error&) {
        double lambda = 1e-4;
        Matrix reg(mNumRows, mNumRows);
        for (int i = 0; i < mNumRows; ++i) {
            reg.mData[i][i] = lambda;
        }
        return (AtA + reg).inverse() * At;
    }
}