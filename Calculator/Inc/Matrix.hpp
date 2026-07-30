#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <string>

/*
 * Storage is std::vector<std::vector<double>> — RAII means this
 * class never needs an explicit destructor, copy constructor, or
 * assignment operator; the compiler-generated ones are correct
 * because vector already manages its own memory. This is the
 * concrete advantage C++ has here over a hand-rolled C
 * malloc/free 2D array: there's no exit path that can leak.
 */
class Matrix
{
public:
    Matrix() : rows_(0), cols_(0) {}
    Matrix(size_t rows, size_t cols);
    explicit Matrix(const std::vector<std::vector<double>> &data);

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }

    double &at(size_t r, size_t c) { return data_[r][c]; }
    double at(size_t r, size_t c) const { return data_[r][c]; }

    Matrix operator+(const Matrix &o) const;
    Matrix operator-(const Matrix &o) const;
    Matrix operator*(const Matrix &o) const; /* matrix product */
    Matrix operator*(double scalar) const;   /* scale */

    Matrix transpose() const;

    /* Both throw std::runtime_error for non-square input.
       determinant() returns 0.0 for a singular matrix;
       inverse() throws for a singular matrix (no result to return). */
    double determinant() const;
    Matrix inverse() const;

    std::string toString() const;

private:
    size_t rows_;
    size_t cols_;
    std::vector<std::vector<double>> data_;

    void requireSameShape(const Matrix &o, const char *op) const;
    void requireSquare(const char *op) const;
};

#endif