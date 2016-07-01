#include "Player.h"

#include <memory>

#include "worldstate/Object.h"

using namespace std;
using namespace glm;

void Player::LinkCamera(weak_ptr<Camera>& camera) {
	camera_ = camera;
}

void Player::SetPosition(vec3 &pos) {
	if (!camera_.expired()) {
		camera_.lock()->SetPosition(pos);
	}
	Entity::SetPosition(pos);
}

void Player::Rotate(const vec3 &axis, float radians) {
	if (!camera_.expired()) {
		camera_.lock()->Rotate(axis, radians);
	}
	Entity::Rotate(axis, radians);
}

float Player::GetWeight() {
	return 2.0f;
}

std::string Player::GetModelName() {
	return "";
}
