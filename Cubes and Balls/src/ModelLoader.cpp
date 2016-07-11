#include "ModelLoader.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "Windows.h"

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace std;
using namespace glm;

map<string, shared_ptr<const Model>> ModelLoader::models_;

shared_ptr<const Model> ModelLoader::GetModel(const string &name) {
	return GetModel(name, vec3(1, 1, 1));
}

shared_ptr<const Model> ModelLoader::GetModel(const string &name, const vec3 &scale) {
	string modelLookup = name + to_string(scale);
	if (models_.find(modelLookup) == models_.end()) {
		TryLoadModel_(name, scale);
	}
	return models_[modelLookup];
}

void ModelLoader::TryLoadModel_(const string &name, const vec3 &scalevec) {
	// TODO setup proper exception handling
	string path = "res/models/" + name + ".model";
	ifstream modelData(path);
	string line;

	Model model;

	mat3 vertexScale = mat3(scale(scalevec));
	mat3 normalScale = inverse(vertexScale);

	while (getline(modelData, line)) {
		if (line == "") continue;
		stringstream lineStream(line);
		char first;
		lineStream >> first;
		switch (first) {
		case '#':
			continue;
		case 'r':
		{
			lineStream >> model.minRadius >> model.maxRadius;
			break;
		}
		case 'v':
		{
			// x, y, z are the position, nx, ny, nz are the normal vertex.
			GLfloat x, y, z, nx, ny, nz;
			lineStream >> x >> y >> z >> nx >> ny >> nz;
			vec3 position(x, y, z);
			vec3 normal(nx, ny, nz);
			position = vertexScale * position;
			normal = normalScale * normal;
			model.vertices.push_back(position.x);
			model.vertices.push_back(position.y);
			model.vertices.push_back(position.z);
			model.vertices.push_back(normal.x);
			model.vertices.push_back(normal.y);
			model.vertices.push_back(normal.z);
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
	models_[name + to_string(scalevec)] = make_shared<const Model>(model);
}
