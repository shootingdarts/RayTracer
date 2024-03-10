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

/*
Intersection Triangle::intersect(const Ray& ray){
    // Implement Möller–Trumbore intersection algorithm
    Intersection result;
    // Calculate intersection, set result.hit, result.distance, result.point, result.normal
    vec3 ba = v1 - v0;
    vec3 ca = v2 - v0;
    vec3 normal = cross(ba, ca);
    normal = normalize(normal);
    float t = (dot(v0, normal) - dot(ray.origin, normal)) / dot(ray.direction, normal);
    vec3 p = ray.origin + t * ray.direction;
    vec3 pa = p - v0;
    if (length(ba) < length(ca)) {
        vec3 temp = ca;
        ca = ba;
        ba = temp;
    }
    float gamma = (pa.x * ba.y / ba.x - pa.y) / (ba.y * ca.x / ba.x - ca.y);
    float beta = (pa.y - gamma * ca.y) / ba.y;

    float gamma = (pa.y - (ba.y * pa.x / ba.x)) / (ca.y - (ca.x * ba.y / ba.x));
    float beta = (pa.y - gamma * ca.y) / ba.y;

    result.hit = 0 < beta && beta < 1 && 0 < gamma && gamma < 1 && beta + gamma <= 1;
    if (result.hit) {
        result.distance = t;
        result.point = p;
        result.normal = normal;

        //std::cout << normal.x << " " << normal.y << " " << normal.z << endl;
    }
    return result;
}

*/

// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection.html
Intersection Triangle::intersect(const Ray& ray) {
    Intersection result;
    vec3 v0v1 = v1 - v0;
    vec3 v0v2 = v2 - v0;
    vec3 pvec = cross(ray.direction, v0v2);
    float det = dot(v0v1, pvec);
    // ray and triangle are parallel if det is close to 0
    if (fabs(det) < kEpsilon) {
        result.hit = false;
        return result;
    }
    float invDet = 1 / det;

    vec3 tvec = ray.origin - v0;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) {
        result.hit = false;
        return result;
    }
    vec3 qvec = cross(tvec, v0v1);
    float v = dot(ray.direction, qvec) * invDet;
    if (v < 0 || u + v > 1) {
        result.hit = false;
        return result;
    }

    float t = dot(v0v2, qvec) * invDet;

    result.distance = t;
    result.hit = true;
    result.normal = normalize(cross(v0v2, v0v1));
    result.point = ray.origin + t * ray.direction;;
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

InterObject findBlockingObject(const Ray& ray) {
    Intersection closestIntersection;
    SceneObject* blocking = nullptr;
    bool hitt = false;
    int i = 1;
    for (auto& object : currScene) {
        Intersection intersection = object->intersect(ray);
        /*
        if (intersection.hit && i == 1) {
            cout << "Triangle 1" << endl;
        }
        else if (intersection.hit && i == 2) {
            cout << "Triangle 2" << endl;
        }
        */
        if (intersection.hit && intersection.distance < closestIntersection.distance) {
            closestIntersection = intersection;
            blocking = object;
        }
        i++;
    }
    return InterObject(closestIntersection, blocking);
}

// Function to find the color at a hit
vec3 findColor(const InterObject& inObj) {
    vec3 I = vec3(0, 0, 0);
    float NL = 0.0;
    float NH = 0.0;
    bool blocked;
    Intersection hit = inObj.intsect;
    SceneObject* obj_light = inObj.obj_light;
    float atten = 1.0;

    if (hit.hit) {
        for (int i = 0; i < lights.size(); i++) {
            blocked = false;
            Light curr_light = lights[i];

            if (curr_light.type == directional) {
                vec3 direction = normalize(curr_light.direction);
                Ray dir = Ray(hit.point + direction * float(pow(10.0, -6.0)), direction);
                
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
                
                atten = 1.0;
                NL = dot(hit.normal, direction);
                vec3 halfvec = direction + (cam.position - hit.point);
                halfvec = glm::normalize(halfvec);
                NH = dot(hit.normal, halfvec);
            }
            else {
                vec3 direction = curr_light.location - hit.point;
                float dist = length(direction);
                direction = normalize(direction);
                Ray pt = Ray(hit.point + direction * float(pow(10.0, -6.0)), direction);
                for (auto& object : currScene) {
                    Intersection intersection = object->intersect(pt);
                    if (intersection.hit && intersection.distance < dist) {
                        blocked = true;
                        break;
                    }
                }
                if (blocked) {
                    continue;
                }
                atten = curr_light.attenuation[0] + curr_light.attenuation[1] * dist + curr_light.attenuation[2] * dist * dist;
                NL = dot(hit.normal, direction);
                vec3 halfvec = direction + (cam.position - hit.point);
                halfvec = glm::normalize(halfvec);
                NH = dot(hit.normal, halfvec);
            }
            vec3 eachLight = obj_light->diffuse * std::max(NL, float(0.0)) + obj_light->specular * pow(std::max(NH, float(0.0)), obj_light->shininess);
            eachLight = (curr_light.color / atten) * eachLight;
            I = I + eachLight;
        }
        I = I + obj_light->ambient + obj_light->emission;
    }
    else {
        I = vec3(0, 0, 0);
    }
    return I;
}