#include "color.h"

Color::Color(const double r_, const double g_, const double b_) noexcept
    : r(r_), g(g_), b(b_) {}

Color Color::operator*(double scalar) const noexcept {
    return {r * scalar, g * scalar, b * scalar};
}

Color Color::operator+(const Color& other) const noexcept {
    return {r + other.r, g + other.g, b + other.b};
}

Color Color::operator*(const Color& other) const noexcept {
    return {r * other.r, g * other.g, b * other.b};
}

Color& Color::operator+=(const Color& other) noexcept {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}