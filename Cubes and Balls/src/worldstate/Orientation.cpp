#include "Orientation.h"

#include <mutex>

#include "glm/gtx/rotate_vector.hpp"

using namespace glm;
using namespace std;

mutex orientationmtx;


void Orientation::Rotate(const vec3 &axis, float radians) {
	if (radians == 0.0f) return;
	vec3 upSide = rotate(GetUpSide(), radians, axis);
	vec3 frontSide = rotate(GetFrontSide(), radians, axis);
	WriteUpFrontSide(frontSide, upSide);
}

void Orientation::SetUpSide(const vec3 &u) {
	vec3 upSide = normalize(u);
	vec3 frontSide = GetFrontSide();

	// Up must be orthogonal to front. If they're in the same 1-dimensional subspace, we 
	// reset up to either (0, 0, 1) or (0, -1, 0).
	if (distance(frontSide, upSide) < 0.00001 ||
		distance(frontSide, -upSide) < 0.00001) {
		frontSide = (-0.5 < upSide.z || upSide.z < 0.5)
			? vec3(0, -1, 0) : vec3(0, 0, 1);
	}
	else {
		// Gram Schmidt
		frontSide = normalize(frontSide - dot(frontSide, upSide) * upSide_);
	}
	WriteUpFrontSide(frontSide, upSide);
}

void Orientation::SetFrontSide(const vec3 &f) {
	vec3 frontSide = normalize(f);
	vec3 upSide = GetUpSide();

	// Up must be orthogonal to front. If they're in the same 1-dimensional subspace, we 
	// reset up to either (0, 1, 0) or (0, 0, 1).
	if (distance(frontSide, upSide) < 0.00001 ||
		distance(frontSide, -upSide) < 0.00001) {
		upSide = (-0.5 < frontSide_.y || frontSide_.y < 0.5)
			? vec3(1, 0, 0) : vec3(0, 1, 0);
	}
	else {
		// Gram Schmidt
		upSide = normalize(upSide - dot(upSide, frontSide) * frontSide);
	}
	WriteUpFrontSide(frontSide, upSide);
}

vec3 Orientation::GetUpSide() const {
	orientationmtx.lock();
	vec3 upSide = upSide_;
	orientationmtx.unlock();
	return upSide;
}

vec3 Orientation::GetFrontSide() const {
	orientationmtx.lock();
	vec3 frontSide = frontSide_;
	orientationmtx.unlock();
	return frontSide;
}

Orientation::~Orientation() {}

void Orientation::WriteUpFrontSide(const glm::vec3 &frontSide, const glm::vec3 &upSide) {
	orientationmtx.lock();
	upSide_ = upSide;
	frontSide_ = frontSide;
	orientationmtx.unlock();
}
