#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_WORLD_STATE_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_WORLD_STATE_H_

#include <forward_list>
#include <memory>

#include "Entity.h"

// Main object used to provide a general interface
// for anything that is part of the world our game is simulating.
class WorldState {
public:
	void AddEntity(const std::shared_ptr<Entity> &entity);

	void RemoveEntity(const std::shared_ptr<Entity> &entity);

	std::forward_list<std::shared_ptr<Entity>> GetEntities();

	std::forward_list<std::shared_ptr<Entity>> GetEntitiesInBox(const glm::vec3 &min, const glm::vec3 &max);

	std::forward_list<std::shared_ptr<Entity>> GetEntitiesInSphere(const glm::vec3 &center, float radius);
private:
	std::forward_list<std::shared_ptr<Entity>> entities_;
};

#endif