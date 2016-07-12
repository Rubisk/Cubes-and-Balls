#include "PhysicsEngine.h"

#include <ctime>
#include <thread>
#include <iostream>

#include "glm/glm.hpp"

using namespace std;
using namespace glm;
using namespace chrono;

PhysicsEngine::PhysicsEngine() {
	forceApplier = make_shared<ForceApplier>();
	entityUpdater_ = make_shared<EntityUpdater>(forceApplier);
}

void PhysicsEngine::Start(shared_ptr<WorldState> world, int updatesPerSecond) {
	world_ = world;
	shouldStop_ = false;
	entityUpdater_->SetWorldState(world);
	thread_ = new thread(PhysicsEngine::Loop_, this, updatesPerSecond);
}

void PhysicsEngine::Stop() {
	shouldStop_ = true;
	thread_->join();
	delete thread_;
}


void PhysicsEngine::Loop_(PhysicsEngine *e, int loopsPerSecond) {
	while (!e->shouldStop_) {
		auto start = high_resolution_clock::now();
		e->Tick_(1.0f / loopsPerSecond);
		auto end = high_resolution_clock::now();
		auto timeLeft = milliseconds((long) 1000.0 / loopsPerSecond) - (end - start);

		milliseconds timeLeftAsMS = duration_cast<milliseconds>(timeLeft);
		if (timeLeftAsMS.count() < 0) {
			cout << "Physics tick took " << -timeLeftAsMS.count() << " ms too long.";
		}

		this_thread::sleep_for(timeLeft);
	}
}

void PhysicsEngine::Tick_(float timePassed) {
	for (shared_ptr<ForceGenerator> fg : forceGenerators_) {
		fg->GenerateForces(forceApplier, timePassed);
	}
	forceApplier->UpdateForces(timePassed);
	entityUpdater_->UpdateEntities(timePassed);
}

void PhysicsEngine::RegisterForceGenerator(shared_ptr<ForceGenerator> generator) {
	forceGenerators_.push_back(generator);
}

void PhysicsEngine::UnRegisterForceGenerator(shared_ptr<ForceGenerator> generator) {
	forceGenerators_.remove(generator);
}

PhysicsEngine::~PhysicsEngine() {}
