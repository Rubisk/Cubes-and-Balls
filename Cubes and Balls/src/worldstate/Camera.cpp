#include "Camera.h"

#include "glm/gtx/rotate_vector.hpp"

using namespace glm;

void Camera::LookAt(vec3 point) {
	vec3 facing = point - position_;
	orientation_.frontSide = normalize(facing);
}

void Camera::Rotate(vec3 axis, float degrees) {
	orientation_.frontSide = rotate(orientation_.frontSide, degrees, axis);
	orientation_.upSide = rotate(orientation_.upSide, degrees, axis);
}

void Camera::GoTo(vec3 position) {
	position_ = position;
}

void Camera::Move(vec3 distance) {
	position_ += distance;
}

vec3 Camera::GetPosition() {
	return position_;
}

Orientation Camera::GetOrientation() {
	return orientation_;
}

vec3 Camera::GetPointLookingAt(float distance) {
	return position_ + distance * orientation_.frontSide;
}
