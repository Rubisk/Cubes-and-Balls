#include "EntityUpdater.h"

#include <iostream>

using namespace std;
using namespace glm;

EntityUpdater::EntityUpdater(std::shared_ptr<ForceApplier> forceApplier) {
	forceApplier_ = forceApplier;
}

void EntityUpdater::SetWorldState(shared_ptr<WorldState> world) {
	world_ = world;
	collisionDetector_.SetWorldState(world);
}

void EntityUpdater::UpdateEntities(float timePassed) {
	assert(collisionDetector_.LookForCollisions().empty());
	MoveEntities_(timePassed);
	vector<Collision> collisions = collisionDetector_.LookForCollisions();
	while (!collisions.empty()) {
		MoveEntities_(-timePassed);
		assert(collisionDetector_.LookForCollisions().empty());
		for (const Collision &c : collisions)
			ApplyForceAtCollision_(c);
		MoveEntities_(timePassed);
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
		e->SetRotationSpeed(e->GetRotationSpeed() * pow(0.9f, timePassed)); // TODO add friction / air resistance
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
		ApplyForceAtEntityEntityCollision_(firstAsEntity, secondAsEntity, c.worldPosition, c.impact);
	}
	else {
		if (secondAsEntity)
			ApplyForceAtObjectEntityCollision_(firstAsObject, secondAsEntity, c.worldPosition, -c.impact);
		else
			ApplyForceAtObjectEntityCollision_(secondAsObject, firstAsEntity, c.worldPosition, c.impact);
	}
}

void EntityUpdater::ApplyForceAtObjectEntityCollision_(shared_ptr<Object> object, shared_ptr<Entity> entity,
													   const vec3 &positionOfCollision, const vec3 &collisionImpact) {
	vec3 localImpact = entity->ToLocalSpace(collisionImpact, 0);
	vec3 localPosition = positionOfCollision - entity->GetPosition();
	vec3 speed = entity->GetRotationSpeed() * cross(entity->GetRotationAxis(), localPosition) + entity->GetSpeed();
	vec3 force = -1.0f * length(speed) * collisionImpact; // TODO add object material bouncyness

	forceApplier_->AddForce(entity, force, positionOfCollision, 0);

	entity->SetSpeed(entity->GetSpeed() * 0.9f); // TODO add resistances
	entity->SetRotationSpeed(entity->GetRotationSpeed() * 0.9f); // TODO add resistances

}

void EntityUpdater::ApplyForceAtEntityEntityCollision_(shared_ptr<Entity> first, shared_ptr<Entity> second,
													   const vec3 &positionOfCollision, const vec3 &collisionImpact) {
	float firstWeight = first->GetWeight();
	float secondWeight = second->GetWeight();
	mat4 transformFirst = first->LocalToWorldSpaceMatrix();
	mat4 transformSecond = second->LocalToWorldSpaceMatrix();
	vec3 positionLocalToFirst = positionOfCollision - first->GetPosition();
	vec3 positionLocalToSecond = positionOfCollision - second->GetPosition();

	// Lots of speed variables here. Just calculate the absolute speed of the 2 entities
	// at the point of localImpact first. Then subtract those to figure out how fast
	// entity 1 is crashing into entity second. We only care about the norm of this vector,
	// since the direction of acceleration depends on the localImpact direction at the model,
	// not the way the objects are colliding themselves (this is quite tricky, draw the picture!).
	vec3 firstImpactSpeed = first->GetRotationSpeed() * cross(first->GetRotationAxis(), positionLocalToFirst) + first->GetSpeed();
	vec3 secondImpactSpeed = second->GetRotationSpeed() * cross(second->GetRotationAxis(), positionLocalToSecond) + second->GetSpeed();
	vec3 speedDifference = firstImpactSpeed - secondImpactSpeed;

	vec3 firstForce = length(speedDifference) * -collisionImpact;
	vec3 secondForce = length(speedDifference) * collisionImpact;
	forceApplier_->AddForce(first, firstForce, positionLocalToFirst, 0);
	forceApplier_->AddForce(second, secondForce, positionLocalToSecond, 0);


	first->SetSpeed(first->GetSpeed() * 0.9f); // TODO add resistances
	first->SetRotationSpeed(first->GetRotationSpeed() * 0.9f); // TODO add resistances
	second->SetSpeed(second->GetSpeed() * 0.9f); // TODO add resistances
	second->SetRotationSpeed(second->GetRotationSpeed() * 0.9f); // TODO add resistances
}