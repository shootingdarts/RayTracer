// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    mat3 ret;
    // YOUR CODE FOR HW2 HERE
    // Please implement this.  Likely the same as in HW 1.
    vec3 axisnorm = normalize(axis);
    float x = axisnorm.x;
    float y = axisnorm.y;
    float z = axisnorm.z;
    float theta = degrees * pi / 180;
    mat3 m1(1, 0, 0, 0, 1, 0, 0, 0, 1);
    mat3 m2(x * x, x * y, x * z, x * y, y * y, y * z, x * z, y * z, z * z);
    mat3 m3(0, z, -y, -z, 0, x, y, -x, 0);
    ret = cosf(theta) * m1 + (1 - cosf(theta)) * m2 + sinf(theta) * m3;
    return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.
    mat3 rotation = Transform::rotate(degrees, up);
    eye = rotation * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    // Likely the same as in HW 1.
    vec3 u = cross(up, -normalize(eye));
    mat3 rotation = Transform::rotate(degrees, u);
    eye = rotation * eye;
    up = rotation * up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.
    vec3 w = normalize(eye - center);
    vec3 u = normalize(cross(up, w));
    vec3 v = cross(w, u);
    mat4 rotation(vec4(u, 0), vec4(v, 0), vec4(w, 0), vec4(0, 0, 0, 1));
    mat4 translation(1);
    translation[3] = vec4(-eye, 1);
    ret = transpose(rotation) * translation;
    return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.
    float theta = fovy * pi / 180;
    float d = 1 / tanf(theta / 2);
    ret = mat4(d / aspect, 0, 0, 0, 0, d, 0 ,0, 0, 0, -((zFar + zNear) / (zFar - zNear)), -1, 0, 0, -((2 * zFar * zNear) / (zFar - zNear)), 0);
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement scaling
    ret = mat4(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
    ret = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, tx, ty, tz, 1);
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
