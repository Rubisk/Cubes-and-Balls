#ifndef CUBE_AND_BALLS_SRC_CAMERA_H_
#define CUBE_AND_BALLS_SRC_CAMERA_H_

#include "glm/glm.hpp"
#include "Orientation.h"


class Camera {
public:
	void LookAt(const glm::vec3 &point);

	// worldCoords: True if we're using world coordinates instead of view coordinates. defaults to false.
	void Rotate(const glm::vec3 &axis, float radians);

	void Rotate(const glm::vec3 &axis, float radians, bool worldCoords);

	void GoTo(const glm::vec3 &position);

	// worldCoords: True if we're using world coordinates instead of view coordinates. defaults to false.
	void Move(const glm::vec3 &distance);

	void Move(const glm::vec3 &distance, bool worldCoords);

	glm::vec3 GetPosition() const;

	Orientation GetOrientation() const;

	// Returns the point the center of the camera is looking at, at <distance> away.
	glm::vec3 GetPointLookingAt(float distance) const;

	// Returns a matrix mapping a point in world space to the correct point in view space.
	glm::mat4 GetViewToWorldMatrix() const;

	// Use this to convert coordinates in camera/view-space to world space coordinates.
	// viewPoint are some coordinates relative to the camera (0, 0, 1) is a little bit forward,
	// (0, 1, 0) is up a bit, and returns the world coordinates representing that point.
	glm::vec3 ViewPointToWorld(const glm::vec3 &viewPoint) const;
private:
	Orientation orientation_;
	glm::vec3 position_;
};

#endif