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

Model ModelLoader::GetModel(const string &name) {
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
		stringstream lineStream(line);
		char first;
		lineStream >> first;
		switch (first) {
		case '#':
			continue;
		case 'v':
		{
			GLfloat x, y, z;
			lineStream >> x >> y >> z;
			model.vertices.push_back(x);
			model.vertices.push_back(y);
			model.vertices.push_back(z);
			break;
		}
		case 'e':
		{
			GLuint x, y, z;
			lineStream >> x >> y >> z;
			model.elements.push_back(x);
			model.elements.push_back(y);
			model.elements.push_back(z);
			break;
		}
		default:
			cout << "Unkwown line while loading model " << name << ":\""
				<< line << "\" ignoring and loading the next line.";
		}
	}
	models_[name] = model;
}
