#include "EntityUpdater.h"

#include <iostream>

using namespace std;
using namespace glm;

// Time working precision for collisions.
const float collisionTimePrecision = 0.00000000001f;

namespace {

vec3 GetLocalSpeedAtCollision(shared_ptr<Entity> e, vec3 collisionPointLocal, vec3 collisionDirectionLocal) {
	vec3 speed = cross(e->GetRotationSpeed() * e->GetRotationAxis(), collisionPointLocal) + e->GetSpeed();
	return dot(speed, collisionDirectionLocal) * collisionDirectionLocal;
}

// Accelerates a point in local entity space with a certain speed. Local position is obviously in MODEL SPACE (!!!!)
void AccelerateEntityAtPoint(shared_ptr<Entity> e, vec3 speedToAdd, vec3 localPosition) {
	// TODO this code is mainly copied from forceApplier stuff, figure out a way
	// to reuse this code properly.

	// Split the force in a force that rotates the object, and one that moves the center.
	// (I suggest sketching the picture of a sphere)
	vec3 rotationForce;
	if (length(localPosition) == 0) rotationForce = vec3(0, 0, 0);
	else rotationForce = speedToAdd - dot(speedToAdd, localPosition) * speedToAdd; // Gram-Schmidt
	vec3 centerForce = speedToAdd - rotationForce;

	float weight = e->GetWeight();

	// Update center movement
	vec3 speed = e->GetSpeed();
	speed += centerForce;
	e->SetSpeed(speed);

	// Update rotation momentum
	vec3 oldRotationAxis = e->GetRotationAxis();
	vec3 newRotationAxis;
	if (length(localPosition) == 0) newRotationAxis = vec3(0, 0, 0);
	else newRotationAxis = normalize(cross(localPosition, rotationForce));

	float oldRotationSpeed = e->GetRotationSpeed();
	float newRotationSpeed;
	if (length(localPosition) == 0) newRotationSpeed = 0;
	else newRotationSpeed = length(rotationForce) * length(localPosition);

	vec3 totalRotationAxis = (oldRotationSpeed * oldRotationAxis + newRotationSpeed * newRotationAxis);
	totalRotationAxis = (length(totalRotationAxis) == 0) ? totalRotationAxis : normalize(totalRotationAxis);

	e->SetRotationAxis(totalRotationAxis);
	e->SetRotationSpeed(dot(oldRotationAxis, totalRotationAxis) * oldRotationSpeed + dot(newRotationAxis, totalRotationAxis) * newRotationSpeed);
}

} // namespace 

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
	float updateTimeStep = e->GetModel()->minRadius / length(speed);
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
	e->SetSpeed(e->GetSpeed() * pow(0.003f, timePassed)); // TODO add friction / air resistance
	e->SetRotationSpeed(e->GetRotationSpeed() * pow(0.001f, timePassed)); // TODO add friction / air resistance
	e->Rotate(e->GetRotationAxis(), e->GetRotationSpeed());
}

// Moves and entity to the point where it starts colliding with another object.
// Returns the time at which this happens (in seconds since the last physics tick).
float EntityUpdater::MoveEntityToCollisionTime_(shared_ptr<Entity> e, float startTime, float endTime, Collision &outputCollision) {
	while (abs(startTime - endTime) > collisionTimePrecision) {
		// Binary search the correct time point.
		float middleTime = (startTime + endTime) / 2;
		MoveEntity_(e, middleTime - startTime);
		if (collisionDetector_.IsCollidingQ(e, outputCollision)) {
			startTime = middleTime;
		}
		else {
			endTime = middleTime;
			MoveEntity_(e, startTime - middleTime);
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
		float firstAcceleration = impactSpeed * firstWeight / (firstWeight + secondWeight);
		AccelerateEntityAtPoint(firstAsEntity, firstAcceleration * -c.impactDirectionAtFirst, positionLocalToFirst);

		float secondAcceleration = impactSpeed * secondWeight / (firstWeight + secondWeight);
		AccelerateEntityAtPoint(secondAsEntity, firstAcceleration * -c.impactDirectionAtFirst, positionLocalToSecond);
	}
	else {
		// Make "first" the entity and "second" the object"
		if (secondAsEntity) {
			firstAsEntity = secondAsEntity;
		}

		// Just get the speed of impact and use all that speed to accelerate the entity
		// since the object is stable anyway.
		mat4 transformFirst = c.first->LocalToWorldSpaceMatrix();
		vec3 positionLocalToFirst = vec3(inverse(transformFirst) * vec4(c.worldPosition, 1));
		vec3 firstImpactSpeed = vec3(transformFirst * vec4(
			GetLocalSpeedAtCollision(firstAsEntity,
									 positionLocalToFirst,
									 c.impactDirectionAtFirst), 0)
		);
		float firstAcceleration = length(firstImpactSpeed);
		AccelerateEntityAtPoint(firstAsEntity, firstAcceleration * -c.impactDirectionAtFirst, positionLocalToFirst);
	}
}
