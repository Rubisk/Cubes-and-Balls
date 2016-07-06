#include "Entity.h"

using namespace std;
using namespace glm;

float Entity::GetWeight() const {
	return 0.0f;
}

vec3 Entity::GetSpeed() const {
	speedmtx_.lock();
	vec3 speed = speed_;
	speedmtx_.unlock();
	return speed;
}

void Entity::SetSpeed(const vec3 &speed) {
	speedmtx_.lock();
	speed_ = speed;
	speedmtx_.unlock();
}

vec3 Entity::GetRotationAxis() const {
	rotationAxismtx_.lock();
	vec3 rotationAxis = rotationAxis_;
	rotationAxismtx_.unlock();
	return rotationAxis;
}

void Entity::SetRotationAxis(const vec3 & axis) {
	rotationAxismtx_.lock();
	rotationAxis_ = axis;
	rotationAxismtx_.unlock();
}

float Entity::GetRotationSpeed() const {
	rotationSpeedmtx_.lock();
	float rotationSpeed = rotationSpeed_;
	rotationSpeedmtx_.unlock();
	return rotationSpeed;
}

void Entity::SetRotationSpeed(float radians) {
	rotationSpeedmtx_.lock();
	rotationSpeed_ = radians;
	rotationSpeedmtx_.unlock();
}
