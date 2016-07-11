#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_WORLDGRAVITY_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_WORLDGRAVITY_H_

#include "../physics/ForceGenerator.h"
#include "WorldState.h"

class WorldGravity :
	public ForceGenerator {
public:
	WorldGravity(std::shared_ptr<WorldState> world, const glm::vec3 &gravity);

	virtual void GenerateForces(std::shared_ptr<ForceApplier> forceApplier, float timePassed);
private:
	std::shared_ptr<WorldState> world_;
	glm::vec3 gravity_;
};

#endif