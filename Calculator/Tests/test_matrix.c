/* evaluateMatrixExpression() (matrix_eval.h) is an extern "C" entry
   point specifically so plain C code -- including this test file --
   can call into the C++ matrix engine (Matrix.cpp, matrix_eval.cpp)
   without itself being compiled as C++. */
#include "test_framework.h"
#include "tests.h"
#include "matrix_eval.h"

static void test_matrix_arithmetic(void)
{
    char result[256];

    ASSERT_TRUE(evaluateMatrixExpression("[[1,2],[3,4]] + [[5,6],[7,8]]", result, sizeof(result)) == 1,
                "matrix addition should succeed");
    ASSERT_STR_EQ(result, "[[6,8],[10,12]]", "should add element-wise");

    ASSERT_TRUE(evaluateMatrixExpression("[[5,6],[7,8]] - [[1,2],[3,4]]", result, sizeof(result)) == 1,
                "matrix subtraction should succeed");
    ASSERT_STR_EQ(result, "[[4,4],[4,4]]", "should subtract element-wise");

    ASSERT_TRUE(evaluateMatrixExpression("2*[[1,2],[3,4]]", result, sizeof(result)) == 1,
                "scalar*matrix should succeed");
    ASSERT_STR_EQ(result, "[[2,4],[6,8]]", "should scale every element");

    ASSERT_TRUE(evaluateMatrixExpression("[[1,2],[3,4]]*2", result, sizeof(result)) == 1,
                "matrix*scalar should succeed (order shouldn't matter)");
    ASSERT_STR_EQ(result, "[[2,4],[6,8]]", "should scale every element regardless of operand order");

    ASSERT_TRUE(evaluateMatrixExpression("[[1,2],[3,4]]*[[5,6],[7,8]]", result, sizeof(result)) == 1,
                "matrix*matrix should succeed");
    ASSERT_STR_EQ(result, "[[19,22],[43,50]]", "should compute the matrix product");
}

static void test_matrix_operations(void)
{
    char result[256];

    ASSERT_TRUE(evaluateMatrixExpression("det([[1,2],[3,4]])", result, sizeof(result)) == 1,
                "det() should succeed on a square matrix");
    ASSERT_STR_EQ(result, "-2", "det([[1,2],[3,4]]) should be -2");

    ASSERT_TRUE(evaluateMatrixExpression("transpose([[1,2],[3,4]])", result, sizeof(result)) == 1,
                "transpose() should succeed");
    ASSERT_STR_EQ(result, "[[1,3],[2,4]]", "transpose should swap rows and columns");

    ASSERT_TRUE(evaluateMatrixExpression("inverse([[1,2],[3,4]])", result, sizeof(result)) == 1,
                "inverse() should succeed on a non-singular square matrix");
    ASSERT_STR_EQ(result, "[[-2,1],[1.5,-0.5]]", "inverse([[1,2],[3,4]]) should be [[-2,1],[1.5,-0.5]]");
}

static void test_matrix_error_paths(void)
{
    char result[256];

    ASSERT_TRUE(evaluateMatrixExpression("[[1,2],[3,4]] + [[1,2,3],[4,5,6]]", result, sizeof(result)) == 0,
                "adding mismatched dimensions should fail");
    ASSERT_STR_EQ(result, "Cannot add: matrix dimensions do not match", "should explain the dimension mismatch");

    ASSERT_TRUE(evaluateMatrixExpression("[[1,2],[3,4]] + 5", result, sizeof(result)) == 0,
                "adding a scalar and a matrix should fail");
    ASSERT_STR_EQ(result, "Cannot add a scalar and a matrix", "should explain the type mismatch");

    ASSERT_TRUE(evaluateMatrixExpression("[[1,2,3],[4,5,6]]*[[1,2],[3,4]]", result, sizeof(result)) == 0,
                "multiplying with incompatible inner dimensions should fail");
    ASSERT_STR_EQ(result, "Cannot multiply: left matrix columns must match right matrix rows",
                  "should explain the inner-dimension mismatch");

    ASSERT_TRUE(evaluateMatrixExpression("det([[1,2,3],[4,5,6]])", result, sizeof(result)) == 0,
                "det() of a non-square matrix should fail");
    ASSERT_STR_EQ(result, "determinant() requires a square matrix", "should explain the square requirement");

    ASSERT_TRUE(evaluateMatrixExpression("inverse([[1,2],[2,4]])", result, sizeof(result)) == 0,
                "inverse() of a singular matrix should fail");
    ASSERT_STR_EQ(result, "Matrix is singular; inverse does not exist", "should explain that it's singular");

    ASSERT_TRUE(evaluateMatrixExpression("det(5)", result, sizeof(result)) == 0,
                "det() of a scalar should fail");
    ASSERT_STR_EQ(result, "det() requires a matrix argument", "should explain that det() needs a matrix");

    ASSERT_TRUE(evaluateMatrixExpression("[[1,2],[3]]", result, sizeof(result)) == 0,
                "inconsistent row lengths should fail");
    ASSERT_STR_EQ(result, "Matrix rows must all have the same length", "should explain the inconsistent row length");
}

void run_matrix_tests(void)
{
    test_matrix_arithmetic();
    test_matrix_operations();
    test_matrix_error_paths();
}
