#include "Object.h"

#include <mutex>


using namespace std;
using namespace glm;

mutex positionmtx;

string Object::GetModelName() const {
	return "";
}

glm::vec3 Object::GetModelScale() const {
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

mat4 Object::LocalToWorldSpaceMatrix() const {
	vec3 up = GetUpSide();
	vec3 front = GetFrontSide();
	vec3 right = cross(front, up);
	return mat4(vec4(right, 0),
				vec4(up, 0),
				vec4(-front, 0),
				vec4(GetPosition(), 1));
}


Object::~Object() {
}

