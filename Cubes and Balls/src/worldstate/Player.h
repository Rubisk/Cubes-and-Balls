#ifndef CUBE_AND_BALLS_SRC_PLAYER_H_
#define CUBE_AND_BALLS_SRC_PLAYER_H_

#include "glm/glm.hpp"
#include "Entity.h"

class Player :
	public Entity {
public:
	virtual std::shared_ptr<const Model> GetModel() const;

	virtual Material GetMaterial() const;

	void LookAt(const glm::vec3 &point);

	virtual float GetWeight() const;
};

#endif