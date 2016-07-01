#ifndef CUBES_AND_BALLS_SRC_WORDSTATE_OBJECT_H_
#define CUBES_AND_BALLS_SRC_WORDSTATE_OBJECT_H_

#include <string>

#include "glm/glm.hpp"

#include "Orientation.h"

// This class and it's access functions are all thread-safe.
class Object {
public:
	Orientation orientation;

	virtual std::string GetModelName();

	virtual glm::vec3 GetModelScale();

	glm::vec3 GetPosition() const;

	void SetPosition(const glm::vec3 &position);

	virtual ~Object();
private:
	glm::vec3 position_ = glm::vec3(0, 0, 0);
};

#endif