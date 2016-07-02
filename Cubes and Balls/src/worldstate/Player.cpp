#include "Player.h"

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"


using namespace glm;

void Player::LookAt(const vec3 &point) {
	vec3 facing = point - GetPosition();
	SetFrontSide(facing);
}

float Player::GetWeight() const {
	return 2.0f;
}
