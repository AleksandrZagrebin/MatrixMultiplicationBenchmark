#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <cmath>
#include <cassert>
#include <type_traits>

template <typename T>
class Matrix
{
    static_assert(std::is_arithmetic_v<T>,
        "Matrix can only be instantiated with arithmetic types");

private:
    T **m_matr;
    size_t m_rows;
    size_t m_cols;
    static long double threshold;

    struct Row_matr
    {
        size_t m_cols;
        T *m_row;

        Row_matr(size_t cols, T *row);
        const T &operator[](size_t i) const;
        T &operator[](size_t i);
        Row_matr(const Row_matr &row_m) = default;
        Row_matr &operator=(const Row_matr &row_m) = default;
    };

public:
    Matrix(size_t rows, size_t cols);

    template <typename It>
    Matrix(size_t rows, size_t cols, It begin, It end);

    Matrix(size_t rows, size_t cols, const std::initializer_list<T> &ilist);

    template <typename empl_func>
    Matrix(size_t rows, size_t cols, empl_func fnc);

    Matrix(const Matrix &matr);
    Matrix(Matrix &&matr) noexcept;

    Matrix &operator=(const Matrix &matr);
    Matrix &operator=(Matrix &&matr) noexcept;

    Matrix &operator+=(const Matrix &matr);
    Matrix &operator-=(const Matrix &matr);
    Matrix &operator*=(const Matrix &matr);
    Matrix &operator*=(T val);

    Matrix &Transpose();
    Matrix Transposing() const;

    long double Det() const;

    size_t getCols() const { return m_cols; }
    size_t getRows() const { return m_rows; }

    static Matrix Identity(size_t rows);

    const T &At(size_t i, size_t j) const;

    Row_matr operator[](size_t i) const;
    Row_matr operator[](size_t i);

    ~Matrix();

    void AddLineMVal(size_t dest_ind, size_t src_ind, long double val);

    bool IsEq(const Matrix &matr) const;

    void Dump(std::ostream &ost) const;

    static void SetThreshold(long double new_thres) { threshold = new_thres; }
    static void SetDefThres() { threshold = DEFAULT_THRESHOLD; }
    static long double GetThreshold() { return threshold; }
    static bool IsZero(long double val) { return std::abs(val) < threshold; }

private:
    static constexpr long double DEFAULT_THRESHOLD = 1e-10;

    Matrix &Transpose_Quad();

    void Alloc();

    template <typename It>
    void FillByIt(It begin, It end);

    static void Swap(Matrix &lhs, Matrix &rhs);
    static void Copy(Matrix &dst, const Matrix &src);

    void SwapLines(size_t lhs, size_t rhs);
    void AddLine(size_t dest_ind, size_t src_ind);
    void MulLine(size_t line, long double val);

    int FindNonZero(size_t st_col) const;

    template <typename func>
    void ForEach(func f);
};

template <typename T>
bool operator==(const Matrix<T> &lhs, const Matrix<T> &rhs);

template <typename T>
Matrix<T> operator+(const Matrix<T> &lhs, const Matrix<T> &rhs);

template <typename T>
Matrix<T> operator-(const Matrix<T> &lhs, const Matrix<T> &rhs);

template <typename T>
Matrix<T> operator*(const Matrix<T> &lhs, const Matrix<T> &rhs);

template <typename T>
Matrix<T> operator*(const Matrix<T> &lhs, T val);

template <typename T>
Matrix<T> operator*(T val, const Matrix<T> &lhs);

template <typename T>
std::ostream &operator<<(std::ostream &ost, const Matrix<T> &matr);

template <typename T>
std::istream &operator>>(std::istream &ist, Matrix<T> &matr);

template <typename T>
std::istream &InputQuadr(std::istream &ist, Matrix<T> &matr);

#include "matrix_impl.hpp"

#endif // MATRIX_HPP
