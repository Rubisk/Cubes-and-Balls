#include "Entity.h"

#include <mutex>

using namespace std;
using namespace glm;


mutex speedmtx;
mutex rotationAxismtx;
mutex rotationSpeedmtx;

float Entity::GetWeight() const {
	return 0.0f;
}

vec3 Entity::GetSpeed() const {
	speedmtx.lock();
	vec3 speed = speed_;
	speedmtx.unlock();
	return speed;
}

void Entity::SetSpeed(const vec3 &speed) {
	speedmtx.lock();
	speed_ = speed;
	speedmtx.unlock();
}

vec3 Entity::GetRotationAxis() const {
	rotationAxismtx.lock();
	vec3 rotationAxis = rotationAxis_;
	rotationAxismtx.unlock();
	return rotationAxis;
}

void Entity::SetRotationAxis(const vec3 & axis) {
	rotationAxismtx.lock();
	rotationAxis_ = axis;
	rotationAxismtx.unlock();
}

float Entity::GetRotationSpeed() const {
	rotationSpeedmtx.lock();
	float rotationSpeed = rotationSpeed_;
	rotationSpeedmtx.unlock();
	return rotationSpeed;
}

void Entity::SetRotationSpeed(float radians) {
	rotationSpeedmtx.lock();
	rotationSpeed_ = radians;
	rotationSpeedmtx.unlock();
}
