#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <glm/glm.hpp>

#include "include/FreeImage.h"
#include "Transform.h"

#define MAINPROGRAM 
#include "readfile.h"
#include "variables.h"
#include "variables.cpp"


using namespace std;

Image Raytrace(Camera cam, Scene scene, int width, int height) {
	Image image = new Image(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Ray ray = RayThruPixel(cam, i, j);
			Intersection hit = Intersect(ray, scene);
			image[i][j] = FindColor(hit);
		}
	}
	return image;
}


int main(int argc, char* argv[]) {
	cout << "hello" << endl;

	if (argc < 2) {
		cerr << "Usage: transforms scenefile [grader input (optional)]\n";
		exit(-1);
	}
	readfile(argv[1]);
	cout << fovy << endl;
	
	for (int i = 0; i < 3; i++) {
		cout << eyeinit[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < 3; i++) {
		cout << center[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < 3; i++) {
		cout << upinit[i] << " ";
	}
	cout << endl;
	cout << "width: " << w << endl;
	cout << "height: " << h << endl;
	cout << "depth: " << depth << endl;
	cout << "output: " << outputFile << endl;
	FreeImage_Initialise();
	FreeImage_DeInitialise();
	return 0;
}