#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib> // For srand, rand
#include <ctime>   // For time
#include "Matrix.hpp"
#include "Vector.hpp"
#include "LinearSystem.hpp"

std::vector<std::vector<double>> loadDataset(const std::string& filename) {
    std::vector<std::vector<double>> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return data; // Returns empty vector
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<double> row;
        // Skip first two columns (vendor, model)
        for (int i = 0; i < 2 && std::getline(ss, token, ','); ++i);
        while (std::getline(ss, token, ',')) {
            row.push_back(std::stod(token));
        }
        if (row.size() == 7) { // MYCT, MMIN, MMAX, CACH, CHMIN, CHMAX, PRP
            data.push_back(row);
        }
    }
    file.close();
    if (data.empty()) {
        std::cerr << "Warning: No valid data loaded from " << filename << std::endl;
    }
    return data;
}

// Custom Fisher-Yates shuffle implementation
template<typename T>
void customShuffle(std::vector<T>& vec) {
    for (size_t i = vec.size() - 1; i > 0; --i) {
        size_t j = rand() % (i + 1); // Pick a random index from 0 to i
        std::swap(vec[i], vec[j]);
    }
}

int main() {
    auto data = loadDataset("machine.data");
    std::cout << "Total data size: " << data.size() << std::endl;

    if (data.empty()) {
        std::cerr << "Error: Dataset is empty. Please check machine.data file." << std::endl;
        return 1;
    }

    std::vector<std::vector<double>> trainData, testData;
    std::vector<int> indices(data.size());
    for (int i = 0; i < indices.size(); ++i) indices[i] = i;

    // Seed the random number generator and shuffle
    srand(static_cast<unsigned int>(time(nullptr)));
    customShuffle(indices); // Use custom shuffle instead of std::shuffle

    int trainSize = static_cast<int>(0.8 * data.size());
    std::cout << "Train size: " << trainSize << std::endl;
    for (int i = 0; i < trainSize; ++i) {
        trainData.push_back(data[indices[i]]);
    }
    for (int i = trainSize; i < data.size(); ++i) {
        testData.push_back(data[indices[i]]);
    }
    std::cout << "Test size: " << testData.size() << std::endl;

    // Create matrices with validated sizes
    Matrix A(trainSize, 6);
    Matrix A_test(testData.size(), 6);

    // Populate A and A_test
    for (int i = 0; i < trainSize; ++i) {
        for (int j = 0; j < 6; ++j) {
            A(i + 1, j + 1) = trainData[i][j]; // 1-based indexing
        }
    }
    for (int i = 0; i < testData.size(); ++i) {
        for (int j = 0; j < 6; ++j) {
            A_test(i + 1, j + 1) = testData[i][j]; // 1-based indexing
        }
    }

    // Create b and b_test (target variable PRP)
    Vector b(trainSize);
    Vector b_test(testData.size());
    for (int i = 0; i < trainSize; ++i) {
        b(i + 1) = trainData[i][6]; // PRP is the 7th column (0-based index 6)
    }
    for (int i = 0; i < testData.size(); ++i) {
        b_test(i + 1) = testData[i][6]; // PRP is the 7th column
    }

    // Solve linear system using Moore-Penrose inverse
    Matrix A_pinv = A.pseudoInverse();
    Vector x = A_pinv * b;

    // Compute RMSE on test set
    double rmse = 0.0;
    for (int i = 0; i < testData.size(); ++i) {
        double predicted = 0.0;
        for (int j = 0; j < 6; ++j) {
            predicted += x(j + 1) * A_test(i + 1, j + 1);
        }
        double error = predicted - b_test(i + 1);
        rmse += error * error;
    }
    rmse = std::sqrt(rmse / testData.size());
    std::cout << "RMSE on test set: " << rmse << std::endl;

    // Print model parameters
    std::cout << "Model parameters (x1, x2, x3, x4, x5, x6):" << std::endl;
    for (int i = 1; i <= 6; ++i) {
        std::cout << "x" << i << ": " << x(i) << std::endl;
    }

    return 0;
}