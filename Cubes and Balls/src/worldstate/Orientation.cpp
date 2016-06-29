#include "Orientation.h"

#include "glm/gtx/rotate_vector.hpp"

using namespace glm;

void Orientation::Rotate(const vec3 &axis, float degrees) {
	upSide = rotate(upSide, degrees, axis);
	frontSide = rotate(frontSide, degrees, axis);
}

mat4 Orientation::GetRotationMatrix() {
	return mat4(vec4(cross(frontSide, upSide), 0), 
				vec4(upSide, 0), 
				vec4(frontSide, 0), 
				vec4(0, 0, 0, 1));
}