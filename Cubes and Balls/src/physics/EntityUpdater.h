#ifndef CUBES_AND_BALLS_SRC_PHYSICS_ENTITYUPDATER_H_
#define CUBES_AND_BALLS_SRC_PHYSICS_ENTITYUPDATER_H_

#include <memory>

#include "../worldstate/WorldState.h"
#include "CollisionDetector.h"
#include "ForceApplier.h"


class EntityUpdater {
public:
	EntityUpdater(std::shared_ptr<ForceApplier> forceApplier);

	void SetWorldState(std::shared_ptr<WorldState> world);

	void UpdateEntities(float timePassed);
private:
	void UpdateEntity_(std::shared_ptr<Entity> e, float timePassed);

	void MoveEntities_(float timePassed);

	void MoveEntity_(std::shared_ptr<Entity> e, float timePassed);


	void ApplyForceAtCollision_(const Collision &c);

	CollisionDetector collisionDetector_;
	std::shared_ptr<ForceApplier> forceApplier_;
	std::shared_ptr<WorldState> world_;
};

#endif