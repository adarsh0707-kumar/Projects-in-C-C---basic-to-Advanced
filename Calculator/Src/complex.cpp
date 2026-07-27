#include "complex.hpp"
#include <cmath>
#include <sstream>

Complex Complex::operator+(const Complex &o) const
{
    return Complex(re + o.re, im + o.im);
}

Complex Complex::operator-(const Complex &o) const
{
    return Complex(re - o.re, im - o.im);
}

Complex Complex::operator*(const Complex &o) const
{
    return Complex(re * o.re - im * o.im,
                   re * o.im + im * o.re);
}

Complex Complex::operator/(const Complex &o) const
{
    double denom = o.re * o.re + o.im * o.im;
    return Complex((re * o.re + im * o.im) / denom,
                   (im * o.re - re * o.im) / denom);
}

Complex Complex::operator-() const
{
    return Complex(-re, -im);
}

Complex Complex::sqrtOf(const Complex &c)
{
    /* Standard principal-square-root formula for a complex number. */
    double r = std::sqrt(c.re * c.re + c.im * c.im);
    double re = std::sqrt((r + c.re) / 2.0);
    double im = std::sqrt((r - c.re) / 2.0);

    if (c.im < 0.0)
        im = -im;

    return Complex(re, im);
}

std::string Complex::toString() const
{
    std::ostringstream oss;

    if (im == 0.0)
    {
        oss << re;
        return oss.str();
    }

    if (re == 0.0)
    {
        oss << im << "i";
        return oss.str();
    }

    oss << re;
    if (im >= 0.0)
        oss << "+";
    oss << im << "i";

    return oss.str();
}