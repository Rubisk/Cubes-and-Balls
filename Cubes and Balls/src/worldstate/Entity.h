#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_ENTITY_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_ENTITY_H_

#include "glm/glm.hpp"
#include "Orientation.h"

class Entity {
public:
	virtual std::string GetModelName() { return ""; };

	glm::vec3 position;
	glm::vec3 speed;     // units/sec

	Orientation orientation;
	glm::vec3 rotationAxis;
	float rotationSpeed; // radians/sec

	virtual ~Entity() {};
};

#endif