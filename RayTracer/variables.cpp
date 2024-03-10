#include <algorithm>
#include "variables.h"


Intersection Sphere::intersect(const Ray& ray) {
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
    float t1 = -b + sqrt(delta) / (2 * a);
    float t2 = -b - sqrt(delta) / (2 * a);

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

Intersection Triangle::intersect(const Ray& ray){
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

Intersection findClosestIntersection(const Ray& ray) {
    Intersection closestIntersection;
    for (auto& object : currScene) {
        Intersection intersection = object->intersect(ray);
        if (intersection.hit && intersection.distance < closestIntersection.distance) {
            closestIntersection = intersection;
        }
    }
    return closestIntersection;
}

// Function to find the color at a hit
vec3 findColor(const Intersection& hit) {
    vec3 I = vec3(0, 0, 0);
    float NL;
    float NH;

    if (hit.hit) {
        for (int i = 0; i < lights.size(); i++) {
            Light curr_light = lights[i];
            bool blocked = false;

            if (curr_light.type == 0) {
                Ray dir = Ray(hit.point, -curr_light.direction);
                for (auto& object : currScene) {
                    Intersection intersection = object->intersect(dir);
                    if (intersection.hit) {
                        blocked = true;
                        break;
                    }
                }
                if (blocked) {
                    continue;
                }
                NL = dot(hit.normal, curr_light.direction);
                vec3 halfvec = curr_light.direction + (cam.position - hit.point);
                halfvec = glm::normalize(halfvec);
                NH = dot(hit.normal, halfvec);
            }
            else {
                vec3 direction = curr_light.location - hit.point;
                Ray pt = Ray(hit.point + direction * float(pow(10, -6)), direction);
                for (auto& object : currScene) {
                    Intersection intersection = object->intersect(pt);
                    if (intersection.hit && intersection.distance < glm::length(direction)) {
                        blocked = true;
                        break;
                    }
                }
                if (blocked) {
                    continue;
                }
                NL = dot(hit.normal, (curr_light.location - hit.point));
                vec3 halfvec = (curr_light.location - hit.point) + (cam.position - hit.point);
                halfvec = glm::normalize(halfvec);
                NH = dot(hit.normal, halfvec);
            }
            vec3 eachLight = hit.obj_light->diffuse * std::max(NL, float(0.0)) + hit.obj_light->specular * pow(std::max(NH, float(0.0)), hit.obj_light->shininess);
            eachLight = (curr_light.color / curr_light.attenuation) * eachLight;
            I = I + eachLight;
        }
        I = I + hit.obj_light->ambient + hit.obj_light->emission;
    }
    else {
        I = vec3(0, 0, 0);
    }
    return I;
}