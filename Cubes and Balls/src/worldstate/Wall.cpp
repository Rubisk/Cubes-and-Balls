#include "Wall.h"

using namespace glm;

Wall::Wall(vec3 size) {
	size_ = size;
}


std::shared_ptr<const Model> Wall::GetModel() const {
	return ModelLoader::GetModel("crate", size_);
}

Material Wall::GetMaterial() const {
	Material mat;
	mat.color = vec4(1, 1, 1, 1);
	return mat;
}
