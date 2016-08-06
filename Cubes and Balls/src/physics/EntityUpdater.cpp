#include "EntityUpdater.h"

#include <iostream>

using namespace std;
using namespace glm;

namespace {

vec3 GetLocalSpeedAtCollision(shared_ptr<Entity> e, vec3 collisionPointLocal) {
	vec3 speed = e->GetRotationSpeed() * cross(e->GetRotationAxis(), collisionPointLocal) + e->GetSpeed();
	return speed;
}

} // namespace 

EntityUpdater::EntityUpdater(std::shared_ptr<ForceApplier> forceApplier) {
	forceApplier_ = forceApplier;
}

void EntityUpdater::SetWorldState(shared_ptr<WorldState> world) {
	world_ = world;
	collisionDetector_.SetWorldState(world);
}

void EntityUpdater::UpdateEntities(float timePassed) {
	MoveEntities_(timePassed);
	vector<Collision> collisions = collisionDetector_.LookForCollisions();
	while (!collisions.empty()) {
		MoveEntities_(-timePassed);
		for (const Collision &c : collisions)
			ApplyForceAtCollision_(c);
		collisions = collisionDetector_.LookForCollisions();
	}
}

void EntityUpdater::MoveEntities_(float timePassed) {
	for (shared_ptr<Entity> e : world_->GetEntities())
		MoveEntity_(e, timePassed);
}

// Updates entity position/rotation, simulating <timePassed> of movement.
void EntityUpdater::MoveEntity_(shared_ptr<Entity> e, float timePassed) {
	if (timePassed > 0) {
		e->SetSpeed(e->GetSpeed() * pow(0.9f, timePassed)); // TODO add friction / air resistance
		e->SetRotationSpeed(e->GetRotationSpeed()* pow(0.9f, timePassed)); // TODO add friction / air resistance
		e->SetPosition(e->GetSpeed() * timePassed + e->GetPosition());
		e->Rotate(e->GetRotationAxis(), e->GetRotationSpeed() * timePassed);
	}
	else {
		e->SetPosition(e->GetSpeed() * timePassed + e->GetPosition());
		e->Rotate(e->GetRotationAxis(), e->GetRotationSpeed() * timePassed);
		e->SetSpeed(e->GetSpeed() * pow(0.9f, timePassed)); // TODO add friction / air resistance
		e->SetRotationSpeed(e->GetRotationSpeed()* pow(0.9f, timePassed)); // TODO add friction / air resistance
	}
}

// Handles a collision. Assumes the 2 objects colliding are aligned, then applies forces so they will start moving as
// they should after their collision.
void EntityUpdater::ApplyForceAtCollision_(const Collision &c) {
	shared_ptr<Object> firstAsObject = c.first;
	shared_ptr<Object> secondAsObject = c.second;
	shared_ptr<Entity> firstAsEntity = dynamic_pointer_cast<Entity>(firstAsObject);
	shared_ptr<Entity> secondAsEntity = dynamic_pointer_cast<Entity>(secondAsObject);
	if (!(firstAsEntity || secondAsEntity)) {
		cout << "WARNING: Two objects are noted as \'colliding\', but they both can't move. Should not happen!"; // TODO setup logger.
	}
	else if (firstAsEntity && secondAsEntity) {
		float firstWeight = firstAsEntity->GetWeight();
		float secondWeight = secondAsEntity->GetWeight();
		mat4 transformFirst = c.first->LocalToWorldSpaceMatrix();
		mat4 transformSecond = c.second->LocalToWorldSpaceMatrix();
		vec3 positionLocalToFirst = vec3(inverse(transformFirst) * vec4(c.worldPosition, 1));
		vec3 positionLocalToSecond = vec3(inverse(transformSecond) * vec4(c.worldPosition, 1));

		// Lots of speed variables here. Just calculate the absolute speed of the 2 entities
		// at the point of impact first. Then subtract those to figure out how fast
		// entity 1 is crashing into entity second. We only care about the norm of this vector,
		// since the direction of acceleration depends on the impact direction at the model,
		// not the way the objects are colliding themselves (this is quite tricky, draw the picture!).

		vec3 firstImpactSpeed = vec3(transformFirst * vec4(GetLocalSpeedAtCollision(firstAsEntity, positionLocalToFirst), 0));
		vec3 secondImpactSpeed = vec3(transformSecond * vec4(GetLocalSpeedAtCollision(secondAsEntity, positionLocalToSecond), 0));
		vec3 speedDifference = firstImpactSpeed - secondImpactSpeed;

		vec3 firstForce = -length(speedDifference) * c.impact;
		vec3 secondForce = firstForce;
		forceApplier_->AddForce(firstAsEntity, firstForce, positionLocalToFirst, 0);
		forceApplier_->AddForce(secondAsEntity, secondForce, positionLocalToSecond, 0);
	}
	else {
		// Make "first" the entity and "second" the object"
		vec3 impact = c.impact;
		if (secondAsEntity) {
			firstAsEntity = secondAsEntity;
			impact *= -1;
		}

		// Just get the speed of impact and use all that speed to accelerate the entity
		// since the object is stable anyway.
		mat4 transformFirst = firstAsEntity->LocalToWorldSpaceMatrix();
		impact = mat3(inverse(transformFirst)) * impact;
		vec3 positionLocalToFirst = vec3(inverse(transformFirst) * vec4(c.worldPosition, 1));
		vec3 impactSpeed = vec3(transformFirst * vec4(GetLocalSpeedAtCollision(firstAsEntity, positionLocalToFirst), 0));
		vec3 force = -length(impactSpeed) * impact;

		// TODO create a proper "collisionlistener" interface
		// then use that to test if an entity has collided and therefor should not be affected by gravity.
		if (distance(normalize(force), vec3(0, 1, 0)) < 0.1f)
			firstAsEntity->downCollided = 1;
		forceApplier_->AddForceLocal(firstAsEntity, force, positionLocalToFirst, 0);
		// TODO add material speed absorption.
		if (length(firstAsEntity->GetSpeed()) < 0.01f) firstAsEntity->SetSpeed(vec3(0, 0, 0));
		if (firstAsEntity->GetRotationSpeed() < 0.01f) firstAsEntity->SetRotationSpeed(0);
		impactSpeed = vec3(transformFirst * vec4(GetLocalSpeedAtCollision(firstAsEntity, positionLocalToFirst), 0));
	}
}
