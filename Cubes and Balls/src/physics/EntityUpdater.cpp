#include "EntityUpdater.h"

#include <iostream>

using namespace std;
using namespace glm;

// Time working precision for collisions.
const float collisionTimePrecision = 0.01f;

namespace {

vec3 GetLocalSpeedAtCollision(shared_ptr<Entity> e, vec3 collisionPointLocal, vec3 collisionDirectionLocal) {
	vec3 speed = cross(e->GetRotationSpeed() * e->GetRotationAxis(), collisionPointLocal) + e->GetSpeed();
	return dot(speed, collisionDirectionLocal) * collisionDirectionLocal;
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
	for (shared_ptr<Entity> e : world_->GetEntities()) {
		UpdateEntity_(e, timePassed);
	}
}

void EntityUpdater::UpdateEntity_(shared_ptr<Entity> e, float timePassed) {
	vec3 speed = e->GetSpeed();
	vec3 position = e->GetPosition();
	Collision outputCollision;

	float totalUpdateTime = 0;
	float updateTimeStep;
	if (e->GetModel()->minRadius && length(speed) > 0)
		updateTimeStep = e->GetModel()->minRadius / length(speed);
	else
		updateTimeStep = timePassed;
	while (totalUpdateTime < timePassed) {
		if (totalUpdateTime + updateTimeStep > timePassed) updateTimeStep = timePassed - totalUpdateTime;
		totalUpdateTime += updateTimeStep;
		MoveEntity_(e, updateTimeStep);
		if (collisionDetector_.IsCollidingQ(e, outputCollision)) {
			MoveEntity_(e, -updateTimeStep);
			float timeUntilCollision = MoveEntityToCollisionTime_(e, totalUpdateTime - updateTimeStep, totalUpdateTime, outputCollision);
			ApplyForceAtCollision_(outputCollision);
			totalUpdateTime += timeUntilCollision;
		}
	}
}

// Updates entity position/rotation, simulating <timePassed> of movement.
void EntityUpdater::MoveEntity_(shared_ptr<Entity> e, float timePassed) {
	e->SetPosition(e->GetSpeed() * timePassed + e->GetPosition());
	e->SetSpeed(e->GetSpeed()); // TODO add friction / air resistance
	e->SetRotationSpeed(e->GetRotationSpeed()); // TODO add friction / air resistance
	e->Rotate(e->GetRotationAxis(), e->GetRotationSpeed() * timePassed);
}

// Moves and entity to the point where it starts colliding with another object.
// Returns the time at which this happens (in seconds since the last physics tick).
float EntityUpdater::MoveEntityToCollisionTime_(shared_ptr<Entity> e, float startTime, float endTime, Collision &outputCollision) {
	while (abs(startTime - endTime) > collisionTimePrecision) {
		// Binary search the correct time point.
		float middleTime = (startTime + endTime) / 2;
		MoveEntity_(e, middleTime - startTime);
		if (collisionDetector_.IsCollidingQ(e, outputCollision)) {
			endTime = middleTime;
			MoveEntity_(e, startTime - middleTime);
		}
		else {
			startTime = middleTime;
		}
	}
	return startTime;
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

		vec3 firstImpactSpeed = vec3(transformFirst * vec4(
			GetLocalSpeedAtCollision(firstAsEntity,
									 positionLocalToFirst,
									 c.impactDirectionAtFirst), 0)
		);
		vec3 secondImpactSpeed = vec3(transformSecond * vec4(
			GetLocalSpeedAtCollision(secondAsEntity,
									 positionLocalToSecond,
									 c.impactDirectionAtSecond), 0)
		);
		vec3 speedDifference = firstImpactSpeed - secondImpactSpeed;

		float impactSpeed = length(speedDifference);
		float impact = impactSpeed;
		forceApplier_->AddForce(firstAsEntity, impact * c.impactDirectionAtFirst, positionLocalToFirst, 0);
		forceApplier_->AddForce(secondAsEntity, impact * c.impactDirectionAtSecond, positionLocalToSecond, 0);
	}
	else {
		// Make "first" the entity and "second" the object"
		vec3 impactDirection;
		if (secondAsEntity) {
			firstAsEntity = secondAsEntity;
			impactDirection = c.impactDirectionAtSecond;
		}
		else {
			impactDirection = c.impactDirectionAtFirst;
		}

		// Just get the speed of impact and use all that speed to accelerate the entity
		// since the object is stable anyway.
		mat4 transformFirst = firstAsEntity->LocalToWorldSpaceMatrix();
		vec3 positionLocalToFirst = vec3(inverse(transformFirst) * vec4(c.worldPosition, 1));
		vec3 impactSpeed = vec3(transformFirst * vec4(
			GetLocalSpeedAtCollision(firstAsEntity,
									 positionLocalToFirst,
									 impactDirection), 0)
		);
		float impact = length(impactSpeed) * (firstAsEntity->GetWeight());
		forceApplier_->AddForce(firstAsEntity, impact * impactDirection, positionLocalToFirst, 0);
	}
}
