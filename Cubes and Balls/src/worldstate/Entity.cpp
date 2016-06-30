#include "Entity.h"

#include <mutex>

using namespace std;
using namespace glm;


mutex positionmtx;
mutex speedmtx;
mutex rotationAxismtx;
mutex rotationSpeedmtx;

string Entity::GetModelName() {
	return "";
}

float Entity::GetWeight() {
	return 0.0f;
}

bool Entity::IsImmovable() {
	return false;
}

vec3 Entity::GetPosition() {
	positionmtx.lock();
	vec3 position = position_;
	positionmtx.unlock();
	return position;
}

void Entity::SetPosition(const vec3 &position) {
	positionmtx.lock();
	position_ = position;
	positionmtx.unlock();
}

vec3 Entity::GetSpeed() {
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

vec3 Entity::GetRotationAxis() {
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

float Entity::GetRotationSpeed() {
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
