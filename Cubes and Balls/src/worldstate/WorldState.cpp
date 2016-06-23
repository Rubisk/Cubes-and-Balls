#include "WorldState.h"

using namespace std;
using namespace glm;

void WorldState::AddEntity(const shared_ptr<Entity> &entity) {
	entities_.push_front(entity);
}

void WorldState::RemoveEntity(const shared_ptr<Entity> &entity) {
	entities_.remove(entity);
}

forward_list<shared_ptr<Entity>> WorldState::GetEntities() {
	return entities_;
}

forward_list<shared_ptr<Entity>> WorldState::GetEntitiesInBox(const vec3 &min, const vec3 &max) {
	forward_list<shared_ptr<Entity>> entitiesInBox;
	for (shared_ptr<Entity> e : entities_) {
		vec3 pos = e->position;
		bool inBox = true;
		for (int i = 0; i < 3; i++) {
			float minc = (min[i] < max[i]) ? min[i] : max[i];
			float maxc = (min[i] > max[i]) ? min[i] : max[i];
			if (minc > pos[i] || pos[i] > maxc) {
				inBox = false;
				break;
			}
		}
		if (inBox) entitiesInBox.push_front(e);
	}
	return entitiesInBox;
}

forward_list<shared_ptr<Entity>> WorldState::GetEntitiesInSphere(const vec3 &center, float radius) {
	forward_list<shared_ptr<Entity>> entitiesInSphere;
	for (shared_ptr<Entity> e : entities_)
		if (distance(e->position, center) <= radius)
			entitiesInSphere.push_front(e);
	return entitiesInSphere;
}