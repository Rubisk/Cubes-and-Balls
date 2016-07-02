#ifndef CUBES_AND_BALLS_SRC_PHYSICS_ENTITYUPDATER_H_
#define CUBES_AND_BALLS_SRC_PHYSICS_ENTITYUPDATER_H_

#include <memory>

#include "../worldstate/WorldState.h"


class EntityUpdater {
public:
	void SetWorldState(const std::weak_ptr<WorldState> &world);

	void UpdateEntities(float timePassed);
private:
	void UpdateEntity_(std::shared_ptr<Entity> &e, float timePassed);

	std::weak_ptr<WorldState> world_;
};

#endif