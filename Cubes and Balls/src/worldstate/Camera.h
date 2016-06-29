#ifndef CUBE_AND_BALLS_SRC_CAMERA_H_
#define CUBE_AND_BALLS_SRC_CAMERA_H_

#include "glm/glm.hpp"
#include "Orientation.h"


class Camera {
public:
	void LookAt(glm::vec3 point);

	void Rotate(glm::vec3 axis, float degrees);

	void GoTo(glm::vec3 position);

	void Move(glm::vec3 distance);

	glm::vec3 GetPosition();

	Orientation GetOrientation();

	// Returns the point the center of the camera is looking at, at <distance> away.
	glm::vec3 GetPointLookingAt(float distance);
private:
	Orientation orientation_;
	glm::vec3 position_;
};

#endif