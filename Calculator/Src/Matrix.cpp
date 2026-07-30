#include "Matrix.hpp"
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <algorithm>

Matrix::Matrix(size_t rows, size_t cols)
    : rows_(rows), cols_(cols), data_(rows, std::vector<double>(cols, 0.0))
{
}

Matrix::Matrix(const std::vector<std::vector<double>> &data)
    : rows_(data.size()), cols_(data.empty() ? 0 : data[0].size()), data_(data)
{
    for (const auto &row : data_)
    {
        if (row.size() != cols_)
            throw std::runtime_error("Matrix rows must all have the same length");
    }
}

void Matrix::requireSameShape(const Matrix &o, const char *op) const
{
    if (rows_ != o.rows_ || cols_ != o.cols_)
    {
        throw std::runtime_error(
            std::string("Cannot ") + op + ": matrix dimensions do not match");
    }
}

void Matrix::requireSquare(const char *op) const
{
    if (rows_ != cols_)
        throw std::runtime_error(std::string(op) + " requires a square matrix");
}

Matrix Matrix::operator+(const Matrix &o) const
{
    requireSameShape(o, "add");

    Matrix result(rows_, cols_);
    for (size_t i = 0; i < rows_; i++)
        for (size_t j = 0; j < cols_; j++)
            result.data_[i][j] = data_[i][j] + o.data_[i][j];

    return result;
}

Matrix Matrix::operator-(const Matrix &o) const
{
    requireSameShape(o, "subtract");

    Matrix result(rows_, cols_);
    for (size_t i = 0; i < rows_; i++)
        for (size_t j = 0; j < cols_; j++)
            result.data_[i][j] = data_[i][j] - o.data_[i][j];

    return result;
}

Matrix Matrix::operator*(const Matrix &o) const
{
    if (cols_ != o.rows_)
    {
        throw std::runtime_error(
            "Cannot multiply: left matrix columns must match right matrix rows");
    }

    Matrix result(rows_, o.cols_);
    for (size_t i = 0; i < rows_; i++)
    {
        for (size_t j = 0; j < o.cols_; j++)
        {
            double sum = 0.0;
            for (size_t k = 0; k < cols_; k++)
                sum += data_[i][k] * o.data_[k][j];
            result.data_[i][j] = sum;
        }
    }

    return result;
}

Matrix Matrix::operator*(double scalar) const
{
    Matrix result(rows_, cols_);
    for (size_t i = 0; i < rows_; i++)
        for (size_t j = 0; j < cols_; j++)
            result.data_[i][j] = data_[i][j] * scalar;

    return result;
}

Matrix Matrix::transpose() const
{
    Matrix result(cols_, rows_);
    for (size_t i = 0; i < rows_; i++)
        for (size_t j = 0; j < cols_; j++)
            result.data_[j][i] = data_[i][j];

    return result;
}

double Matrix::determinant() const
{
    requireSquare("determinant()");

    size_t n = rows_;
    std::vector<std::vector<double>> a = data_;
    double det = 1.0;

    for (size_t col = 0; col < n; col++)
    {
        size_t pivotRow = col;
        double maxVal = std::fabs(a[col][col]);

        for (size_t r = col + 1; r < n; r++)
        {
            if (std::fabs(a[r][col]) > maxVal)
            {
                maxVal = std::fabs(a[r][col]);
                pivotRow = r;
            }
        }

        if (maxVal < 1e-12)
            return 0.0; /* singular */

        if (pivotRow != col)
        {
            std::swap(a[pivotRow], a[col]);
            det = -det;
        }

        det *= a[col][col];

        for (size_t r = col + 1; r < n; r++)
        {
            double factor = a[r][col] / a[col][col];
            for (size_t c = col; c < n; c++)
                a[r][c] -= factor * a[col][c];
        }
    }

    return det;
}

Matrix Matrix::inverse() const
{
    requireSquare("inverse()");

    size_t n = rows_;
    std::vector<std::vector<double>> a = data_;
    std::vector<std::vector<double>> inv(n, std::vector<double>(n, 0.0));

    for (size_t i = 0; i < n; i++)
        inv[i][i] = 1.0;

    for (size_t col = 0; col < n; col++)
    {
        size_t pivotRow = col;
        double maxVal = std::fabs(a[col][col]);

        for (size_t r = col + 1; r < n; r++)
        {
            if (std::fabs(a[r][col]) > maxVal)
            {
                maxVal = std::fabs(a[r][col]);
                pivotRow = r;
            }
        }

        if (maxVal < 1e-12)
            throw std::runtime_error("Matrix is singular; inverse does not exist");

        if (pivotRow != col)
        {
            std::swap(a[pivotRow], a[col]);
            std::swap(inv[pivotRow], inv[col]);
        }

        double pivot = a[col][col];
        for (size_t c = 0; c < n; c++)
        {
            a[col][c] /= pivot;
            inv[col][c] /= pivot;
        }

        for (size_t r = 0; r < n; r++)
        {
            if (r == col)
                continue;

            double factor = a[r][col];
            for (size_t c = 0; c < n; c++)
            {
                a[r][c] -= factor * a[col][c];
                inv[r][c] -= factor * inv[col][c];
            }
        }
    }

    return Matrix(inv);
}

std::string Matrix::toString() const
{
    std::ostringstream oss;
    oss << "[";

    for (size_t i = 0; i < rows_; i++)
    {
        oss << "[";
        for (size_t j = 0; j < cols_; j++)
        {
            oss << data_[i][j];
            if (j + 1 < cols_)
                oss << ",";
        }
        oss << "]";
        if (i + 1 < rows_)
            oss << ",";
    }

    oss << "]";
    return oss.str();
}