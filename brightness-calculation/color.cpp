#include "color.h"

// Constructor implementation
Color::Color(const double r_, const double g_, const double b_) noexcept
    : r(r_), g(g_), b(b_) {}

// Scalar multiplication: scales color intensity by a scalar value
Color Color::operator*(double scalar) const noexcept {
    return {r * scalar, g * scalar, b * scalar};
}

// Color addition: adds two colors component-wise
Color Color::operator+(const Color& other) const noexcept {
    return {r + other.r, g + other.g, b + other.b};
}

// Color multiplication: multiplies colors component-wise (modulation)
Color Color::operator*(const Color& other) const noexcept {
    return {r * other.r, g * other.g, b * other.b};
}

// In-place color addition: adds another color to this color
Color& Color::operator+=(const Color& other) noexcept {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}