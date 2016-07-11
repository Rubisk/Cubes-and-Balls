#include "Crate.h"

using namespace std;
using namespace glm;

shared_ptr<const Model> Crate::GetModel() const {
	return ModelLoader::GetModel("crate", vec3(1, 1, 1));
}

float Crate::GetWeight() const {
	return 1.0f;
}
