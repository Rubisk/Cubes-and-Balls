#include "Orientation.h"

#include "glm/gtx/rotate_vector.hpp"

using namespace glm;

void Orientation::Rotate(const vec3 &axis, float radians) {
	upSide_ = rotate(upSide_, radians, axis);
	frontSide_ = rotate(frontSide_, radians, axis);
}

void Orientation::SetUpSide(const vec3 &upSide) {
	upSide_ = normalize(upSide);

	// Up must be orthogonal to front. If they're in the same 1-dimensional subspace, we 
	// reset up to either (0, 0, 1) or (0, -1, 0).
	if (distance(frontSide_, upSide_) < 0.00001 ||
		distance(frontSide_, -upSide_) < 0.00001) {
		frontSide_ = (-0.5 < upSide_.z && upSide_.z < 0.5)
			? vec3(0, 0, 1) : vec3(0, -1, 0);
	}
	else {
		// Gram Schmidt
		frontSide_ = normalize(frontSide_ - dot(frontSide_, upSide_) * upSide_);
	}
}

void Orientation::SetFrontSide(const vec3 &frontSide) {
	frontSide_ = normalize(frontSide);

	// Up must be orthogonal to front. If they're in the same 1-dimensional subspace, we 
	// reset up to either (0, 1, 0) or (0, 0, 1).
	if (distance(frontSide_, upSide_) < 0.00001 ||
		distance(frontSide_, -upSide_) < 0.00001) {
		upSide_ = (-0.5 < frontSide_.y && frontSide_.y < 0.5)
			? vec3(0, 1, 0) : vec3(1, 0, 0);
	}
	else {
		// Gram Schmidt
		upSide_ = normalize(upSide_ - dot(upSide_, frontSide_) * frontSide_);
	}
}

vec3 Orientation::GetUpSide() {
	return upSide_;
}

vec3 Orientation::GetFrontSide() {
	return frontSide_;
}