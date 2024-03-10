#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <glm/glm.hpp>

#include "include/FreeImage.h"
#include "Transform.h"
#include "readfile.h"
#define MAINPROGRAM 
#include "variables.h"
#include "variables.cpp"


using namespace std;

Ray rayThruPixel(Camera cam, float i, float j) {
	float alpha = float(w) / h * tanf(cam.fovy / 2) * ((j - w / 2) / (w / 2));
	float beta = tanf(cam.fovy / 2) * (h / 2 - i) / (h / 2);
	vec3 p1 = alpha * cam.u + beta * cam.v - cam.w;
	return Ray(cam.position, normalize(p1));
}

BYTE* raytrace(Camera cam, int width, int height, BYTE* pixels) {
	BYTE* image = pixels;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Ray ray = rayThruPixel(cam, i + 0.5, j + 0.5);
			InterObject hit = findBlockingObject(ray);
			vec3 color = findColor(hit);
			int pixelNumber = i * width + j;
			image[pixelNumber * 3] = 255 * color.z;
			image[pixelNumber * 3 + 1] = 255 * color.y;
			image[pixelNumber * 3 + 2] = 255 * color.x;
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
	kEpsilon = pow(10, -6);
	currAmbient = vec3(0.2, 0.2, 0.2);
	currDiffuse = vec3(0);
	currSpecular = vec3(0);
	currShininess = 1;
	currEmission = vec3(0);
	currAttenuation = vec3(1, 0, 0);
	outputFile = "Image.png";
	readfile(argv[1]);
	/*
	for (auto object : currScene) {
		SceneObject* tri = object;
		cout << object->ambient.x << endl;
	}
	for (auto vertex : vertices) {
		cout << vertex.x << vertex.y << vertex.z << endl;
	}
	for (auto light : lights) {
		cout << light.type << endl;
	}
	*/
	FreeImage_Initialise();
	int pix = w * h;
	BYTE* pixels = new BYTE[3*pix];
	pixels = raytrace(cam, w, h, pixels);
	FIBITMAP* img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
	FreeImage_Save(FIF_PNG, img, outputFile.c_str(), 0);
	FreeImage_DeInitialise();
	delete pixels;
	return 0;
}