#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_ENTITY_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_ENTITY_H_

#include <string>

#include "glm/glm.hpp"

#include "Orientation.h"
#include "Object.h"

// This class and it's access functions are all thread-safe.
class Entity : 
	public Object {
public:
	virtual float GetWeight() const;

	glm::vec3 GetSpeed() const;

	void SetSpeed(const glm::vec3 &speed);

	glm::vec3 GetRotationAxis() const;

	void SetRotationAxis(const glm::vec3 &axis);

	float GetRotationSpeed() const;

	void SetRotationSpeed(float radians);

	virtual ~Entity() {};
private:
	glm::vec3 speed_ = glm::vec3(0, 0, 0);     // units/sec

	glm::vec3 rotationAxis_;
	float rotationSpeed_ = 0; // radians/sec
};

#endif