#include <algorithm>
#include "variables.h"


Intersection Sphere::intersect(const Ray& ray) {
    // Sphere intersection logic...
    Intersection result;
    float bound = pow(10, -2);
    // Set result.hit, result.distance, result.point, result.normal
    float a = dot(ray.direction, ray.direction);
    float b = 2 * dot(ray.direction, (ray.origin - center));
    float c = dot((ray.origin - center), (ray.origin - center)) - radius * radius;

    float delta = b * b - 4 * a * c;
    if (delta < 0) {
        result.hit = false;
        return result;
    }
    float t1 = (-b + sqrt(delta)) / (2 * a);
    float t2 = (-b - sqrt(delta)) / (2 * a);
    vec3 p = vec3(0);
    float t = 0.0;
    vec3 n = vec3(0);

    if (t1 > (0 + bound) && t2 > (0 + bound)) {
        result.hit = true;
        if (t1 <= t2) {
            t = t1;
            p = ray.direction * t + ray.origin;
        }
        else {
            t = t2;
            p = ray.direction * t + ray.origin;
        }
    }
    else if (t1 > (0 + bound)) {
        t = t1;
        p = ray.direction * t + ray.origin;
    }
    else if (t2 > (0 + bound)) {
        t = t2;
        p = ray.direction * t + ray.origin;
    }
    else {
        return result;
    }
    if (this->hasTransf) {
        vec4 homPoint = this->m * vec4(p, 1);
        result.point = vec3(homPoint) / homPoint.w;
        mat3 q = transpose(this->mInverse);
        vec3 n = q * normalize(p - center);
        result.normal = normalize(n);
        vec4 homOrigin = this->m * vec4(ray.origin, 1);
        vec3 origin = vec3(homOrigin) / homOrigin.w;
        result.distance = length(result.point - origin);
        result.rayDir = this->m * vec4(ray.direction, 0);
    }
    else {
        n = normalize(p - center);
        result.distance = t;
        result.normal = n;
        result.point = p;
        result.rayDir = ray.direction;
    }
    result.hit = true;
    return result;
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection.html
Intersection Triangle::intersect(const Ray& ray) {
    // Implement Möller–Trumbore intersection algorithm
    Intersection result;
    //float bound = pow(10, -2);
    vec3 v0v1 = v1 - v0;
    vec3 v0v2 = v2 - v0;
    vec3 pvec = cross(ray.direction, v0v2);
    float det = dot(v0v1, pvec);
    // ray and triangle are parallel if det is close to 0
    /*
    *
    if (fabs(det) < kEpsilon) {
        result.hit = false;
        return result;
    }
    float invDet = 1 / det;
    */

    vec3 tvec = ray.origin - v0;
    float u = dot(tvec, pvec) / det;
    vec3 qvec = cross(tvec, v0v1);
    float v = dot(ray.direction, qvec) / det;
    if (u >= (0) && v >= (0) && u + v <= (1)) {
        result.hit = true;
    }
    else {
        result.hit = false;
        return result;
    }
    float t = dot(v0v2, qvec) / det;
    if (t < 0) {
        result.hit = false;
        return result;
    }
    vec3 p = ray.origin + t * ray.direction;
    vec3 n = normalize(cross(v0v1, v0v2));
    if (this->hasTransf) {
        vec4 homPoint = this->m * vec4(p, 1);
        result.point = vec3(homPoint) / homPoint.w;
        mat3 q = transpose(this->mInverse);
        result.normal = normalize(q * n);
        vec4 homOrigin = this->m * vec4(ray.origin, 1);
        vec3 origin = vec3(homOrigin) / homOrigin.w;
        result.distance = length(result.point - origin);
        result.rayDir = this->m * vec4(ray.direction, 0);
    }
    else {
        result.distance = t;
        result.normal = n;
        result.point = p;
        result.rayDir = ray.direction;
    }
    return result;
}

InterObject findBlockingObject(const Ray& ray) {
    Intersection closestIntersection;
    SceneObject* blocking = nullptr;
    Ray transfRay = ray;
    for (auto& object : currScene) {
        if (object->hasTransf) {
            vec4 homOrigin = object->mInverse * vec4(ray.origin, 1);
            vec3 newDirection = object->mInverse * vec4(ray.direction, 0);
            vec3 origin = vec3(homOrigin) / homOrigin.w;
            transfRay = Ray(origin, normalize(newDirection));
        }
        Intersection intersection = object->intersect(transfRay);
        if (intersection.hit && intersection.distance < closestIntersection.distance) {
            closestIntersection = intersection;
            blocking = object;
        }
        transfRay = ray;
    }
    return InterObject(closestIntersection, blocking);
}

// Function to find the color at a hit
vec3 findColor(const InterObject& inObj, Camera cam) {
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
                Ray dir = Ray(hit.point + direction * kEpsilon, direction);
                Ray transfDir = dir;
                for (auto& object : currScene) {
                    if (object->hasTransf) {
                        object->mInverse = inverse(object->m);
                        vec4 origin = object->mInverse * vec4(dir.origin, 1);
                        vec4 newDirection = object->mInverse * vec4(dir.direction, 0);
                        vec3 newOrigin = vec3(origin) / origin.w;
                        transfDir = Ray(newOrigin, normalize(vec3(newDirection)));
                    }
                    Intersection intersection = object->intersect(transfDir);
                    if (intersection.hit) {
                        blocked = true;
                        break;
                    }
                    transfDir = dir;
                }
                if (blocked) {
                    continue;
                }
                atten = 1.0;
                NL = dot(hit.normal, direction);
                vec3 halfvec = direction + normalize(-hit.rayDir);
                halfvec = glm::normalize(halfvec);
                NH = dot(hit.normal, halfvec);
            }
            else {
                vec3 direction = curr_light.location - hit.point;
                float dist = length(direction);
                direction = normalize(direction);
                Ray pt = Ray(hit.point + direction * kEpsilon, direction);
                Ray transfPt = pt;
                for (auto& object : currScene) {
                    if (object->hasTransf) {
                        object->mInverse = inverse(object->m);
                        vec4 origin = object->mInverse * vec4(pt.origin, 1);
                        vec4 newDirection = object->mInverse * vec4(pt.direction, 0);
                        vec3 newOrigin = vec3(origin) / origin.w;
                        transfPt = Ray(newOrigin, normalize(vec3(newDirection)));
                    }
                    Intersection intersection = object->intersect(transfPt);
                    if (intersection.hit && intersection.distance < dist) {
                        blocked = true;
                        break;
                    }
                    transfPt = pt;
                }
                if (blocked) {
                    continue;
                }
                atten = curr_light.attenuation[0] + curr_light.attenuation[1] * dist + curr_light.attenuation[2] * dist * dist;
                NL = dot(hit.normal, direction);
                vec3 halfvec = direction + normalize(-hit.rayDir);
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