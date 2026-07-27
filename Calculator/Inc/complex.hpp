#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <string>

/*
 * A standard complex number type. This is where C++ genuinely earns
 * its keep over the rest of this project's C code: operator
 * overloading lets arithmetic read the way the math reads
 * ((a + bi) * (c + di) instead of complexMultiply(a, b)).
 */
class Complex
{
public:
    double re;
    double im;

    Complex(double re_ = 0.0, double im_ = 0.0) : re(re_), im(im_) {}

    Complex operator+(const Complex &o) const;
    Complex operator-(const Complex &o) const;
    Complex operator*(const Complex &o) const;
    Complex operator/(const Complex &o) const;
    Complex operator-() const;

    /* Principal square root — handles negative reals (sqrt(-1) = i)
       and the fully general complex case (sqrt of a complex number). */
    static Complex sqrtOf(const Complex &c);

    /* Formats as "3+4i", "3-4i", or plain "3" if purely real. */
    std::string toString() const;
};

#endif