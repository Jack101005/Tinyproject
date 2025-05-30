#ifndef LINEARSYSTEM_HPP
#define LINEARSYSTEM_HPP

#include "Matrix.hpp"
#include "Vector.hpp"

class LinearSystem {
protected:
    int mSize;
    Matrix* mpA;
    Vector* mpb;

private:
    LinearSystem(const LinearSystem&) = delete; // Prevent copy constructor
    LinearSystem& operator=(const LinearSystem&) = delete; // Prevent assignment

public:
    LinearSystem(Matrix& A, Vector& b);
    virtual ~LinearSystem();

    virtual Vector Solve() const;
};

class PosSymLinSystem : public LinearSystem {
public:
    PosSymLinSystem(Matrix& A, Vector& b);
    Vector Solve() const override;
};

#endif