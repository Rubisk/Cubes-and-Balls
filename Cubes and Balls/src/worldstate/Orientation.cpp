#include "Orientation.h"

#include "glm/gtx/rotate_vector.hpp"

using namespace glm;

void Orientation::Rotate(const vec3 &axis, float degrees) {
	upSide = rotate(upSide, degrees, axis);
	frontSide = rotate(frontSide, degrees, axis);
}
