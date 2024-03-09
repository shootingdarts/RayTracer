#include <glm/glm.hpp>
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
        float a = ray.direction * ray.direction;
        float b = 2 * ray.direction * (ray.origin - center);
        float c = (ray.center - center) * (ray.center - center) - radius * radius;

        float delta = b * b - 4 * a * c;
        if (delta < 0) {
            return result;
        }
        float t1 = -b + sqrt(delta) / 2 * a;
        float t2 = -b - sqrt(delta) / 2 * a;

        if (t1 > 0 && t2 > 0) {
            result.hit = true;
            if (t1 <= t2) {
                result.distance = t1;
                result.point = ray.direction * t1 + ray.origin;
                result.nomral = result.point - center;
            }
            else {
                result.distance = t2;
                result.point = ray.direction * t2 + ray.origin;
                result.nomral = result.point - center;
            }
        }
        else if (t1 > 0) {
            result.distance = t1;
            result.point = ray.direction * t1 + ray.origin;
            result.nomral = result.point - center;
        }
        else if (t2 > 0) {
            result.distance = t2;
            result.point = ray.direction * t2 + ray.origin;
            result.nomral = result.point - center;
        }
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
        vec3 normal = cross(v2 - v0, v1 - v0);
        normal = normalize(normal);
        float t = (dot(v0, normal) - dot(ray.origin, normal)) / dot(ray.direction, normal);
        vec3 p = ray.origin + t * ray.direction;
        float gamma = 0;
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