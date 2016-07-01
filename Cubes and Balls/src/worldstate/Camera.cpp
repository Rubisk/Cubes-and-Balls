#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"


using namespace glm;

void Camera::LookAt(const vec3 &point) {
	vec3 facing = point - GetPosition();
	SetFrontSide(facing);
}

void Camera::Rotate(const vec3 &axis, float radians) {
	Rotate(axis, radians, false);
}

void Camera::Rotate(const vec3 &axis, float radians, bool worldCoords) {
	vec3 worldAxis = (worldCoords) ? axis : vec3(GetViewToWorldMatrix() * vec4(axis, 0));
	Object::Rotate(worldAxis, radians);
}

void Camera::Move(const vec3 &distance) {
	Move(distance, false);
}

void Camera::Move(const vec3 &distance, bool worldCoords) {
	vec3 worldDistance = (worldCoords) ? distance : vec3(GetViewToWorldMatrix() * vec4(distance, 0));
	Object::Move(worldDistance);
}

vec3 Camera::GetPointLookingAt(float distance) const {
	return GetPosition() + distance * GetFrontSide();
}

mat4 Camera::GetViewToWorldMatrix() const {
	vec3 up = GetUpSide();
	vec3 front = GetFrontSide();
	vec3 right = cross(front, up);
	return mat4(vec4(right, 0),
				vec4(up, 0),
				vec4(-front, 0),
				vec4(GetPosition(), 1));
}

vec3 Camera::ViewPointToWorld(const vec3 &viewPoint) const {
	return vec3(GetViewToWorldMatrix() * vec4(viewPoint + GetPosition(), 1));
}