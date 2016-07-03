#include "ModelLoader.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "Windows.h"

#include "GL/glew.h"
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

shared_ptr<const Model> ModelLoader::GetModel(const string &name) {
	if (models_.find(name) == models_.end()) {
		TryLoadModel_(name);
	}
	return models_[name];
}

void ModelLoader::TryLoadModel_(const string &name) {
	// TODO setup proper exception handling
	string path = "res/models/" + name + ".model";
	ifstream modelData(path);
	string line;

	Model model;

	while (getline(modelData, line)) {
		if (line == "") continue;
		stringstream lineStream(line);
		char first;
		lineStream >> first;
		switch (first) {
		case '#':
			continue;
		case 'v':
		{
			// x, y, z are the position, nx, ny, nz are the normal vertex.
			GLfloat x, y, z, nx, ny, nz;
			lineStream >> x >> y >> z >> nx >> ny >> nz;
			model.vertices.push_back(x);
			model.vertices.push_back(y);
			model.vertices.push_back(z);
			model.vertices.push_back(nx);
			model.vertices.push_back(ny);
			model.vertices.push_back(nz);
			break;
		}
		case 'f':
		{
			// A face is represented by 3 vertices in clock-wise order.
			GLuint f1, f2, f3;
			lineStream >> f1 >> f2 >> f3;
			model.elements.push_back(f1);
			model.elements.push_back(f2);
			model.elements.push_back(f3);
			break;
		}
		default:
			cout << "Unkwown line while loading model " << name << ":\""
				<< line << "\" ignoring and loading the next line.";
		}
	}
	models_[name] = make_shared<const Model>(model);
}
