#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"


using namespace glm;

// This file assumes the two vectors in orientation_ are always orthogonal.
// Upon changing them, please KEEP THIS PROPERTY.

void Camera::LookAt(const vec3 &point) {
	vec3 facing = point - position_;
	orientation_.frontSide = normalize(facing);

	// Up must be orthogonal to facing. If they're in the same 1-dimensional subspace, we 
	// reset up to either (0, 1, 0) or (0, 0, 1).
	if (distance(normalize(orientation_.frontSide), normalize(orientation_.upSide)) < 0.00001 ||
		distance(normalize(orientation_.frontSide), -normalize(orientation_.upSide)) < 0.00001) {
		orientation_.upSide = (-0.5 < orientation_.frontSide.y && orientation_.frontSide.y < 0.5)
			? vec3(0, 1, 0) : vec3(1, 0, 0);
	}
	else {
		orientation_.upSide = normalize(orientation_.upSide - dot(orientation_.upSide, orientation_.frontSide) * orientation_.frontSide);
	}
}

void Camera::Rotate(const vec3 &axis, float radians) {
	Rotate(axis, radians, false);
}

#include <iostream>
#include "glm/gtx/io.hpp"
void Camera::Rotate(const vec3 &axis, float radians, bool worldCoords) {
	if (worldCoords) {
		orientation_.frontSide = rotate(orientation_.frontSide, radians, axis);
		orientation_.upSide = rotate(orientation_.upSide, radians, axis);
	}
	else {
		vec3 worldAxis = vec3(GetViewToWorldMatrix() * vec4(axis, 0));
		orientation_.frontSide = rotate(orientation_.frontSide, radians, worldAxis);
		orientation_.upSide = rotate(orientation_.upSide, radians, worldAxis);
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

vec3 Camera::GetPosition() {
	return position_;
}

Orientation Camera::GetOrientation() {
	return orientation_;
}

vec3 Camera::GetPointLookingAt(float distance) {
	return position_ + distance * orientation_.frontSide;
}

mat4 Camera::GetViewToWorldMatrix() {
	vec3 up = orientation_.upSide;
	vec3 front = orientation_.frontSide;
	vec3 right = cross(front, up);
	return mat4(vec4(right, 0),
				vec4(up, 0),
				vec4(-front, 0),
				vec4(position_, 1));
}

vec3 Camera::ViewPointToWorld(const vec3 &viewPoint) {
	return vec3(GetViewToWorldMatrix() * vec4(viewPoint + position_, 1));
}