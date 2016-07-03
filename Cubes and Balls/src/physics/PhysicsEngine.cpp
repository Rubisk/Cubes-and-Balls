#include "PhysicsEngine.h"

#include <ctime>
#include <thread>

using namespace std;


void PhysicsEngine::Start(shared_ptr<WorldState> world, int updatesPerSecond) {
	world_ = world;
	this_thread::sleep_for(chrono::seconds(1));
	shouldStop_ = false;
	thread_ = new thread(PhysicsEngine::Loop_, this, updatesPerSecond);
	entityUpdater.SetWorldState(world);
	collisionDetector.SetWorldState(world);
}

void PhysicsEngine::Stop() {
	shouldStop_ = true;
	thread_->join();
	delete thread_;
}


void PhysicsEngine::Loop_(PhysicsEngine *e, int loopsPerSecond) {
	while (!e->shouldStop_) {
		auto start = chrono::high_resolution_clock::now();
		e->Tick_(1.0f / loopsPerSecond);
		auto end = chrono::high_resolution_clock::now();
		this_thread::sleep_for(chrono::milliseconds((long) 1000.0f / loopsPerSecond) - (end - start));
	}
}

void PhysicsEngine::Tick_(float timePassed) {
	for (shared_ptr<ForceGenerator> fg : forceGenerators_) {
		fg->GenerateForces(forceApplier, timePassed);
	}
	forceApplier.UpdateForces(timePassed);
	entityUpdater.UpdateEntities(timePassed);
}

void PhysicsEngine::RegisterForceGenerator(shared_ptr<ForceGenerator> generator) {
	forceGenerators_.push_back(generator);
}

void PhysicsEngine::UnRegisterForceGenerator(shared_ptr<ForceGenerator> generator) {
	forceGenerators_.remove(generator);
}

PhysicsEngine::~PhysicsEngine() {
}
