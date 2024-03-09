#include <glm/glm.hpp>
#include "variables.h"

// Abstract class for scene objects
class SceneObject {
public:
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    vec3 emission;
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
        float a = dot(ray.direction, ray.direction);
        float b = 2 * dot(ray.direction, (ray.origin - center));
        float c = dot((ray.origin - center), (ray.origin - center)) - radius * radius;

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
                result.normal = result.point - center;
            }
            else {
                result.distance = t2;
                result.point = ray.direction * t2 + ray.origin;
                result.normal = result.point - center;
            }
        }
        else if (t1 > 0) {
            result.distance = t1;
            result.point = ray.direction * t1 + ray.origin;
            result.normal = result.point - center;
        }
        else if (t2 > 0) {
            result.distance = t2;
            result.point = ray.direction * t2 + ray.origin;
            result.normal = result.point - center;
        }
        return result;
    }
};

class Triangle : public SceneObject {
    vec3 v0, v1, v2;

public:
    Triangle(const vec3& v0, const vec3& v1, const vec3& v2) : v0(v0), v1(v1), v2(v2) {}

    Intersection intersect(const Ray& ray) override {
        // Implement M�ller�Trumbore intersection algorithm
        Intersection result;
        // Calculate intersection, set result.hit, result.distance, result.point, result.normal
        vec3 ba = v1 - v0;
        vec3 ca = v2 - v0;
        vec3 normal = cross(ca, ba);
        normal = normalize(normal);
        float t = (dot(v0, normal) - dot(ray.origin, normal)) / dot(ray.direction, normal);
        vec3 p = ray.origin + t * ray.direction;
        vec3 pa = p - v0;
        float gamma = (pa.x * ba.y / ba.x - pa.y) / (ba.y * ca.x / ba.x - ca.y);
        float beta = (pa.y - gamma * ca.y) / ba.y;
        result.hit = 0 < beta && beta < 1 && 0 < gamma && gamma < 1 && beta + gamma <= 1;
        if (result.hit) {
            result.distance = t;
            result.point = p;
            result.normal = normal;
        }
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
vec3 findColor(const Intersection& hit) {
    vec3 I;

    if (hit.hit) {
        for (int i = 0; i < lights.size(); i++) {
            Light curr_light = lights[i];
            if (curr_light.type == 0) {
                float NL = dot(hit.normal, curr_light.direction);
                vec3 halfvec = curr_light.direction + (cam.position - hit.point);
                float NH = dot(hit.normal, halfvec);
            }
            else {
                float NL = dot(hit.normal, (curr_light.locaiton - hit.point));
                vec3 halfvec = (curr_light.location - hit.point) + (cam.position - hit.point);
                float NH = dot(hit.normal, halfvec);
            }
            vec3 eachLight = dot(hit.obj_light.diffuse, max(NL, 0)) + dot(hit.obj_light.specular, power(max(NH, 0), hit.obj_light.shininess));
            eachLight 
            I = I + eachLight;
        }
        I = hit.obj_light.ambient + hit.obj_light.emission;
    }
    else {
        
    }
    return I;
}