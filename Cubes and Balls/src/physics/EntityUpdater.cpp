#include "EntityUpdater.h"

using namespace std;

void EntityUpdater::SetWorldState(const weak_ptr<WorldState> &world) {
	world_ = world;
}

void EntityUpdater::UpdateEntities(float timePassed) {
	if (world_.expired()) return;
	shared_ptr<WorldState> world = world_.lock();
	for (shared_ptr<Entity> e : world->GetEntities()) {
		UpdateEntity_(e, timePassed);
	}
}

void EntityUpdater::UpdateEntity_(shared_ptr<Entity> &e, float timePassed) {
	e->SetPosition(e->GetSpeed() * timePassed + e->GetPosition());
	e->SetSpeed(e->GetSpeed() * pow(0.003f, timePassed)); // TODO add friction / air resistance
	e->SetRotationSpeed(e->GetRotationSpeed() * pow(0.001f, timePassed)); // TODO add friction / air resistance
	e->Rotate(e->GetRotationAxis(), e->GetRotationSpeed());
}
