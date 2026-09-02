#ifndef MATRIX_IMPL_HPP
#define MATRIX_IMPL_HPP

#include "matrix.hpp"


template <typename T>
Matrix<T>::Row_matr::Row_matr(size_t cols, T *row) : m_cols(cols), m_row(row) {}

template <typename T>
const T &Matrix<T>::Row_matr::operator[](size_t i) const
{
    assert(i < m_cols);
    return m_row[i];
}

template <typename T>
T &Matrix<T>::Row_matr::operator[](size_t i)
{
    assert(i < m_cols);
    return m_row[i];
}


template <typename T>
long double Matrix<T>::threshold = DEFAULT_THRESHOLD;


template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols) : m_matr(nullptr), m_rows(rows), m_cols(cols)
{
    Alloc();
    ForEach([](int, int) { return T{}; });
}

template <typename T>
template <typename It>
Matrix<T>::Matrix(size_t rows, size_t cols, It begin, It end)
    : m_matr(nullptr), m_rows(rows), m_cols(cols)
{
    Alloc();
    FillByIt(begin, end);
}

template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols, const std::initializer_list<T> &ilist)
    : m_matr(nullptr), m_rows(rows), m_cols(cols)
{
    Alloc();
    FillByIt(ilist.begin(), ilist.end());
}

template <typename T>
template <typename empl_func>
Matrix<T>::Matrix(size_t rows, size_t cols, empl_func fnc)
    : m_matr(nullptr), m_rows(rows), m_cols(cols)
{
    Alloc();
    ForEach(fnc);
}


template <typename T>
template <typename func>
void Matrix<T>::ForEach(func f)
{
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            m_matr[i][j] = f(i, j);
}


template <typename T>
Matrix<T>::Matrix(const Matrix &matr) : m_matr(nullptr), m_rows(matr.m_rows), m_cols(matr.m_cols)
{
    Alloc();
    Copy(*this, matr);
}

template <typename T>
Matrix<T>::Matrix(Matrix &&matr) noexcept
    : m_matr(matr.m_matr), m_rows(matr.m_rows), m_cols(matr.m_cols)
{
    matr.m_matr = nullptr;
    matr.m_rows = matr.m_cols = 0;
}


template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix &matr)
{
    if (this == &matr)
        return *this;

    if (m_rows == matr.m_rows && m_cols == matr.m_cols)
        Copy(*this, matr);
    else
    {
        Matrix tmp(matr);
        Swap(*this, tmp);
    }

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(Matrix &&matr) noexcept
{
    if (this == &matr)
        return *this;

    Matrix tmp(std::move(matr));
    Swap(*this, tmp);

    return *this;
}



template <typename T>
Matrix<T> &Matrix<T>::operator+=(const Matrix &matr)
{
    assert(matr.m_rows == m_rows && matr.m_cols == m_cols);

    auto add_fnc = [&](int i, int j) { return m_matr[i][j] + matr.m_matr[i][j]; };
    ForEach(add_fnc);

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(const Matrix &matr)
{
    assert(matr.m_rows == m_rows && matr.m_cols == m_cols);
    auto add_fnc = [&](int i, int j) { return m_matr[i][j] - matr.m_matr[i][j]; };
    ForEach(add_fnc);

    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix& matr)
{
    assert(m_cols == matr.m_rows);
    
    Matrix<T> BT = matr.Transposing();
    
    // Результат
    Matrix<T> result(m_rows, matr.m_cols);
    
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < matr.m_cols; ++j) {
            T sum = T(0);
            for (size_t k = 0; k < m_cols; ++k) {
                sum += m_matr[i][k] * BT[j][k];
            }
            result.m_matr[i][j] = sum;
        }
    }
    
    Swap(*this, result);
    return *this;
}
template <typename T>
Matrix<T> &Matrix<T>::operator*=(T val)
{
    auto mul_fnc = [&](int i, int j) { return m_matr[i][j] * val; };
    ForEach(mul_fnc);

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::Transpose()
{
    if (m_rows == m_cols)
        return Transpose_Quad();

    Matrix<T> temp{m_cols, m_rows, [&](int i, int j) { return m_matr[j][i]; }};

    Swap(*this, temp);

    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::Transposing() const
{
    return Matrix<T>(m_cols, m_rows, [&](int i, int j) { return m_matr[j][i]; });
}

template <typename T>
Matrix<T> &Matrix<T>::Transpose_Quad()
{
    for (size_t i = 0; i < m_cols; ++i)
        for (size_t j = i + 1; j < m_cols; ++j)
            std::swap(m_matr[i][j], m_matr[j][i]);

    return *this;
}


template <typename T>
int Matrix<T>::FindNonZero(size_t st_col) const
{
    for (size_t i = st_col + 1; i < m_cols; ++i)
        if (!IsZero(m_matr[i][st_col]))
            return i;

    return -1;
}

template <typename T>
long double Matrix<T>::Det() const
{
    if (m_rows != m_cols)
        return NAN;

    if (!std::is_arithmetic_v<T>)
        return NAN;

    int sign = 1;
    Matrix<T> tmp{*this};

    for (size_t i = 0; i < m_rows - 1; ++i)
    {
        if (IsZero(tmp.m_matr[i][i]))
        {
            int non_z_line = tmp.FindNonZero(i);
            if (non_z_line == -1)
                return 0;

            tmp.SwapLines(non_z_line, i);
            sign = -sign;
        }

        T div = tmp.m_matr[i][i];
        for (size_t j = i + 1; j < m_rows; ++j)
            tmp.AddLineMVal(j, i, -static_cast<long double>(tmp.m_matr[j][i]) / div);
    }

    long double det = sign;
    for (size_t i = 0; i < m_rows; ++i)
        det *= tmp.m_matr[i][i];

    return det;
}


template <typename T>
Matrix<T> Matrix<T>::Identity(size_t rows)
{
    Matrix id(rows, rows, [](int i, int j) { return i == j; });
    return id;
}


template <typename T>
const T &Matrix<T>::At(size_t i, size_t j) const
{
    assert(i < m_rows);
    assert(j < m_cols);
    return m_matr[i][j];
}

template <typename T>
typename Matrix<T>::Row_matr Matrix<T>::operator[](size_t i) const
{
    assert(i < m_rows);
    return Row_matr{m_cols, m_matr[i]};
}

template <typename T>
typename Matrix<T>::Row_matr Matrix<T>::operator[](size_t i)
{
    assert(i < m_rows);
    return Row_matr{m_cols, m_matr[i]};
}


template <typename T>
void Matrix<T>::SwapLines(size_t lhs, size_t rhs)
{
    assert(lhs < m_rows);
    assert(rhs < m_rows);
    std::swap(m_matr[lhs], m_matr[rhs]);
}

template <typename T>
void Matrix<T>::AddLine(size_t dest_ind, size_t src_ind)
{
    assert(dest_ind < m_rows);
    assert(src_ind < m_rows);

    for (int i = 0; i < m_cols; ++i)
        m_matr[dest_ind][i] += m_matr[src_ind][i];
}

template <typename T>
void Matrix<T>::AddLineMVal(size_t dest_ind, size_t src_ind, long double val)
{
    assert(dest_ind < m_rows);
    assert(src_ind < m_rows);

    for (size_t i = 0; i < m_cols; ++i)
        m_matr[dest_ind][i] += val * m_matr[src_ind][i];
}

template <typename T>
void Matrix<T>::MulLine(size_t line, long double val)
{
    assert(line < m_rows);

    for (int i = 0; i < m_cols; ++i)
        m_matr[line][i] *= val;
}

template <typename T>
Matrix<T>::~Matrix()
{
    for (size_t i = 0; i < m_rows; ++i)
        delete[] m_matr[i];

    delete[] m_matr;
    m_matr = nullptr;
    m_rows = m_cols = 0;
}


template <typename T>
bool Matrix<T>::IsEq(const Matrix &matr) const
{
    if (std::is_arithmetic_v<T>)
    {
        for (size_t i = 0; i < m_rows; ++i)
            for (size_t j = 0; j < m_cols; ++j)
                if (!IsZero(m_matr[i][j] - matr.m_matr[i][j]))
                    return false;
    }
    else
    {
        for (size_t i = 0; i < m_rows; ++i)
            for (size_t j = 0; j < m_cols; ++j)
                if (m_matr[i][j] != matr.m_matr[i][j])
                    return false;
    }
    return true;
}

template <typename T>
void Matrix<T>::Dump(std::ostream &ost) const
{
    for (size_t i = 0; i < m_rows; ++i)
    {
        ost << "|| ";
        for (size_t j = 0; j < m_cols; ++j)
            ost << m_matr[i][j] << (j == m_cols - 1 ? "" : ", ");
        ost << " ||\n";
    }
}

template <typename T>
void Matrix<T>::Alloc()
{
    m_matr = new T *[m_rows];

    for (size_t i = 0; i < m_rows; ++i)
        m_matr[i] = new T[m_cols];
}

template <typename T>
template <typename It>
void Matrix<T>::FillByIt(It begin, It end)
{
    size_t i = 0, size = m_rows * m_cols;

    for (It it = begin; it != end && i < size; ++it, ++i)
        m_matr[i / m_cols][i % m_cols] = *it;
}

template <typename T>
void Matrix<T>::Swap(Matrix &lhs, Matrix &rhs)
{
    std::swap(lhs.m_matr, rhs.m_matr);
    std::swap(lhs.m_cols, rhs.m_cols);
    std::swap(lhs.m_rows, rhs.m_rows);
}

template <typename T>
void Matrix<T>::Copy(Matrix &dst, const Matrix &src)
{
    assert(dst.m_rows == src.m_rows);
    assert(dst.m_cols == src.m_cols);

    for (size_t i = 0; i < dst.m_rows; ++i)
        for (size_t j = 0; j < dst.m_cols; ++j)
            dst.m_matr[i][j] = src.m_matr[i][j];
}

template <typename T>
bool operator==(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    return lhs.IsEq(rhs);
}

template <typename T>
Matrix<T> operator+(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    Matrix<T> temp{lhs};
    temp += rhs;
    return temp;
}

template <typename T>
Matrix<T> operator-(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    Matrix<T> temp{lhs};
    temp -= rhs;
    return temp;
}

template <typename T>
Matrix<T> operator*(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    Matrix<T> temp{lhs};
    temp *= rhs;
    return temp;
}

template <typename T>
Matrix<T> operator*(const Matrix<T> &lhs, T val)
{
    Matrix<T> temp{lhs};
    temp *= val;
    return temp;
}

template <typename T>
Matrix<T> operator*(T val, const Matrix<T> &rhs)
{
    return rhs * val;
}

template <typename T>
std::ostream &operator<<(std::ostream &ost, const Matrix<T> &matr)
{
    matr.Dump(ost);
    return ost;
}

template <typename T>
std::istream &operator>>(std::istream &ist, Matrix<T> &matr)
{
    size_t rows = 0, cols = 0;
    ist >> rows >> cols;

    matr = Matrix<T>{rows, cols, [&](int, int)
    {
        T val{};
        ist >> val;
        return val;
    }};

    return ist;
}

template <typename T>
std::istream &InputQuadr(std::istream &ist, Matrix<T> &matr)
{
    size_t size = 0;
    ist >> size;

    matr = Matrix<T>{size, size, [&](int, int)
    {
        T val{};
        ist >> val;
        return val;
    }};

    return ist;
}


#endif // MATRIX_IMPL_HPP
