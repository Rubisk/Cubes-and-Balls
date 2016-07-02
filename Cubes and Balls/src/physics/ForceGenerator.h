#ifndef CUBES_AND_BALLS_SRC_FORCEGENERATOR_H_
#define CUBES_AND_BALLS_SRC_FORCEGENERATOR_H_

#include "ForceApplier.h"

// A ForceGenerator can be used to provide an interface between the worldState physics
// and the game logic. Register these to the physics engine,
// and it will call their GenerateForces function every time before
// applying forces.
class ForceGenerator {
public:
	virtual void GenerateForces(shared_ptr<ForceApplier> forceApplier, float timePassed) = 0;

	virtual ~ForceGenerator() {};
};

#endif