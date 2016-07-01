#include "Player.h"

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"


using namespace glm;

// This file assumes the two vectors in orientation_ are always orthogonal.
// Upon changing them, please KEEP THIS PROPERTY.

void Player::LookAt(const vec3 &point) {
	vec3 facing = point - GetPosition();
	SetFrontSide(facing);
}

void Player::RotateLocal(const vec3 &axis, float radians) {
	vec3 worldAxis = vec3(LocalToWorldSpaceMatrix() * vec4(axis, 0));
	Rotate(worldAxis, radians);
}

void Player::MoveLocal(const vec3 &distance) {
	vec3 worldDistance = vec3(LocalToWorldSpaceMatrix() * vec4(distance, 0));
	Move(worldDistance);
}

vec3 Player::GetPointLookingAt(float distance) const {
	return GetPosition() + distance * GetFrontSide();
}

mat4 Player::LocalToWorldSpaceMatrix() const {
	vec3 up = GetUpSide();
	vec3 front = GetFrontSide();
	vec3 right = cross(front, up);
	return mat4(vec4(right, 0),
				vec4(up, 0),
				vec4(-front, 0),
				vec4(GetPosition(), 1));
}

vec3 Player::ViewPointToWorld(const vec3 &viewPoint) const {
	return vec3(LocalToWorldSpaceMatrix() * vec4(viewPoint + GetPosition(), 1));
}

float Player::GetWeight() {
	return 2.0f;
}