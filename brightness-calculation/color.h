#ifndef COLOR_H
#define COLOR_H

class Color {
public:
    double r, g, b;

    Color(double r_ = 0.0, double g_ = 0.0, double b_ = 0.0) noexcept;

    Color operator*(double scalar) const noexcept;
    Color operator+(const Color& other) const noexcept;
    Color operator*(const Color& other) const noexcept;
    
    Color& operator+=(const Color& other) noexcept;
};

#endif // COLOR_H