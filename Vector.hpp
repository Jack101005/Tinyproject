#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cassert>
#include <stdexcept>

class Vector {
private:
    int mSize;
    double* mData;

public:
    // Constructors
    Vector(int size);
    Vector(const Vector& other);
    ~Vector();

    // Assignment operator
    Vector& operator=(const Vector& other);

    // Unary operators
    Vector operator-() const;

    // Binary operators
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(double scalar) const;
    friend Vector operator*(double scalar, const Vector& vec);

    // Access operators
    double& operator[](int index);
    const double& operator[](int index) const;
    double& operator()(int index);
    const double& operator()(int index) const;

    // Get size
    int size() const { return mSize; }

    // Dot product
    double dot(const Vector& other) const;
};

#endif