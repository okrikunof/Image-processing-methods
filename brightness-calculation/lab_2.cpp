#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

// Вектор в 3D
class Vector3D {
public:
    double x, y, z;

    Vector3D(const double x_ = 0, const double y_ = 0, const double z_ = 0)
        : x(x_), y(y_), z(z_) {}

    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    double dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3D cross(const Vector3D& other) const {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    double norm() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3D normalized() const {
        double n = norm();
        return (n != 0.0) ? (*this) * (1.0 / n) : *this;
    }
};

// Цвет RGB
class Color {
public:
    double r, g, b;

    Color(double r_ = 0.0, double g_ = 0.0, double b_ = 0.0)
        : r(r_), g(g_), b(b_) {}

    Color operator*(double scalar) const {
        return Color(r * scalar, g * scalar, b * scalar);
    }

    Color operator+(const Color& other) const {
        return Color(r + other.r, g + other.g, b + other.b);
    }

    Color operator*(const Color& other) const {
        return Color(r * other.r, g * other.g, b * other.b);
    }

    Color& operator+=(const Color& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }
};

// Источник света
class Light {
public:
    Vector3D position;
    Vector3D direction;
    Color intensity;
};

// Материал
class Material {
public:
    Color color;
    double diffuse;
    double specular;
    double exponent;
};

// Проверка, по одну ли сторону от нормали находятся точки
bool isSameSide(const Vector3D& point, const Vector3D& planePoint,
                const Vector3D& normal, const Vector3D& reference) {
    Vector3D vecToPoint = point - planePoint;
    Vector3D vecToRef = reference - planePoint;
    double dotPoint = vecToPoint.dot(normal);
    double dotRef = vecToRef.dot(normal);
    return (dotPoint * dotRef) > 0;
}

// Расчёт освещённости от одного источника
Color calculateIllumination(const Light& light,
                            const Vector3D& P0, const Vector3D& P1, const Vector3D& P2,
                            const double x, const double y, const Vector3D& viewDir) {

    Vector3D edge1 = (P1 - P0).normalized();
    Vector3D edge2 = (P2 - P0).normalized();
    Vector3D PT = P0 + edge1 * x + edge2 * y;

    Vector3D N = (P2 - P0).cross(P1 - P0).normalized();

    if (!isSameSide(light.position, PT, N, viewDir)) {
        return Color();
    }

    const Vector3D s_vec = PT - light.position;
    double R2 = s_vec.norm() * s_vec.norm();

    Vector3D s_normalized = s_vec.normalized();
    Vector3D lightDirNormalized = light.direction.normalized();

    double cos_alpha = std::max(0.0, s_normalized.dot(N));
    double cos_theta = std::max(0.0, s_normalized.dot(lightDirNormalized));

    return light.intensity * (cos_theta * cos_alpha / R2);
}

// Итоговая яркость в точке от всех источников
Color calculateBrightness(const std::vector<Light>& lights,
                          const Vector3D& P0, const Vector3D& P1, const Vector3D& P2,
                          double x, double y, const Vector3D& viewDir,
                          const Material& material) {
    Vector3D edge1 = (P1 - P0).normalized();
    Vector3D edge2 = (P2 - P0).normalized();
    Vector3D PT = P0 + edge1 * x + edge2 * y;

    Vector3D N = (P2 - P0).cross(P1 - P0).normalized();
    if (viewDir.dot(N) < 0) {
        N = N * -1.0;
    }

    Color totalBrightness;
    for (const auto& light : lights) {
        Color E = calculateIllumination(light, P0, P1, P2, x, y, viewDir);
        std::cout << E.r << ", " << E.g << ", " << E.b << std::endl;
        Vector3D s = (light.position - PT).normalized();
        Vector3D h = (viewDir + s).normalized();
        std::cout << h.dot(s) << std::endl;

        double diffuse = material.diffuse;
        double specular = material.specular * std::pow(std::max(0.0, h.dot(N)), material.exponent);

        Color contribution = E * material.color * (diffuse + specular) * (1.0 / M_PI);
        totalBrightness += contribution;
    }

    return totalBrightness;
}

// Главная функция
int main() {
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Ошибка: не удалось открыть 'input.txt'.\n";
        return 1;
    }

    int lightCount = 0;
    if (!(inputFile >> lightCount) || lightCount <= 0) {
        std::cerr << "Ошибка: некорректное количество источников света.\n";
        return 1;
    }

    std::vector<Light> lights;
    for (int i = 0; i < lightCount; ++i) {
        double lx, ly, lz, ldx, ldy, ldz, lr, lg, lb;
        if (!(inputFile >> lx >> ly >> lz >> ldx >> ldy >> ldz >> lr >> lg >> lb)) {
            std::cerr << "Ошибка: неверные данные для источника света №" << i + 1 << ".\n";
            return 1;
        }

        lights.push_back({
            Vector3D(lx, ly, lz),
            Vector3D(ldx, ldy, ldz),
            Color(lr, lg, lb)
        });
    }

    // Вершины треугольника
    Vector3D P0, P1, P2;
    if (!(inputFile >> P0.x >> P0.y >> P0.z
                   >> P1.x >> P1.y >> P1.z
                   >> P2.x >> P2.y >> P2.z)) {
        std::cerr << "Ошибка: неверные координаты треугольника.\n";
        return 1;
    }

    // Параметры материала
    Color color;
    double kd, ks, ke;
    if (!(inputFile >> color.r >> color.g >> color.b >> kd >> ks >> ke)) {
        std::cerr << "Ошибка: неверные параметры материала.\n";
        return 1;
    }

    Material material = {color, kd, ks, ke};

    // Точка внутри треугольника и направление взгляда
    double x, y;
    Vector3D viewDir;
    if (!(inputFile >> x >> y >> viewDir.x >> viewDir.y >> viewDir.z)) {
        std::cerr << "Ошибка: неверные данные для точки или взгляда.\n";
        return 1;
    }

    Color brightness = calculateBrightness(lights, P0, P1, P2, x, y, viewDir, material);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Яркость точки: ("
              << brightness.r << ", "
              << brightness.g << ", "
              << brightness.b << ")\n";

    return 0;
}
