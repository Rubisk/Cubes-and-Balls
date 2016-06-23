#include "World.h"

using namespace std;
using namespace glm;

void World::AddEntity(const shared_ptr<Entity> &entity) {
	entities_.push_front(entity);
}

void World::RemoveEntity(const shared_ptr<Entity> &entity) {
	entities_.remove(entity);
}

forward_list<shared_ptr<Entity>> World::GetEntities() {
	return entities_;
}

forward_list<shared_ptr<Entity>> World::GetEntitiesInBox(const vec3 &min, const vec3 &max) {
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

forward_list<shared_ptr<Entity>> World::GetEntitiesInSphere(const vec3 &center, float radius) {
	forward_list<shared_ptr<Entity>> entitiesInSphere;
	for (shared_ptr<Entity> e : entities_)
		if (distance(e->position, center) <= radius)
			entitiesInSphere.push_front(e);
	return entitiesInSphere;
}