#ifndef CUBES_AND_BALLS_SRC_PHYSICS_COLLISIONDETECTOR_H_
#define CUBES_AND_BALLS_SRC_PHYSICS_COLLISIONDETECTOR_H_

#include <memory>

#include "../worldstate/WorldState.h"

// Used as return value for collision checking
struct Collision {
	std::shared_ptr<Object> first;
	std::shared_ptr<Object> second;
	glm::vec3 worldPosition;

	// Normalized vector specifying the direction at which the first object is crashing into the second.
	glm::vec3 impact;
};

class CollisionDetector {
public:
	void SetWorldState(std::shared_ptr<WorldState> world);

	std::vector<Collision> LookForCollisions();
private:
	bool IsCollidingQ_(std::shared_ptr<Object> toTest, Collision &outputCollission);

	bool CollidingQ_(std::shared_ptr<Object> first, std::shared_ptr<Object> second, Collision &outputCollission);

	std::shared_ptr<WorldState> world_;
};

#endif