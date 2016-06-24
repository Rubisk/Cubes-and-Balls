#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_ENTITY_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_ENTITY_H_

#include "glm/glm.hpp"

// Data type used to represent orientations of objects.
// rightSide is the way (1, 0, 0) is pointing, upSide is the way 
// (0, 1, 0) is pointing. This represents a unique orientation.
struct Orientation {
	glm::vec3 rightSide;
	glm::vec3 upSide;
};

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