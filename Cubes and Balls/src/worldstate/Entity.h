#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_ENTITY_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_ENTITY_H_

#include <string>
#include <mutex>

#include "glm/glm.hpp"

#include "Orientation.h"
#include "Object.h"

// This class and it's access functions are all thread-safe.
class Entity : 
	public Object {
public:
	virtual float GetWeight() const;

	virtual glm::vec3 GetSpeed() const;

	virtual void SetSpeed(const glm::vec3 &speed);

	virtual glm::vec3 GetRotationAxis() const;

	virtual void SetRotationAxis(const glm::vec3 &axis);

	virtual float GetRotationSpeed() const;

	virtual void SetRotationSpeed(float radians);

	virtual ~Entity() {};
private:

	mutable std::mutex speedmtx_;
	mutable std::mutex rotationAxismtx_;
	mutable std::mutex rotationSpeedmtx_;

	glm::vec3 speed_ = glm::vec3(0, 0, 0);     // units/sec

	glm::vec3 rotationAxis_ = glm::vec3(0, 1, 0);
	float rotationSpeed_ = 0; // radians/sec
};

#endif