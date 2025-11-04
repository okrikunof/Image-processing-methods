#ifndef COLOR_H
#define COLOR_H

/**
 * @brief RGB color representation class
 * 
 * Represents colors in RGB color space with double precision
 * for accurate lighting calculations.
 */
class Color {
public:
    double r, g, b; ///< Red, green, and blue color components (typically 0.0-1.0 or intensity values)

    /**
     * @brief Construct an RGB color
     * @param r_ Red component (default: 0.0)
     * @param g_ Green component (default: 0.0)
     * @param b_ Blue component (default: 0.0)
     */
    Color(double r_ = 0.0, double g_ = 0.0, double b_ = 0.0) noexcept;

    /**
     * @brief Multiply color by a scalar (brightness adjustment)
     * @param scalar Multiplication factor
     * @return Scaled color
     */
    Color operator*(double scalar) const noexcept;

    /**
     * @brief Add two colors (color blending)
     * @param other Color to add
     * @return Sum of colors
     */
    Color operator+(const Color& other) const noexcept;

    /**
     * @brief Multiply two colors component-wise (color modulation)
     * @param other Color to multiply with
     * @return Component-wise product
     */
    Color operator*(const Color& other) const noexcept;
    
    /**
     * @brief Add and assign another color to this color
     * @param other Color to add
     * @return Reference to this color after addition
     */
    Color& operator+=(const Color& other) noexcept;
};

#endif // COLOR_H