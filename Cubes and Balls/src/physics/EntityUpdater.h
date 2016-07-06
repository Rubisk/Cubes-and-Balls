#ifndef CUBES_AND_BALLS_SRC_PHYSICS_ENTITYUPDATER_H_
#define CUBES_AND_BALLS_SRC_PHYSICS_ENTITYUPDATER_H_

#include <memory>

#include "../worldstate/WorldState.h"
#include "CollisionDetector.h"


class EntityUpdater {
public:
	void SetWorldState(std::shared_ptr<WorldState> world);

	void UpdateEntities(float timePassed);
private:
	void UpdateEntity_(std::shared_ptr<Entity> e, float timePassed);

	void MoveEntity_(std::shared_ptr<Entity> e, float timePassed);

	float EntityUpdater::MoveEntityToCollisionTime_(std::shared_ptr<Entity> e, float startTime, float endTime, Collision &outputCollision);

	void ApplyForceAtCollision_(const Collision &c);

	CollisionDetector collisionDetector_;
	std::shared_ptr<WorldState> world_;
};

#endif