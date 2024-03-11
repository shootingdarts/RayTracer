#pragma once
#include <sstream>
#include <glm/glm.hpp>

using namespace std;

void matransform(stack<glm::mat4>& transfstack, glm::vec3& values);
void rightmultiply(const glm::mat4& M, stack<glm::mat4>& transfstack);
bool readvals(stringstream & s, const int numvals, float* values);
void readfile(const char* filename);