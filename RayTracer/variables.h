#include <string>
#include <vector>
#include <glm/glm.hpp>

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN glm::vec3 eyeinit;
EXTERN glm::vec3 center;
EXTERN glm::vec3 upinit;
EXTERN float fovy;
EXTERN int w, h;
EXTERN int depth;
EXTERN string outputFile;

// Ray structure
struct Ray {
    vec3 origin;
    vec3 direction;

    Ray(const vec3& origin, const vec3& direction) : origin(origin), direction(direction) {}
};

// Structure to store intersection information
struct Intersection {
    bool hit;
    float distance;
    vec3 point;
    vec3 normal;

    Intersection() : hit(false), distance(std::numeric_limits<float>::max()) {}
};

// Camera structure
struct Camera {
    vec3 position;
    vec3 direction;
    float fov;
};

// Image structure
struct Image {
    int width, height;
    std::vector<vec3> pixels;

    Image(int width, int height) : width(width), height(height), pixels(width* height) {}

    void setPixel(int x, int y, const vec3& color) {
        pixels[y * width + x] = color;
    }
};

