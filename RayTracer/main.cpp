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

Ray rayThruPixel(Camera cam, float i, float j) {
	float alpha = w / h * tanf(cam.fovy / 2) * ((j - w / 2) / (w / 2));
	float beta = tanf(cam.fovy / 2) * (h / 2 - i) / (h / 2);
	vec3 p1 = alpha * cam.u + beta * cam.v + cam.direction;
	return Ray(cam.position, normalize(p1));
}

Image Raytrace(Camera cam, Scene scene, int width, int height) {
	Image image = Image(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Ray ray = rayThruPixel(cam, i + 0.5, j + 0.5);
			Intersection hit = scene.findClosestIntersection(ray);
			vec3 color = FindColor(hit);
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

	int pix = w * h;
	BYTE* pixels = new BYTE[3*pix];
	pixels[0] = 0;
	pixels[1] = 0;
	pixels[2] = 0;
	pixels[6] = 255;
	pixels[7] = 0;
	pixels[8] = 0;
	FIBITMAP* img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FIF_PNG, img, outputFile.c_str(), 0);

	FreeImage_DeInitialise();
	delete pixels;
	return 0;
}