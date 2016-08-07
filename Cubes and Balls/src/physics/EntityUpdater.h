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
	void MoveEntities_(float timePassed);

	void MoveEntity_(std::shared_ptr<Entity> e, float timePassed);

	void ApplyForceAtCollision_(const Collision &c);

	void ApplyForceAtObjectEntityCollision_(std::shared_ptr<Object> object, std::shared_ptr<Entity> entity,
											const glm::vec3 &positionOfCollision, const glm::vec3 &collisionImpact);

	void ApplyForceAtEntityEntityCollision_(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second,
											const glm::vec3 &positionOfCollision, const glm::vec3 &collisionImpact);

	CollisionDetector collisionDetector_;
	std::shared_ptr<ForceApplier> forceApplier_;
	std::shared_ptr<WorldState> world_;
};

#endif