#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_WORLDSTATE_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_WORLDSTATE_H_

#include <forward_list>
#include <memory>

#include "Entity.h"
#include "Player.h"

// Main object used to provide a general interface
// for anything that is part of the world our game is simulating.
class WorldState {
public:
	WorldState();

	std::shared_ptr<Player> player;

	void AddEntity(std::shared_ptr<Entity> entity);

	void RemoveEntity(std::shared_ptr<Entity> entity);

	void AddObject(std::shared_ptr<Object> object);

	void RemoveObject(std::shared_ptr<Object> object);

	std::forward_list<std::shared_ptr<Entity>> GetEntities();

	std::forward_list<std::shared_ptr<Entity>> GetEntitiesInBox(const glm::vec3 &min, const glm::vec3 &max);

	std::forward_list<std::shared_ptr<Entity>> GetEntitiesInSphere(const glm::vec3 &center, float radius);

	std::forward_list<std::shared_ptr<Object>> GetObjects();

	std::forward_list<std::shared_ptr<Object>> GetObjectsInBox(const glm::vec3 &min, const glm::vec3 &max);

	std::forward_list<std::shared_ptr<Object>> GetObjectsInSphere(const glm::vec3 &center, float radius);
private:
	bool InBox_(const glm::vec3 &min, const glm::vec3 &max, std::shared_ptr<Object> toTest);

	bool InSphere_(const glm::vec3 &center, float radius, std::shared_ptr<Object> toTest);

	std::forward_list<std::shared_ptr<Entity>> entities_;

	std::forward_list<std::shared_ptr<Object>> objects_;
};

#endif