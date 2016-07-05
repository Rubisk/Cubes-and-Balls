#ifndef CUBES_AND_BALLS_SRC_PHYSICS_COLLISIONDETECTOR_H_
#define CUBES_AND_BALLS_SRC_PHYSICS_COLLISIONDETECTOR_H_

#include <memory>

#include "../worldstate/WorldState.h"

// Used as return value for collision checking
struct Collision {
	shared_ptr<Object> first;
	shared_ptr<Object> second;
	vec3 worldPosition;

	// These are (normalized) vectors specifying which way force should be applied at collision.
	// If (for example), the collision occurs with exactly one face, this is the opposite
	// of the face normal. Interpolates between faces if the collision is on face edges.
	// Note that these coordinates are in MODEL SPACE (!!!)
	vec3 impactDirectionAtFirst;
	vec3 impactDirectionAtSecond;
};

class CollisionDetector {
public:
	void SetWorldState(std::shared_ptr<WorldState> world);

	bool IsCollidingQ(std::shared_ptr<Object> toTest, Collision &outputCollission);

	bool CollidingQ(std::shared_ptr<Object> first, std::shared_ptr<Object> second, Collision &outputCollission);
private:
	std::shared_ptr<WorldState> world_;
};

#endif