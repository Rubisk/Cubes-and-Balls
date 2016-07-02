#ifndef CUBES_AND_BALLS_SRC_PHYSICS_PHYSICSENGINE_H_
#define CUBES_AND_BALLS_SRC_PHYSICS_PHYSICSENGINE_H_

#include <thread>

#include "../worldstate/WorldState.h"
#include "EntityUpdater.h"

class PhysicsEngine {
public:
	void Start(const std::weak_ptr<WorldState> &world_, int updatesPerSecond);

	void Stop();

	~PhysicsEngine();
private:
	static void Loop_(PhysicsEngine *e, int loopsPerSecond);

	void Tick_(float timePassed);

	volatile bool shouldStop_;
	std::thread* thread_;

	std::weak_ptr<WorldState> world_;
	EntityUpdater entityUpdater;
};

#endif