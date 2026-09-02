#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <vector>

#include "matrix.hpp"

std::mt19937& GetRNG()
{
    static std::mt19937 rng(
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    return rng;
}

template<typename T>
Matrix<T> CreateRandomMatrix(size_t rows, size_t cols, T min_val, T max_val)
{
    auto& rng = GetRNG();
    std::uniform_int_distribution<T> dist(min_val, max_val);
    return Matrix<T>(rows, cols, [&](int, int) { return dist(rng); });
}

template<typename T>
double MeasureMultiplication(const Matrix<T>& A, const Matrix<T>& B, bool optimized)
{
    auto start = std::chrono::high_resolution_clock::now();

    if (optimized)
        Matrix<T> D = Optimizemult(A, B);
    else
        Matrix<T> C = A * B;
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return static_cast<double>(ms.count());
}

int main()
{
    std::vector<int> sizes = {100, 500, 1000, 1500};
    std::ofstream out_file("benchmark_results.txt");

    std::cout<<"Running Square Matrix Benchmark..."<<std::endl;
    for (int size : sizes)
    {
        Matrix<int> A = CreateRandomMatrix(size, size, -20, 100);
        Matrix<int> B = CreateRandomMatrix(size, size, -20, 100);

        double naive = MeasureMultiplication(A, B, false);
        double optimized = MeasureMultiplication(A, B, true);

        long long elements = static_cast<long long>(size) * size;

        std::cout << "Matrix " << size << "x" << size << " (" << elements << " elements)"
                  << " | Naive: " << naive << " ms"
                  << " | Optimized: " << optimized << " ms"
                  << " | Speedup: " << (optimized > 0 ? naive / optimized : 1.0) << "x\n";
        out_file << size << " " << elements << " " << naive << " " << optimized << "\n";
    }

    out_file.close();
    return 0;
}
~                 
