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

vec3 recursiveFindColor(const Ray& ray, Camera cam, int currDepth) {
	InterObject inObj = findBlockingObject(ray);
	vec3 I = vec3(0.0);
	if (currDepth == maxDepth) {
		return I;
	}
	if (!inObj.intsect.hit) {
		return I;
	}
	vec3 s = inObj.obj_light->specular;
	if (s == vec3(0.0)) {
		return findColor(inObj, cam);
	}
	vec3 n = inObj.intsect.normal;
	vec3 d = ray.direction;
	vec3 r = -2 * dot(d, n) * n + d;
	I += findColor(inObj, cam) + s * recursiveFindColor(Ray(inObj.intsect.point, normalize(r)), cam, currDepth + 1);
	return I;
}

BYTE* raytrace(Camera cam, int width, int height, BYTE* pixels) {
	BYTE* image = pixels;
	int stage = 1;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Ray ray = rayThruPixel(cam, i + 0.5, j + 0.5);
			vec3 color = recursiveFindColor(ray, cam, 0);
			int pixelNumber = i * width + j;
			image[pixelNumber * 3] = 255 * color.z;
			image[pixelNumber * 3 + 1] = 255 * color.y;
			image[pixelNumber * 3 + 2] = 255 * color.x;
			if (((pixelNumber + 1) / float(width * height)) >= (0.20 * stage)) {
				cout << to_string(20 * stage) + "%" << endl;
				stage++;
			}
		}
	}
	return image;
}

void resetScene() {
	kEpsilon = pow(10, -3);
	currAmbient = vec3(0.2, 0.2, 0.2);
	currDiffuse = vec3(0);
	currSpecular = vec3(0);
	currShininess = 1;
	currEmission = vec3(0);
	currAttenuation = vec3(1, 0, 0);
	maxDepth = 10;
	for (auto camera : cameras) {
		delete camera;
	}
	for (auto object : currScene) {
		delete object;
	}
	cameras.clear();
	currScene.clear();
	lights.clear();
	vertices.clear();
}

int main(int argc, char* argv[]) {
	cout << "hello" << endl;
	if (argc < 2) {
		cerr << "Usage: transforms scenefile [grader input (optional)]\n";
		exit(-1);
	}
	// set variables to default
	resetScene();
	FreeImage_Initialise();
	// parse in commands
	for (int j = 1; j < argc; j++) {
		readfile(argv[j]);
		int pix = w * h;
		BYTE* pixels = new BYTE[3 * pix];
		cout << to_string(cameras.size()) + " total cameras loaded." << endl;
		for (int i = 1; i <= cameras.size(); i++) {
			cout << "Rendering..." << endl;
			pixels = raytrace(*cameras[i - 1], w, h, pixels);
			FIBITMAP* img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
			string testName = argv[j];
			string withOut = "";
			for (auto character : testName) {
				if (character == '.') {
					break;
				}
				withOut.push_back(character);
			}
			outputFile = "images/" + outputFile;
			//outputFile = "images/" + withOut + "-Image" + to_string(i) + ".png";
			FreeImage_Save(FIF_PNG, img, outputFile.c_str(), 0);
			cout << "Successfully saved " + withOut + "-Image" + to_string(i) + ".png" << endl;
		}
		resetScene();
		delete pixels;
	}
	FreeImage_DeInitialise();
	return 0;
}