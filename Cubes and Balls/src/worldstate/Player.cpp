#include "Player.h"

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"


using namespace glm;

std::shared_ptr<const Model> Player::GetModel() const {
	return ModelLoader::GetModel("crate", vec3(1, 2, 1));
}

Material Player::GetMaterial() const {
	Material mat;
	mat.color = vec4(0, 0, 0, 0);
	return mat;
}

void Player::LookAt(const vec3 &point) {
	vec3 facing = point - GetPosition();
	SetFrontSide(facing);
}

float Player::GetWeight() const {
	return 2.0f;
}
