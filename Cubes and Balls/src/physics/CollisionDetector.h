#ifndef CUBES_AND_BALLS_SRC_PHYSICS_COLLISIONDETECTOR_H_
#define CUBES_AND_BALLS_SRC_PHYSICS_COLLISIONDETECTOR_H_

#include <memory>

#include "../worldstate/WorldState.h"

class CollisionDetector {
public:
	void SetWorldState(std::shared_ptr<WorldState> world);

	bool IsColliding(std::shared_ptr<Object> toTest, glm::vec3 &outputCenterOfCollision, std::shared_ptr<Object> &outputCollider);

	bool Colliding(std::shared_ptr<Object> first, std::shared_ptr<Object> second, glm::vec3 &outputCenterOfCollision);
private:
	std::shared_ptr<WorldState> world_;
};

#endif