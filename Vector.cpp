#include "Vector.hpp"
#include <cstring>

Vector::Vector(int size) : mSize(size) {
    assert(size >= 0);
    mData = new double[size]();
}

Vector::Vector(const Vector& other) : mSize(other.mSize) {
    mData = new double[mSize];
    std::memcpy(mData, other.mData, mSize * sizeof(double));
}

Vector::~Vector() {
    delete[] mData;
}

Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        delete[] mData;
        mSize = other.mSize;
        mData = new double[mSize];
        std::memcpy(mData, other.mData, mSize * sizeof(double));
    }
    return *this;
}

Vector Vector::operator-() const {
    Vector result(mSize);
    for (int i = 0; i < mSize; ++i) {
        result.mData[i] = -mData[i];
    }
    return result;
}

Vector Vector::operator+(const Vector& other) const {
    assert(mSize == other.mSize);
    Vector result(mSize);
    for (int i = 0; i < mSize; ++i) {
        result.mData[i] = mData[i] + other.mData[i];
    }
    return result;
}

Vector Vector::operator-(const Vector& other) const {
    assert(mSize == other.mSize);
    Vector result(mSize);
    for (int i = 0; i < mSize; ++i) {
        result.mData[i] = mData[i] - other.mData[i];
    }
    return result;
}

Vector Vector::operator*(double scalar) const {
    Vector result(mSize);
    for (int i = 0; i < mSize; ++i) {
        result.mData[i] = mData[i] * scalar;
    }
    return result;
}

Vector operator*(double scalar, const Vector& vec) {
    return vec * scalar;
}

double& Vector::operator[](int index) {
    if (index < 0 || index >= mSize) {
        throw std::out_of_range("Vector index out of range");
    }
    return mData[index];
}

const double& Vector::operator[](int index) const {
    if (index < 0 || index >= mSize) {
        throw std::out_of_range("Vector index out of range");
    }
    return mData[index];
}

double& Vector::operator()(int index) {
    if (index < 1 || index > mSize) {
        throw std::out_of_range("Vector index out of range (1-based)");
    }
    return mData[index - 1];
}

const double& Vector::operator()(int index) const {
    if (index < 1 || index > mSize) {
        throw std::out_of_range("Vector index out of range (1-based)");
    }
    return mData[index - 1];
}

double Vector::dot(const Vector& other) const {
    assert(mSize == other.mSize);
    double result = 0.0;
    for (int i = 0; i < mSize; ++i) {
        result += mData[i] * other.mData[i];
    }
    return result;
}