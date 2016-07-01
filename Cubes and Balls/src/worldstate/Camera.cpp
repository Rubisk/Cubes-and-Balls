#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"


using namespace glm;

// This file assumes the two vectors in orientation_ are always orthogonal.
// Upon changing them, please KEEP THIS PROPERTY.

void Camera::LookAt(const vec3 &point) {
	vec3 facing = point - position_;
	orientation_.SetFrontSide(facing);
}

void Camera::Rotate(const vec3 &axis, float radians) {
	Rotate(axis, radians, false);
}

void Camera::Rotate(const vec3 &axis, float radians, bool worldCoords) {
	if (worldCoords) {
		orientation_.Rotate(axis, radians);
	}
	else {
		vec3 worldAxis = vec3(GetViewToWorldMatrix() * vec4(axis, 0));
		orientation_.Rotate(worldAxis, radians);
	}
}


void Camera::GoTo(const vec3 &position) {
	position_ = position;
}

void Camera::Move(const vec3 &distance) {
	Move(distance, false);
}

void Camera::Move(const vec3 &distance, bool worldCoords) {
	if (worldCoords) {
		position_ += distance;
	}
	else {
		position_ += vec3(GetViewToWorldMatrix() * vec4(distance, 0));
	}
}

vec3 Camera::GetPosition() const {
	return position_;
}

Orientation Camera::GetOrientation() const {
	return orientation_;
}

vec3 Camera::GetPointLookingAt(float distance) const {
	return position_ + distance * orientation_.GetFrontSide();
}

mat4 Camera::GetViewToWorldMatrix() const {
	vec3 up = orientation_.GetUpSide();
	vec3 front = orientation_.GetFrontSide();
	vec3 right = cross(front, up);
	return mat4(vec4(right, 0),
				vec4(up, 0),
				vec4(-front, 0),
				vec4(position_, 1));
}

vec3 Camera::ViewPointToWorld(const vec3 &viewPoint) const {
	return vec3(GetViewToWorldMatrix() * vec4(viewPoint + position_, 1));
}