#ifndef CUBES_AND_BALLS_SRC_PHYSICS_PHYSICSENGINE_H_
#define CUBES_AND_BALLS_SRC_PHYSICS_PHYSICSENGINE_H_

#include <thread>

#include "../worldstate/WorldState.h"
#include "EntityUpdater.h"
#include "ForceApplier.h"
#include "ForceGenerator.h"
#include "CollisionDetector.h"


class PhysicsEngine {
public:
	void Start(std::shared_ptr<WorldState> world_, int updatesPerSecond);

	void Stop();

	void RegisterForceGenerator(std::shared_ptr<ForceGenerator> generator);

	void UnRegisterForceGenerator(std::shared_ptr<ForceGenerator> generator);

	~PhysicsEngine();

	EntityUpdater entityUpdater;
	ForceApplier forceApplier;
private:
	static void Loop_(PhysicsEngine *e, int loopsPerSecond);

	void Tick_(float timePassed);

	volatile bool shouldStop_;
	std::thread* thread_;

	std::shared_ptr<WorldState> world_;
	std::list<std::shared_ptr<ForceGenerator>> forceGenerators_;
};

#endif