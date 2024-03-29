#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <glm/glm.hpp>

using namespace std;
#include "readfile.h"
#include "Transform.h"
#include "variables.h"

void matransform(stack<mat4>& transfstack, vec3& values)
{
    mat4 transform = transfstack.top();
    vec4 valvec = vec4(values[0], values[1], values[2], 1);
    vec4 newval = transform * valvec;
    for (int i = 0; i < 3; i++) values[i] = newval[i] / newval[3];
}

void rightmultiply(const mat4& M, stack<mat4>& transfstack)
{
    mat4& T = transfstack.top();
    T = T * M;
}

bool readvals(stringstream& s, const int numvals, float* values)
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

void readfile(const char* filename)
{
    string str, cmd;
    ifstream in;
    in.open(filename);
    if (in.is_open()) {

        stack <mat4> transfstack;
        transfstack.push(mat4(1.0));
        bool hasTransf = false;

        getline(in, str);
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                stringstream s(str);
                s >> cmd;
                int i;
                float values[10];
                bool validinput;
                if (cmd == "camera") {
                    validinput = readvals(s, 10, values);
                    Camera* cam = new Camera();
                    vec3 center = vec3(values[3], values[4], values[5]);
                    vec3 up = vec3(values[6], values[7], values[8]);
                    cam->position = vec3(values[0], values[1], values[2]);
                    cam->w = normalize(cam->position - center);
                    cam->u = normalize(cross(up, cam->w));
                    cam->v = cross(cam->w, cam->u);
                    cam->fovy = values[9] / 180 * pi;
                    cameras.push_back(cam);
                }
                else if (cmd == "attenuation") {
                    validinput = readvals(s, 3, values);
                    currAttenuation = vec3(values[0], values[1], values[2]);
                }
                else if (cmd == "directional") {
                    validinput = readvals(s, 6, values);
                    Light direct;
                    direct.attenuation = currAttenuation;
                    direct.direction = vec3(values[0], values[1], values[2]);
                    direct.type = directional;
                    direct.color = vec3(values[3], values[4], values[5]);
                    lights.push_back(direct);
                }
                else if (cmd == "point") {
                    validinput = readvals(s, 6, values);
                    Light pt;
                    pt.attenuation = currAttenuation;
                    pt.location = vec3(values[0], values[1], values[2]);
                    pt.type = point;
                    pt.color = vec3(values[3], values[4], values[5]);
                    lights.push_back(pt);
                }
                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values);
                    currAmbient = vec3(values[0], values[1], values[2]);
                }
                else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values);
                    currDiffuse = vec3(values[0], values[1], values[2]);
                }
                else if (cmd == "specular") {
                    validinput = readvals(s, 3, values);
                    currSpecular = vec3(values[0], values[1], values[2]);
                }
                else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values);
                    currShininess = values[0];
                }
                else if (cmd == "emission") {
                    validinput = readvals(s, 3, values);
                    currEmission = vec3(values[0], values[1], values[2]);
                }
                else if (cmd == "maxverts") {
                    validinput = readvals(s, 1, values);
                    maxvertex = values[0];
                }
                else if (cmd == "vertex") {
                    validinput = readvals(s, 3, values);
                    vertices.push_back(vec3(values[0], values[1], values[2]));
                }
                else if (cmd == "sphere") {
                    validinput = readvals(s, 4, values);
                    vec3 center = vec3(values[0], values[1], values[2]);
                    Sphere* sph = new Sphere(center, values[3]);
                    if (hasTransf) { 
                        sph->hasTransf = true;
                        sph->m = transfstack.top();
                        sph->mInverse = inverse(sph->m);
                    }
                    else {
                        sph->hasTransf = false;
                    }
                    sph->ambient = currAmbient;
                    sph->diffuse = currDiffuse;
                    sph->emission = currEmission;
                    sph->shininess = currShininess;
                    sph->specular = currSpecular;
                    currScene.push_back(sph);
                }
                else if (cmd == "tri") {
                    validinput = readvals(s, 3, values);
                    vec3 v0 = vertices[values[0]];
                    vec3 v1 = vertices[values[1]];
                    vec3 v2 = vertices[values[2]];
                    Triangle* tri = new Triangle(v0, v1, v2);
                    if (hasTransf) {
                        tri->hasTransf = true;
                        tri->m = transfstack.top();
                        tri->mInverse = inverse(tri->m);
                    }
                    else {
                        tri->hasTransf = false;
                    }
                    tri->ambient = currAmbient;
                    tri->diffuse = currDiffuse;
                    tri->emission = currEmission;
                    tri->shininess = currShininess;
                    tri->specular = currSpecular;
                    currScene.push_back(tri);
                }
                else if (cmd == "size") {
                    validinput = readvals(s, 2, values);
                    w = values[0];
                    h = values[1];
                }
                else if (cmd == "maxdepth") {
                    validinput = readvals(s, 1, values);
                    maxDepth = values[0];
                }
                else if (cmd == "output") {
                    s >> outputFile;
                }
                else if (cmd == "translate") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        rightmultiply(Transform::translate(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s, 4, values);
                    if (validinput) {
                        rightmultiply(mat4(Transform::rotate(values[3], vec3(values[0], values[1], values[2]))), transfstack);
                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                    hasTransf = true;
                }
                else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n";
                    }
                    else {
                        transfstack.pop();
                        hasTransf = false;
                    }
                }
                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }
            getline(in, str);
        }
    }
    else {
        cerr << "Unable to Open Input Data File " << filename << "\n";
        throw 2;
    }
}