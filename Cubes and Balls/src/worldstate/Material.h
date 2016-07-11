#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_MATERIAL_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_MATERIAL_H_

#include "glm/glm.hpp"

class Material {
public:
	Material() {};

	// Color to render this material with
	virtual glm::vec4 GetColor() {
		return vec4(1, 1, 1, 1);
	};

	// Value between 0 and 1 signifying the amount of energy "lost" on impact with another
	// object. 1 means all energy is lost, and 0 means nothing is lost.
	virtual float GetDampening() {
		return 0;
	};

	virtual ~Material() {};
};

#endif