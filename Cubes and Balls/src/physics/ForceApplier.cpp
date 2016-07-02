#include "ForceApplier.h"

using namespace std;
using namespace glm;

void ApplyForce(const shared_ptr<Entity> &e,
				const vec3 &force,
				const vec3 &localPosition,
				float duration) {
	// Split the force in a force that rotates the object, and one that moves the center.
	// (I suggest sketching the picture of a sphere)
	vec3 rotationForce = force - dot(force, localPosition) * force; // Gram-Schmidt
	vec3 centerForce = force - rotationForce;

	float weight = e->GetWeight();

	// Update center movement
	vec3 speed = e->GetSpeed();
	speed += (duration / weight) * centerForce;
	e->SetSpeed(speed);

	// Update rotation momentum
	vec3 oldRotationAxis = e->GetRotationAxis();
	vec3 newRotationAxis = normalize(cross(localPosition, rotationForce));

	float oldRotationSpeed = e->GetRotationSpeed();
	float newRotationSpeed = length(rotationForce) * duration / (weight * length(localPosition));

	vec3 totalRotationAxis = (oldRotationSpeed * oldRotationAxis + newRotationSpeed * newRotationAxis);
	totalRotationAxis = (length(totalRotationAxis) == 0) ? totalRotationAxis : normalize(totalRotationAxis);

	e->SetRotationAxis(totalRotationAxis);
	e->SetRotationSpeed(dot(oldRotationAxis, newRotationAxis) * oldRotationSpeed + newRotationSpeed);
}
