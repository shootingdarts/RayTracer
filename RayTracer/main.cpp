#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>

#include "include/FreeImage.h"
#include "readfile.h"
#include "Transform.h"
#include <glm/glm.hpp>

using namespace std;

int main(int argc, char* argv[]) {
	cout << "hello" << endl;

	/*if (argc < 2) {
		cerr << "Usage: transforms scenefile [grader input (optional)]\n";
		exit(-1);
	}*/

	FreeImage_Initialise();
	FreeImage_DeInitialise();
	glm::mat4 modelview(1.0);

	glm::mat4 scale = Transform::scale(2, 2, 2);

	cout << scale[2][2] << endl;

	return 0;
}