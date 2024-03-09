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