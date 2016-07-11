#include "ForceApplier.h"

using namespace std;
using namespace glm;

void ForceApplier::AddForce(shared_ptr<Entity> e,
							vec3 &forceVec,
							vec3 &localPosition,
							float duration) {
	Force f(e, forceVec, localPosition, duration);
	if (f.duration == 0) {
		f.duration = 1;
		ApplyForce_(f);
	}
	else {
		forcesToApply_.push_front(f);
	}
}

void ForceApplier::AddForce(Force &f) {
	forcesToApply_.push_front(f);
}

void ForceApplier::AddForceLocal(shared_ptr<Entity> e,
							vec3 &forceVec,
							vec3 &localPosition,
							float duration) {
	Force f(e, forceVec, localPosition, duration);
	AddForceLocal(f);
}

void ForceApplier::AddForceLocal(Force &f) {
	f.forceVec = vec3(f.e->LocalToWorldSpaceMatrix() * vec4(f.forceVec, 0));
	f.localPosition = vec3(f.e->LocalToWorldSpaceMatrix() * vec4(f.localPosition, 0));
	if (f.duration == 0) {
		f.duration = 1;
		ApplyForce_(f);
	}
	else {
		forcesToApply_.push_front(f);
	}
}


void ForceApplier::UpdateForces(float timePassed) {
	list<Force>::iterator f = forcesToApply_.begin();
	while (f != forcesToApply_.end()) {
		float timeLeft = f->duration - timePassed;
		if (timeLeft < 0) {
			ApplyForce_(*f);
			forcesToApply_.erase(f++);
		}
		else {
			f->duration = timePassed;
			ApplyForce_(*f);
			f->duration = timeLeft;
		}
	}
}

void ForceApplier::ApplyForce_(Force &force) {
	if (force.e == nullptr) return;

	shared_ptr<Entity> &e = force.e;
	float duration = force.duration;
	vec3 localPosition = force.localPosition;
	vec3 forceVec = force.forceVec;

	// Split the force in a force that rotates the object, and one that moves the center.
	// (I suggest sketching the picture of a sphere)
	vec3 rotationForce;
	if (length(localPosition) < 0.00001f) rotationForce = vec3(0, 0, 0);
	else rotationForce = forceVec - dot(forceVec, normalize(localPosition)) * normalize(localPosition); // Gram-Schmidt
	vec3 centerForce = forceVec - rotationForce;

	float weight = e->GetWeight();

	// Update center movement
	vec3 speed = e->GetSpeed();
	speed += (duration / weight) * centerForce;
	e->SetSpeed(speed);

	// Update rotation momentum
	vec3 oldRotationAxis = e->GetRotationAxis();
	vec3 newRotationAxis;
	if (length(localPosition) < 0.00001f || length(rotationForce) < 0.00001f) newRotationAxis = vec3(0, 0, 0);
	else newRotationAxis = normalize(cross(localPosition, rotationForce));
	

	float oldRotationSpeed = e->GetRotationSpeed();
	float newRotationSpeed;
	if (length(localPosition) < 0.00001f) newRotationSpeed = 0;
	else newRotationSpeed = length(rotationForce) * duration / (weight * length(localPosition));

	vec3 totalRotationAxis = (oldRotationSpeed * oldRotationAxis + newRotationSpeed * newRotationAxis);
	totalRotationAxis = (length(totalRotationAxis) < 0.00001f) ? totalRotationAxis : normalize(totalRotationAxis);

	e->SetRotationAxis(totalRotationAxis);
	e->SetRotationSpeed(dot(oldRotationAxis, totalRotationAxis) * oldRotationSpeed + dot(newRotationAxis, totalRotationAxis) * newRotationSpeed);
}
