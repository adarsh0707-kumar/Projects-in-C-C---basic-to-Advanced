/**
 * @file Matrix.hpp
 * @brief A dense matrix type with the operations the calculator exposes.
 *
 * Storage is std::vector<std::vector<double>> -- RAII means this
 * class never needs an explicit destructor, copy constructor, or
 * assignment operator; the compiler-generated ones are correct
 * because vector already manages its own memory. This is the
 * concrete advantage C++ has here over a hand-rolled C malloc/free
 * 2D array: there's no exit path that can leak.
 */
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <string>

/** A dense rows x cols matrix of doubles. */
class Matrix
{
public:
    /** @brief Constructs an empty (0x0) matrix. */
    Matrix() : rows_(0), cols_(0) {}
    /** @brief Constructs a @p rows x @p cols matrix, zero-initialized. */
    Matrix(size_t rows, size_t cols);
    /** @brief Constructs a matrix from a nested vector literal, e.g.
     *  `{{1,2},{3,4}}`. All rows must be the same length. */
    explicit Matrix(const std::vector<std::vector<double>> &data);

    size_t rows() const { return rows_; } /**< Row count. */
    size_t cols() const { return cols_; } /**< Column count. */

    double &at(size_t r, size_t c) { return data_[r][c]; }      /**< Mutable element access. */
    double at(size_t r, size_t c) const { return data_[r][c]; } /**< Read-only element access. */

    /** @brief Element-wise addition. Throws std::runtime_error on a
     *  dimension mismatch. */
    Matrix operator+(const Matrix &o) const;
    /** @brief Element-wise subtraction. Throws std::runtime_error on a
     *  dimension mismatch. */
    Matrix operator-(const Matrix &o) const;
    /** @brief Matrix product. Throws std::runtime_error if the inner
     *  dimensions don't match. */
    Matrix operator*(const Matrix &o) const;
    /** @brief Scalar multiplication (every element times @p scalar). */
    Matrix operator*(double scalar) const;

    /** @brief Returns the transpose (rows and columns swapped). */
    Matrix transpose() const;

    /** @brief Computes the determinant via Gaussian elimination.
     *  @return 0.0 for a singular matrix.
     *  @throws std::runtime_error if the matrix isn't square. */
    double determinant() const;

    /** @brief Computes the matrix inverse via Gauss-Jordan elimination.
     *  @throws std::runtime_error if the matrix isn't square, or if
     *  it's singular (no inverse exists). */
    Matrix inverse() const;

    /** @brief Formats as "[[1,2],[3,4]]". */
    std::string toString() const;

private:
    size_t rows_;
    size_t cols_;
    std::vector<std::vector<double>> data_;

    void requireSameShape(const Matrix &o, const char *op) const;
    void requireSquare(const char *op) const;
};

#endif
