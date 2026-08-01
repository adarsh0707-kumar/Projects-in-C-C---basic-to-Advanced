/**
 * @file Complex.hpp
 * @brief A standard complex number type with operator overloading.
 *
 * This is where C++ genuinely earns its keep over the rest of this
 * project's C code: operator overloading lets arithmetic read the
 * way the math reads ((a + bi) * (c + di) instead of
 * complexMultiply(a, b, c, d)).
 */
#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <string>

/** A complex number `re + im*i`, with the usual arithmetic operators. */
class Complex
{
public:
    double re; /**< Real part. */
    double im; /**< Imaginary part. */

    /** @brief Constructs a complex number; defaults to 0 + 0i. */
    Complex(double re_ = 0.0, double im_ = 0.0) : re(re_), im(im_) {}

    Complex operator+(const Complex &o) const;
    Complex operator-(const Complex &o) const;
    Complex operator*(const Complex &o) const;
    /** @note Division by (0+0i) is not caught here and will silently
     *  produce inf/nan components, since this class throws no
     *  exceptions of its own -- see complex_eval.cpp's parser for
     *  where user-facing errors are actually surfaced. */
    Complex operator/(const Complex &o) const;
    Complex operator-() const;

    /** @brief Principal square root -- handles negative reals
     *  (sqrt(-1) == i) and the fully general complex case. */
    static Complex sqrtOf(const Complex &c);

    /** @brief Formats as "3+4i", "3-4i", or plain "3" if purely real. */
    std::string toString() const;
};

#endif
