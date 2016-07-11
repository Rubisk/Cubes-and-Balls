#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_WALL_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_WALL_H_

#include "Object.h"

class Wall :
	public Object {
public:
	Wall(glm::vec3 size);

	std::shared_ptr<const Model> GetModel() const;

	virtual Material GetMaterial() const;
private:
	glm::vec3 size_;
};

#endif