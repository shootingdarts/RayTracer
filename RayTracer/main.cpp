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


using namespace std;

Ray rayThruPixel(Camera cam, float i, float j) {
	float alpha = w / h * tanf(cam.fovy / 2) * ((j - w / 2) / (w / 2));
	float beta = tanf(cam.fovy / 2) * (h / 2 - i) / (h / 2);
	vec3 p1 = alpha * cam.u + beta * cam.v + cam.direction;
	return Ray(cam.position, normalize(p1));
}

BYTE* raytrace(Camera cam, Scene scene, int width, int height, BYTE* pixels) {
	BYTE* image = pixels;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Ray ray = rayThruPixel(cam, i + 0.5, j + 0.5);
			Intersection hit = scene.findClosestIntersection(ray);
			vec3 color = findColor(hit);
			int pixelNumber = j * width + i;
			pixels[pixelNumber * 3] = color.z;
			pixels[pixelNumber * 3 + 1] = color.y;
			pixels[pixelNumber * 3 + 2] = color.x;
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
	FreeImage_Initialise();
	int pix = w * h;
	BYTE* pixels = new BYTE[3*pix];
	raytrace(cam, currScene, w, h, pixels);
	FIBITMAP* img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
	FreeImage_Save(FIF_PNG, img, outputFile.c_str(), 0);
	FreeImage_DeInitialise();
	delete pixels;
	return 0;
}