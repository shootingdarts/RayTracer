#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Transform.h"

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

// Structure to store intersection information
struct Intersection {
    bool hit;
    float distance;
    vec3 point;
    vec3 normal;

    Intersection() : hit(false), distance(std::numeric_limits<float>::max()), point(vec3(0.0)),  normal(vec3(0.0)) {}
};

// Ray structure
struct Ray {
    vec3 origin;
    vec3 direction;

    Ray(const vec3& origin, const vec3& direction) : origin(origin), direction(direction) {}
};

// Camera structure
struct Camera {
    vec3 position;
    vec3 u;
    vec3 v;
    vec3 w;
    float fovy;
};

// abstract class
class SceneObject {
public:
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    vec3 emission;
    virtual Intersection intersect(const Ray& ray) = 0;
};

class Triangle : public SceneObject {
    vec3 v0, v1, v2;
public:
    Triangle(const vec3& v0, const vec3& v1, const vec3& v2) : v0(v0), v1(v1), v2(v2) {};
    Intersection intersect(const Ray& ray) override;
};

class Sphere : public SceneObject {
    vec3 center;
    float radius;
public:
    Sphere(const vec3& center, float radius) : center(center), radius(radius) {};
    Intersection intersect(const Ray& ray) override;
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

enum light {directional, point};

struct Light {
    light type;
    vec3 direction;
    vec3 location;
    vec3 attenuation;
    vec3 color;
};

struct InterObject {
    Intersection intsect;
    SceneObject* obj_light;
    InterObject(Intersection intx, SceneObject* obj) : intsect(intx), obj_light(obj) {}
};

Intersection findClosestIntersection(const Ray& ray);
vec3 findColor(const Intersection& hit);

EXTERN int w, h;
EXTERN int depth;
EXTERN int maxvertex;
EXTERN float kEpsilon;

EXTERN std::string outputFile;
EXTERN Camera cam;
EXTERN std::vector<SceneObject*> currScene;

EXTERN vec3 currAmbient;
EXTERN vec3 currDiffuse;
EXTERN vec3 currSpecular;
EXTERN float currShininess;
EXTERN vec3 currEmission;
EXTERN vec3 currAttenuation;

EXTERN std::vector<Light> lights;
EXTERN std::vector<vec3> vertices;