#include "CollisionDetector.h"

using namespace std;
using namespace glm;

namespace {

struct Box {
	glm::vec3 min;
	glm::vec3 max;
};

void WrapBox(shared_ptr<Object> target, Box &outputBox, int axis) {
	shared_ptr<const Model> model = target->GetModel();
}

}

void CollisionDetector::SetWorldState(shared_ptr<WorldState> world) {
	world_ = world;
}

bool CollisionDetector::IsColliding(shared_ptr<Object> toTest, vec3 &outputCenterOfCollision, shared_ptr<Object> &outputCollider) {
	if (world_ == nullptr) return false;
	vec3 centerOfCollision;
	for (shared_ptr<Object> object : world_->GetObjects()) {
		if (Colliding(object, toTest, centerOfCollision)) {
			outputCollider = object;
			outputCenterOfCollision = centerOfCollision;
			return true;
		}
	}
	return false;
}

bool CollisionDetector::Colliding(shared_ptr<Object> first, shared_ptr<Object> second, vec3 &outputCenterOfCollision) {
	Box box;
	box.min = vec3(-10, -10, -10); // TODO setup proper min/maxing for object size.
	box.max = vec3(10, 10, 10);
	while (box.min.x < box.max.x && box.min.y < box.max.y && box.min.z < box.max.z) {
		Box newBox = box;
		for (int i = 0; i < 3; i++)
			WrapBox(first, newBox, i);
		for (int i = 0; i < 3; i++)
			WrapBox(second, newBox, i);
		if (newBox.min == box.min && newBox.max == box.max) {
			outputCenterOfCollision = 0.5f * (box.min + box.max);
			return true;
		}
	}
	return false;
}
