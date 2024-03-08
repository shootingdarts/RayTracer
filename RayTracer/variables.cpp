#include "variables.h"

// Abstract class for scene objects
class SceneObject {
public:
    virtual Intersection intersect(const Ray& ray) = 0;
};

class Sphere : public SceneObject {
    vec3 center;
    float radius;

public:
    Sphere(const vec3& center, float radius) : center(center), radius(radius) {}

    Intersection intersect(const Ray& ray) override {
        // Sphere intersection logic...
        Intersection result;
        // Set result.hit, result.distance, result.point, result.normal
        return result;
    }
};

class Triangle : public SceneObject {
    vec3 v0, v1, v2;

public:
    Triangle(const vec3& v0, const vec3& v1, const vec3& v2) : v0(v0), v1(v1), v2(v2) {}

    Intersection intersect(const Ray& ray) override {
        // Implement Möller–Trumbore intersection algorithm
        Intersection result;
        // Calculate intersection, set result.hit, result.distance, result.point, result.normal
        return result;
    }
};

// Scene class containing all scene objects
class Scene {
    std::vector<SceneObject*> objects;

public:
    void addObject(SceneObject* object) {
        objects.push_back(object);
    }

    Intersection findClosestIntersection(const Ray& ray) {
        Intersection closestIntersection;
        for (auto& object : objects) {
            Intersection intersection = object->intersect(ray);
            if (intersection.hit && intersection.distance < closestIntersection.distance) {
                closestIntersection = intersection;
            }
        }
        return closestIntersection;
    }
};

// Function to find the color at a hit
vec3 FindColor(const Intersection& hit) {
    if (hit.hit) {
        //HW2
    }
    else {
        //Background color
    }
}

// Function to generate a ray through a pixel
//Ray RayThruPixel(const Camera& cam, int i, int j, int width, int height) {
    // Calculate direction of the ray from camera through the pixel
    // Simple perspective projection
    // vec3 direction = /* Calculate direction based on camera parameters and pixel coordinates */;
    // return Ray(cam.position, direction);
//}