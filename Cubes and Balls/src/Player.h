#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_PLAYER_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_PLAYER_H_

#include <memory>

#include "worldstate/Camera.h"
#include "worldstate/Entity.h"

class Player :
	public Entity {
public:
	void LinkCamera(std::weak_ptr<Camera> &camera);

	virtual void SetPosition(glm::vec3 &pos);

	virtual void Rotate(const glm::vec3 &axis, float radians);

	virtual float GetWeight();

	virtual std::string GetModelName();
private:
	std::weak_ptr<Camera> &camera_;
};

#endif