#ifndef CUBE_AND_BALLS_SRC_CAMERA_H_
#define CUBE_AND_BALLS_SRC_CAMERA_H_

#include "glm/glm.hpp"
#include "Entity.h"


class Player :
	public Entity {
public:
	void LookAt(const glm::vec3 &point);

	// Use this to rotate using local-coordinate space arguments
	virtual void RotateLocal(const glm::vec3 &axis, float radians);

	virtual void MoveLocal(const glm::vec3 &distance);

	virtual float GetWeight();

	// Returns the point the center of the camera is looking at, at <distance> away.
	glm::vec3 GetPointLookingAt(float distance) const;

	// Returns a matrix mapping a point in world space to the correct point in view space.
	glm::mat4 LocalToWorldSpaceMatrix() const;

	// Use this to convert coordinates in camera/view-space to world space coordinates.
	// viewPoint are some coordinates relative to the camera (0, 0, 1) is a little bit forward,
	// (0, 1, 0) is up a bit, and returns the world coordinates representing that point.
	glm::vec3 ViewPointToWorld(const glm::vec3 &viewPoint) const;
};

#endif