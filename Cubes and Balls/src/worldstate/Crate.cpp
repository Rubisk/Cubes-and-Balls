#include "Crate.h"

using namespace std;
using namespace glm;

shared_ptr<const Model> Crate::GetModel() const {
	return ModelLoader::GetModel("crate", vec3(1, 1, 1));
}

Material Crate::GetMaterial() const {
	Material mat;
	mat.color = vec4(0.5, 0.25, 0, 1);
	return mat;
}

float Crate::GetWeight() const {
	return 1.0f;
}
