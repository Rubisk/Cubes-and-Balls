#include "WorldState.h"

using namespace std;
using namespace glm;

WorldState::WorldState() {
	player = make_shared<Player>();
	entities_.push_front(player);
	objects_.push_front(player);
}

void WorldState::AddEntity(shared_ptr<Entity> entity) {
	entities_.push_front(entity);
	objects_.push_front(entity);
}

void WorldState::RemoveEntity(shared_ptr<Entity> entity) {
	entities_.remove(entity);
	objects_.remove(entity);
}

void WorldState::AddObject(shared_ptr<Object> object) {
	objects_.push_front(object);
}

void WorldState::RemoveObject(shared_ptr<Object> object) {
	objects_.remove(object);
}

forward_list<shared_ptr<Entity>> WorldState::GetEntities() {
	return entities_;
}

forward_list<shared_ptr<Entity>> WorldState::GetEntitiesInBox(const vec3 &min, const vec3 &max) {
	forward_list<shared_ptr<Entity>> entitiesInBox;
	for (shared_ptr<Entity> e : entities_) {
		bool inBox = true;
		if (InBox_(min, max, e)) entitiesInBox.push_front(e);
	}
	return entitiesInBox;
}

forward_list<shared_ptr<Entity>> WorldState::GetEntitiesInSphere(const vec3 &center, float radius) {
	forward_list<shared_ptr<Entity>> entitiesInSphere;
	for (shared_ptr<Entity> e : entities_)
		if (InSphere_(center, radius, e)) entitiesInSphere.push_front(e);
	return entitiesInSphere;
}

forward_list<shared_ptr<Object>> WorldState::GetObjects() {
	return objects_;
}

forward_list<shared_ptr<Object>> WorldState::GetObjectsInBox(const vec3 &min, const vec3 &max) {
	forward_list<shared_ptr<Object>> objectsInSphere;
	for (shared_ptr<Object> o : objects_)
		if (InBox_(min, max, o)) objectsInSphere.push_front(o);
	return objectsInSphere;
}

forward_list<shared_ptr<Object>> WorldState::GetObjectsInSphere(const vec3 &center, float radius) {
	forward_list<shared_ptr<Object>> objectsInSphere;
	for (shared_ptr<Object> o : objects_)
		if (InSphere_(center, radius, o)) objectsInSphere.push_front(o);
	return objectsInSphere;
}

bool WorldState::InBox_(const glm::vec3 &min, const glm::vec3 &max, shared_ptr<Object> o) {
	vec3 pos = o->GetPosition();
	bool inBox = true;
	for (int i = 0; i < 3; i++) {
		float minc = (min[i] < max[i]) ? min[i] : max[i];
		float maxc = (min[i] > max[i]) ? min[i] : max[i];
		if (minc > pos[i] || pos[i] > maxc) {
			inBox = false;
			break;
		}
	}
	return inBox;
}

bool WorldState::InSphere_(const glm::vec3 &center, float radius, shared_ptr<Object> e) {
	return distance(e->GetPosition(), center) <= radius;
}
