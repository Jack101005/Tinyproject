#include "LinearSystem.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

LinearSystem::LinearSystem(Matrix& A, Vector& b) : mSize(A.getRows()), mpA(&A), mpb(&b) {
    assert(A.getRows() == A.getCols() && A.getRows() == b.size());
}

LinearSystem::~LinearSystem() {}

Vector LinearSystem::Solve() const {
    // Gaussian elimination with partial pivoting
    Matrix A(*mpA);
    Vector b(*mpb);
    Vector x(mSize);

    for (int k = 0; k < mSize - 1; ++k) {
        // Find pivot
        int maxRow = k;
        double maxVal = std::abs(A(k + 1, k + 1));
        for (int i = k + 1; i < mSize; ++i) {
            if (std::abs(A(i + 1, k + 1)) > maxVal) {
                maxVal = std::abs(A(i + 1, k + 1));
                maxRow = i;
            }
        }
        if (maxVal < 1e-10) throw std::runtime_error("Matrix is singular");

        // Swap rows
        if (maxRow != k) {
            for (int j = k; j < mSize; ++j) {
                std::swap(A(k + 1, j + 1), A(maxRow + 1, j + 1));
            }
            std::swap(b(k + 1), b(maxRow + 1));
        }

        // Eliminate
        for (int i = k + 1; i < mSize; ++i) {
            double factor = A(i + 1, k + 1) / A(k + 1, k + 1);
            for (int j = k; j < mSize; ++j) {
                A(i + 1, j + 1) -= factor * A(k + 1, j + 1);
            }
            b(i + 1) -= factor * b(k + 1);
        }
    }

    // Back substitution
    for (int i = mSize; i >= 1; --i) {
        double sum = 0.0;
        for (int j = i + 1; j <= mSize; ++j) {
            sum += A(i, j) * x(j);
        }
        x(i) = (b(i) - sum) / A(i, i);
    }
    return x;
}

PosSymLinSystem::PosSymLinSystem(Matrix& A, Vector& b) : LinearSystem(A, b) {
    // Check symmetry
    for (int i = 1; i <= mSize; ++i) {
        for (int j = i + 1; j <= mSize; ++j) {
            if (std::abs(A(i, j) - A(j, i)) > 1e-10) {
                throw std::runtime_error("Matrix is not symmetric");
            }
        }
    }
}

Vector PosSymLinSystem::Solve() const {
    // Conjugate gradient method
    Vector x(mSize);
    Vector r = *mpb - (*mpA) * x;
    Vector p = r;
    double rsold = r.dot(r);

    for (int i = 0; i < mSize; ++i) {
        Vector Ap = (*mpA) * p;
        double alpha = rsold / p.dot(Ap);
        x = x + alpha * p;
        r = r - alpha * Ap;
        double rsnew = r.dot(r);
        if (std::sqrt(rsnew) < 1e-10) break;
        p = r + (rsnew / rsold) * p;
        rsold = rsnew;
    }
    return x;
}