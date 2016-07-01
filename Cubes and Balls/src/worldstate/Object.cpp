#include "Object.h"

#include <mutex>


using namespace std;
using namespace glm;

mutex positionmtx;

string Object::GetModelName() {
	return "";
}

glm::vec3 Object::GetModelScale() {
	return glm::vec3(1, 1, 1);
}

vec3 Object::GetPosition() const {
	positionmtx.lock();
	vec3 position = position_;
	positionmtx.unlock();
	return position;
}

void Object::SetPosition(const vec3 &position) {
	positionmtx.lock();
	position_ = position;
	positionmtx.unlock();
}

void Object::Move(const glm::vec3 & distance) {
	SetPosition(GetPosition() + distance);
}

Object::~Object() {
}

