#include "WorldGravity.h"

using namespace std;
using namespace glm;

WorldGravity::WorldGravity(shared_ptr<WorldState> world, const vec3 &gravity) {
	world_ = world;
	gravity_ = gravity;
}

void WorldGravity::GenerateForces(shared_ptr<ForceApplier> forceApplier, float timePassed) {
	for (shared_ptr<Entity> e : world_->GetEntities()) {
		if (dynamic_pointer_cast<Player>(e)) continue;
		forceApplier->AddForce(e, e->GetWeight() * gravity_, e->ToWorldSpace(vec3(0, 0, 0), 1), timePassed);
	}
}
