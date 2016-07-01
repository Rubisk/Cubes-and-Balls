#ifndef CUBES_AND_BALLS_SRC_PHYSICS_PHYSICSENGINE_H_
#define CUBES_AND_BALLS_SRC_PHYSICS_PHYSICSENGINE_H_

#include "../worldstate/WorldState.h"

class PhysicsEngine {
public:
	void Start(std::shared_ptr<WorldState> &world);
	~PhysicsEngine();
};

#endif