#ifndef CUBE_AND_BALLS_SRC_CAMERA_H_
#define CUBE_AND_BALLS_SRC_CAMERA_H_

#include "glm/glm.hpp"
#include "Object.h"


class Camera :
	public Object {
public:
	void LookAt(const glm::vec3 &point);

	// worldCoords: True if we're using world coordinates instead of view coordinates. defaults to false.
	virtual void Rotate(const glm::vec3 &axis, float radians);

	virtual void Rotate(const glm::vec3 &axis, float radians, bool worldCoords);

	// worldCoords: True if we're using world coordinates instead of view coordinates. defaults to false.
	virtual void Move(const glm::vec3 &distance);

	virtual void Move(const glm::vec3 &distance, bool worldCoords);

	// Returns the point the center of the camera is looking at, at <distance> away.
	glm::vec3 GetPointLookingAt(float distance) const;

	// Returns a matrix mapping a point in world space to the correct point in view space.
	glm::mat4 GetViewToWorldMatrix() const;

	// Use this to convert coordinates in camera/view-space to world space coordinates.
	// viewPoint are some coordinates relative to the camera (0, 0, 1) is a little bit forward,
	// (0, 1, 0) is up a bit, and returns the world coordinates representing that point.
	glm::vec3 ViewPointToWorld(const glm::vec3 &viewPoint) const;
};

#endif