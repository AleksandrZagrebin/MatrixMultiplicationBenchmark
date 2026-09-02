#include <iostream>
#include <chrono>
#include <random>

#include "matrix.hpp"

std::mt19937& GetRNG()
 {
    static std::mt19937 rng
        (
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    return rng;
}

template<typename T>
Matrix<T> CreateRandomMatrix(size_t rows, size_t cols, T min_val, T max_val )
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

void maybe_readopt(int argc, char **argv, int n, int *x)
{
  if (argc > n)
  {
    char *flag = NULL;
    *x = strtol(argv[n], &flag, 0);
  }
}

enum DEFAULT {
        DEF_AX = 1200,
        DEF_AY = 800,
        DEF_BY = 1600,
};

int main(int argc, char* argv[])
{
    int ax = DEF_AX, ay = DEF_AY, by = DEF_BY;
    std::cout << "Hello from mult bench. Usage:\n" << argv[0] << " [ax] [ay] [by]" << std::endl;

    maybe_readopt(argc, argv, 1, &ax);
    maybe_readopt(argc, argv, 2, &ay);
    maybe_readopt(argc, argv, 3, &by);

    if (ax <= 0 || ay <= 0 || by <= 0) return 1;

#ifdef DEBUG
    Matrix<int> A = { 3,2,{1,2,3,4,5,6} };
    Matrix<int> B = { 2,6,{1,2,3,4,5,6,7,8,9,10,11,12} };
#else
    Matrix<int> A = CreateRandomMatrix(ax, ay, -20, 100);
    Matrix<int> B = CreateRandomMatrix(ay, by, -20, 100);
#endif

    double naive_time = MeasureMultiplication(A, B, false);

    double optimized_time = MeasureMultiplication(A, B, true);

    double speedup = (optimized_time > 0) ? naive_time / optimized_time : 1.0;

    std::cout << "\n=== Results ===\n";
    std::cout << "Naive multiplication: " << naive_time << " ms\n";
    std::cout << "Optimized multiplication: " << optimized_time << " ms\n";
    std::cout << "Speedup: " << speedup << "x\n";

#ifdef DEBUG
    std::cout << "\nMatrix A:\n";
    A.Dump(std::cout);
    std::cout << "\nMatrix B:\n";
    B.Dump(std::cout);
    std::cout << "\nNaive result C = A * B:\n";
    Matrix<int> C = A * B;
    C.Dump(std::cout);
    std::cout << "\nOptimized result D = Optimizemult(A, B):\n";
    Matrix<int> D = Optimizemult(A, B);
    D.Dump(std::cout);
#endif

    return 0;
}


~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
