/**
 * @file main.cpp
 * @brief 3D Scene Ray Tracing with Intel Embree
 * 
 * This program renders a 3D scene using Intel Embree's high-performance ray tracing
 * library. It supports:
 * - Multiple geometry types (triangles, cubes)
 * - Point and directional light sources
 * - Phong shading with diffuse and specular components
 * - Recursive ray tracing for reflections
 * - Shadow calculation
 * 
 * Output: PPM image file (output.ppm)
 */

#include <embree4/rtcore.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

/**
 * @brief 3D Vector class for geometric operations
 * Provides basic vector arithmetic and operations for 3D graphics
 */
class Vector3D {
public:
    double x, y, z;

    Vector3D(double x_ = 0, double y_ = 0, double z_ = 0) : x(x_), y(y_), z(z_) {
    }

    Vector3D operator+(const Vector3D &other) const { return Vector3D(x + other.x, y + other.y, z + other.z); }
    Vector3D operator-(const Vector3D &other) const { return Vector3D(x - other.x, y - other.y, z - other.z); }
    Vector3D operator*(double scalar) const { return Vector3D(x * scalar, y * scalar, z * scalar); }
    Vector3D operator-() const { return Vector3D(-x, -y, -z); }
    double dot(const Vector3D &other) const { return x * other.x + y * other.y + z * other.z; }

    Vector3D cross(const Vector3D &other) const {
        return Vector3D(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    double norm() const { return std::sqrt(x * x + y * y + z * z); }

    Vector3D normalized() const {
        double n = norm();
        return n > 0 ? *this * (1.0 / n) : *this;
    }
};

/**
 * @brief RGB Color class for color representation
 * Supports color arithmetic operations for lighting calculations
 */
class Color {
public:
    double r, g, b;

    Color(double r_ = 0, double g_ = 0, double b_ = 0) : r(r_), g(g_), b(b_) {
    }

    Color operator*(double scalar) const { return Color(r * scalar, g * scalar, b * scalar); }
    Color operator+(const Color &other) const { return Color(r + other.r, g + other.g, b + other.b); }
    Color operator*(const Color &other) const { return Color(r * other.r, g * other.g, b * other.b); }
};

/**
 * @brief Material structure for surface properties
 * Defines how a surface interacts with light (Phong reflection model)
 */
struct Material {
    Color color;             ///< Base diffuse color
    double diffuse;          ///< Diffuse reflection coefficient
    double specular;         ///< Specular reflection coefficient
    double exponent;         ///< Specular exponent (shininess)
    Color specular_color;    ///< Specular highlight color
    double reflectivity;     ///< Reflection coefficient (0.0 = no reflection, 1.0 = perfect mirror)
};

/**
 * @brief Abstract base class for light sources
 * Defines the interface for different types of light sources
 */
class Light {
public:
    Color intensity;

    virtual bool isOccluded(const Vector3D &point, RTCScene scene) const = 0;

    virtual Vector3D getDirection(const Vector3D &point) const = 0;

    virtual double getAttenuation(const Vector3D &point) const = 0;

    virtual ~Light() {
    }
};

/**
 * @brief Point light source class
 * Emits light uniformly in all directions from a single point
 */
class PointLight : public Light {
public:
    Vector3D position;

    PointLight(Vector3D pos, Color intens) {
        position = pos;
        intensity = intens;
    }

    bool isOccluded(const Vector3D &point, RTCScene scene) const override {
        RTCRay ray;
        ray.org_x = point.x;
        ray.org_y = point.y;
        ray.org_z = point.z;
        Vector3D dir = (position - point).normalized();
        ray.dir_x = dir.x;
        ray.dir_y = dir.y;
        ray.dir_z = dir.z;
        ray.tnear = 0.001f;
        ray.tfar = (position - point).norm() - 0.001f;
        ray.flags = 0;
        rtcOccluded1(scene, &ray);
        return ray.tfar < 0;
    }

    Vector3D getDirection(const Vector3D &point) const override {
        return (position - point).normalized();
    }

    double getAttenuation(const Vector3D &point) const override {
        return 1.0;
    }
};

/**
 * @brief Directional light source class
 * Emits parallel light rays in a specific direction (like sunlight)
 */
class DirectionalLight : public Light {
public:
    Vector3D direction;

    DirectionalLight(Vector3D dir, Color intens) {
        direction = dir.normalized();
        intensity = intens;
    }

    bool isOccluded(const Vector3D &point, RTCScene scene) const override {
        RTCRay ray;
        ray.org_x = point.x;
        ray.org_y = point.y;
        ray.org_z = point.z;
        Vector3D dir = -direction.normalized();
        ray.dir_x = dir.x;
        ray.dir_y = dir.y;
        ray.dir_z = dir.z;
        ray.tnear = 0.001f;
        ray.tfar = std::numeric_limits<float>::infinity();
        ray.flags = 0;
        rtcOccluded1(scene, &ray);
        return ray.tfar < 0;
    }

    Vector3D getDirection(const Vector3D &point) const override {
        return -direction.normalized();
    }

    double getAttenuation(const Vector3D &point) const override {
        return 1.0;
    }
};

/**
 * @brief Compute the ray direction through a pixel
 * 
 * Calculates the direction of a ray from the camera eye through a specific
 * pixel (i, j) on the virtual screen.
 * 
 * @param i Pixel column index
 * @param j Pixel row index
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param eye Camera position
 * @param center Point the camera is looking at
 * @param up Up direction vector
 * @param distance Distance from camera to the screen
 * @param screen_width Width of the virtual screen
 * @param screen_height Height of the virtual screen
 * @return Normalized ray direction vector
 */
Vector3D computeRayDirection(int i, int j, int width, int height, const Vector3D &eye, const Vector3D center,
                             const Vector3D &up, double distance, double screen_width, double screen_height) {
    Vector3D view = (center - eye).normalized();
    Vector3D right = view.cross(up).normalized();
    Vector3D actual_up = right.cross(view).normalized();
    Vector3D screen_center = eye + view * distance;
    double u = (i + 0.5) / width * screen_width - screen_width / 2;
    double v = -(j + 0.5) / height * screen_height + screen_height / 2;
    Vector3D screen_point = screen_center + right * u + actual_up * v;
    Vector3D rayDir = (screen_point - eye).normalized();
    if (i == 0 && j == 0) {
        std::cout << "Луч для пикселя (0,0): направление (" << rayDir.x << ", " << rayDir.y << ", " << rayDir.z << ")"
                << std::endl;
    }
    return rayDir;
}

/**
 * @brief Shading function with recursive ray tracing
 * 
 * Computes the color at a ray-surface intersection point using the Phong
 * reflection model and supports recursive ray tracing for reflections.
 * 
 * @param rayhit Ray-surface intersection information
 * @param scene Embree scene containing all geometry
 * @param lights Vector of light sources
 * @param viewDir View direction (ray direction)
 * @param depth Current recursion depth for reflections
 * @return Final color at the intersection point
 */
Color shade(const RTCRayHit &rayhit, RTCScene scene, const std::vector<Light *> &lights, const Vector3D &viewDir,
            int depth = 0) {
    const int MAX_DEPTH = 50; // Maximum recursion depth for reflections

    // Stop recursion at maximum depth to prevent infinite loops
    if (depth >= MAX_DEPTH) {
        return Color(0, 0, 0);
    }

    RTCGeometry geometry = rtcGetGeometry(scene, rayhit.hit.geomID);
    Material *material = static_cast<Material *>(rtcGetGeometryUserData(geometry));
    Vector3D point(rayhit.ray.org_x + rayhit.ray.tfar * rayhit.ray.dir_x,
                   rayhit.ray.org_y + rayhit.ray.tfar * rayhit.ray.dir_y,
                   rayhit.ray.org_z + rayhit.ray.tfar * rayhit.ray.dir_z);
    Vector3D normal(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z);
    normal = normal.normalized();

    Color totalColor(0, 0, 0);
    for (const auto *light: lights) {
        if (!light->isOccluded(point, scene)) {
            Vector3D L = light->getDirection(point);
            Vector3D H = (viewDir + L).normalized();
            double NdotL = std::max(0.0, normal.dot(L));
            double HdotN = std::max(0.0, H.dot(normal));
            Color diffuse = material->color * material->diffuse * NdotL;
            Color specular = material->specular_color * material->specular * std::pow(HdotN, material->exponent);
            Color contribution = (diffuse + specular) * light->intensity * light->getAttenuation(point);
            totalColor = totalColor + contribution;
        }
    }

    if (material->reflectivity > 0.0) {
        Vector3D incident = -viewDir;
        double NdotI = normal.dot(incident);
        Vector3D reflectedDir = incident - normal * (2.0 * NdotI);
        reflectedDir = reflectedDir.normalized();

        RTCRayHit reflectedRay;
        reflectedRay.ray.org_x = point.x;
        reflectedRay.ray.org_y = point.y;
        reflectedRay.ray.org_z = point.z;
        reflectedRay.ray.dir_x = reflectedDir.x;
        reflectedRay.ray.dir_y = reflectedDir.y;
        reflectedRay.ray.dir_z = reflectedDir.z;
        reflectedRay.ray.tnear = 0.001f;
        reflectedRay.ray.tfar = std::numeric_limits<float>::infinity();
        reflectedRay.ray.flags = 0;
        reflectedRay.hit.geomID = RTC_INVALID_GEOMETRY_ID;

        rtcIntersect1(scene, &reflectedRay);

        if (reflectedRay.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
            Color reflectedColor = shade(reflectedRay, scene, lights, reflectedDir, depth + 1);
            totalColor = totalColor + reflectedColor * material->reflectivity;
        }
    }

    return totalColor;
}

/**
 * @brief Embree error handler
 * Called when Embree encounters an error during ray tracing operations
 */
void errorFunction(void *userPtr, RTCError error, const char *str) {
    std::cerr << "Embree Error " << error << ": " << str << std::endl;
}

int main() {
    RTCDevice device = rtcNewDevice(nullptr);
    if (!device) {
        std::cerr << "Не удалось создать устройство Embree" << std::endl;
        return 1;
    }
    rtcSetDeviceErrorFunction(device, errorFunction, nullptr);
    RTCScene scene = rtcNewScene(device);

    // Определяем материалы для объектов
    Material wallMaterial = {Color(1.0, 1.0, 1.0), 0.7, 0, 10.0, Color(1, 1, 1), 0.1};
    Material floorMaterial = {Color(1.0, 1.0, 0.0), 0.7, 0.3, 10.0, Color(1, 1, 1), 0.1};
    Material sphereMaterial1 = {Color(0.2, 0.2, 0.9), 0.7, 30, 100.0, Color(0, 1, 0), 0.1};
    Material sphereMaterial2 = {Color(0.7, 0.4, 0.5), 0.7, 30, 100.0, Color(1, 1, 1), 0.15};

    // Создаем пол
    RTCGeometry floor = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    float floorVertices[] = {
        -20, 0, -20,
        20, 0, -20,
        20, 0, 20,
        -20, 0, 20
    };
    unsigned floorIndices[] = {0, 2, 1, 0, 3, 2};
    rtcSetSharedGeometryBuffer(floor, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, floorVertices, 0, 3 * sizeof(float),
                               4);
    rtcSetSharedGeometryBuffer(floor, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, floorIndices, 0, 3 * sizeof(unsigned),
                               2);
    rtcCommitGeometry(floor);
    unsigned floorID = rtcAttachGeometry(scene, floor);
    rtcSetGeometryUserData(floor, &floorMaterial);


    RTCGeometry cube1 = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    float cube1Vertices[] = {
        -1.7, 0.3, -1.6,
        0.3, 0.3, -1.6,
        0.3, 2.3, -1.6,
        -1.7, 2.3, -1.6,
        -1.7, 0.3, 0.4,
        0.3, 0.3, 0.4,
        0.3, 2.3, 0.4,
        -1.7, 2.3, 0.4
    };
    unsigned cubeIndices[] = {
        0, 1, 2,  0, 2, 3,
        4, 5, 6,  4, 6, 7,
        0, 1, 5,  0, 5, 4,
        3, 2, 6,  3, 6, 7,
        0, 3, 7,  0, 7, 4,
        1, 2, 6,  1, 6, 5
    };
    rtcSetSharedGeometryBuffer(cube1, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, cube1Vertices, 0, 3 * sizeof(float), 8);
    rtcSetSharedGeometryBuffer(cube1, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, cubeIndices, 0, 3 * sizeof(unsigned), 12);
    rtcCommitGeometry(cube1);
    unsigned cube1ID = rtcAttachGeometry(scene, cube1);
    rtcSetGeometryUserData(cube1, &sphereMaterial1);


    RTCGeometry cube2 = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    float cube2Vertices[] = {
        1.0, -0.2, -1.5,
        4.0, -0.2, -1.5,
        4.0, 2.8, -1.5,
        1.0, 2.8, -1.5,
        1.0, -0.2, 1.5,
        4.0, -0.2, 1.5,
        4.0, 2.8, 1.5,
        1.0, 2.8, 1.5
    };

    rtcSetSharedGeometryBuffer(cube2, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, cube2Vertices, 0, 3 * sizeof(float), 8);
    rtcSetSharedGeometryBuffer(cube2, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, cubeIndices, 0, 3 * sizeof(unsigned), 12);
    rtcCommitGeometry(cube2);
    unsigned cube2ID = rtcAttachGeometry(scene, cube2);
    rtcSetGeometryUserData(cube2, &sphereMaterial2);

    // // Создаем первую сферу
    // RTCGeometry sphere1 = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_SPHERE_POINT);
    // float sphere1Data[] = {-0.7, 1.3, -0.6, 1};
    // rtcSetSharedGeometryBuffer(sphere1, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT4, sphere1Data, 0, 4 * sizeof(float),
    //                            1);
    // rtcCommitGeometry(sphere1);
    // unsigned sphere1ID = rtcAttachGeometry(scene, sphere1);
    // rtcSetGeometryUserData(sphere1, &sphereMaterial1);
    //
    // // Создаем вторую сферу
    // RTCGeometry sphere2 = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_SPHERE_POINT);
    // float sphere2Data[] = {2.5, 1.3, 0, 1.5};
    // rtcSetSharedGeometryBuffer(sphere2, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT4, sphere2Data, 0, 4 * sizeof(float),
    //                            1);
    // rtcCommitGeometry(sphere2);
    // unsigned sphere2ID = rtcAttachGeometry(scene, sphere2);
    // rtcSetGeometryUserData(sphere2, &sphereMaterial2);


    RTCGeometry wall = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    float wallVertices[] = {
        -20, -20, -10,
        20, -20, -10,
        20, 20, -10,
        -20, 20, -10
    };
    unsigned wallIndices[] = {0, 1, 2, 0, 2, 3};
    rtcSetSharedGeometryBuffer(wall, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, wallVertices, 0, 3 * sizeof(float),
                               4);
    rtcSetSharedGeometryBuffer(wall, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, wallIndices, 0, 3 * sizeof(unsigned),
                               2);
    rtcCommitGeometry(wall);
    unsigned wallID = rtcAttachGeometry(scene, wall);
    rtcSetGeometryUserData(wall, &wallMaterial);


    rtcCommitScene(scene);
    std::cout << "Сцена успешно создана" << std::endl;

    // Настройка камеры
    Vector3D eye(1, 2, 5);
    Vector3D center(1, 2, 0);
    Vector3D up(0, 1, 0);
    double distance = 8.0;
    double screen_width = 15.0;
    double screen_height = 15.0;
    int image_width = 800;
    int image_height = 800;

    // Создаем источники света
    std::vector<Light *> lights;
    PointLight plight(Vector3D(1, 3, 3), Color(200, 200, 200));
    DirectionalLight dlight(Vector3D(-1, -1, -1), Color(200.0, 200.0, 200.0));
    lights.push_back(&plight);
    lights.push_back(&dlight);

    // Буфер для хранения цветов изображения
    std::vector<Color> image(image_width * image_height);
    std::cout << "Начало рендеринга" << std::endl;
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            Vector3D rayDir = computeRayDirection(i, j, image_width, image_height, eye, center, up, distance,
                                                  screen_width, screen_height);
            RTCRayHit rayhit;
            rayhit.ray.org_x = eye.x;
            rayhit.ray.org_y = eye.y;
            rayhit.ray.org_z = eye.z;
            rayhit.ray.dir_x = rayDir.x;
            rayhit.ray.dir_y = rayDir.y;
            rayhit.ray.dir_z = rayDir.z;
            rayhit.ray.tnear = 0.001f;
            rayhit.ray.tfar = std::numeric_limits<float>::infinity();
            rayhit.ray.flags = 0;
            rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
            rtcIntersect1(scene, &rayhit);
            if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
                std::cout << "Пересечение в пикселе (" << i << "," << j << "), geomID: " << rayhit.hit.geomID
                        << ", tfar: " << rayhit.ray.tfar << ", x: " << rayhit.ray.org_x + rayhit.ray.tfar * rayhit.ray.
                        dir_x
                        << ", y: " << rayhit.ray.org_y + rayhit.ray.tfar * rayhit.ray.dir_y
                        << ", z: " << rayhit.ray.org_z + rayhit.ray.tfar * rayhit.ray.dir_z << std::endl;
                image[j * image_width + i] = shade(rayhit, scene, lights, rayDir, 0); // Начинаем с глубины 0
            } else {
                image[j * image_width + i] = Color(0, 0, 0);
            }
        }
    }

    // Сохраняем изображение в PPM-файл
    std::ofstream ppm("output.ppm");
    ppm << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (const auto &c: image) {
        double scale = 1.0;
        int r = std::min(255, std::max(0, static_cast<int>(c.r * scale)));
        int g = std::min(255, std::max(0, static_cast<int>(c.g * scale)));
        int b = std::min(255, std::max(0, static_cast<int>(c.b * scale)));
        ppm << r << " " << g << " " << b << "\n";
    }
    ppm.close();

    // Очистка ресурсов
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);

    std::cout << "Изображение сохранено в output.ppm" << std::endl;
    return 0;
}
