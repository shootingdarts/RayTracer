#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <glm/glm.hpp>

using namespace std;
#include "readfile.h"
#include "variables.h"
#include "Transform.h"

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
                    for (i = 0; i < 10; i++) {
                        vec3 center = vec3(values[3], values[4], values[5]);
                        vec3 up = vec3(values[6], values[7], values[8]);
                        cam.position = vec3(values[0], values[1], values[2]);
                        cam.direction = normalize(cam.position - center);
                        cam.u = normalize(cross(up, cam.direction));
                        cam.v = cross(cam.direction, cam.u);
                        cam.fovy = values[9];
                    }
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
                    lights.push_back(direct);
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
                else if (cmd == "shiniess") {
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
                    Sphere sph = Sphere(vec3(values[0], values[1], values[2]), values[3]);
                    sph.ambient = currAmbient;
                    sph.diffuse = currDiffuse;
                    sph.emission = currEmission;
                    sph.shininess = currShininess;
                    sph.specular = currSpecular;
                    currScene.addObject(&sph);
                }
                else if (cmd == "tri") {
                    validinput = readvals(s, 3, values);
                    Triangle tri = Triangle(vertices[values[0]], vertices[values[1]], vertices[values[2]]);
                    tri.ambient = currAmbient;
                    tri.diffuse = currDiffuse;
                    tri.emission = currEmission;
                    tri.shininess = currShininess;
                    tri.specular = currSpecular;
                    currScene.addObject(&tri);
                }
                else if (cmd == "size") {
                    validinput = readvals(s, 2, values);
                    w = values[0];
                    h = values[1];
                }
                else if (cmd == "maxdepth") {
                    validinput = readvals(s, 1, values);
                    depth = values[0];
                }
                else if (cmd == "output") {
                    s >> outputFile;
                }
                else if (cmd == "sphere") {
                    validinput = readvals(s, 4, values);

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