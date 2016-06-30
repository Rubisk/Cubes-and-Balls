#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_ENTITY_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_ENTITY_H_

#include <string>

#include "glm/glm.hpp"
#include "Orientation.h"

class Entity {
public:
	virtual std::string GetModelName();

	virtual float GetWeight();

	virtual bool IsImmovable();

	glm::vec3 GetPosition();

	void SetPosition(const glm::vec3 &position);

	glm::vec3 GetSpeed();

	void SetSpeed(const glm::vec3 &speed);

	glm::vec3 GetRotationAxis();

	void SetRotationAxis(const glm::vec3 &axis);

	float GetRotationSpeed();

	void SetRotationSpeed(float radians);

	// Orientation handles it's own thread safety
	Orientation orientation;

	virtual ~Entity() {};
private:
	glm::vec3 position_ = glm::vec3(0, 0, 0);
	glm::vec3 speed_ = glm::vec3(0, 0, 0);     // units/sec

	glm::vec3 rotationAxis_;
	float rotationSpeed_ = 0; // radians/sec
};

#endif