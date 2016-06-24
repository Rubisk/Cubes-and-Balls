#include "ModelLoader.h"

#include "GL/glew.h"

using namespace std;

Model ModelLoader::GetModel(const string &name) {
	if (models_.find(name) != models_.end()) 
		return models_.find(name)->second;
	else {
		// TODO implement actual model loading
		Model model;
		models_[name] = model;
	}
	return models_[name];
}
