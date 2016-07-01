#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_WORLD_STATE_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_WORLD_STATE_H_

#include <forward_list>
#include <memory>

#include "Entity.h"
#include "Camera.h"

// Main object used to provide a general interface
// for anything that is part of the world our game is simulating.
class WorldState {
public:
	Camera camera;

	void AddEntity(const std::shared_ptr<Entity> &entity);

	void RemoveEntity(const std::shared_ptr<Entity> &entity);

	void AddObject(const std::shared_ptr<Object> &object);

	void RemoveObject(const std::shared_ptr<Object> &object);

	std::forward_list<std::shared_ptr<Entity>> GetEntities();

	std::forward_list<std::shared_ptr<Entity>> GetEntitiesInBox(const glm::vec3 &min, const glm::vec3 &max);

	std::forward_list<std::shared_ptr<Entity>> GetEntitiesInSphere(const glm::vec3 &center, float radius);

	std::forward_list<std::shared_ptr<Object>> GetObjects();

	std::forward_list<std::shared_ptr<Object>> GetObjectsInBox(const glm::vec3 &min, const glm::vec3 &max);

	std::forward_list<std::shared_ptr<Object>> GetObjectsInSphere(const glm::vec3 &center, float radius);
private:
	bool InBox_(const glm::vec3 &min, const glm::vec3 &max, const Object &toTest);

	bool InSphere_(const glm::vec3 &center, float radius, const Object &toTest);

	std::forward_list<std::shared_ptr<Entity>> entities_;

	std::forward_list<std::shared_ptr<Object>> objects_;
};

#endif