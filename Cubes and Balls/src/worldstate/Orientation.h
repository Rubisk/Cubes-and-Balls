#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_ORIENTATION_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_ORIENTATION_H_

#include "glm/glm.hpp"

// Data type used to represent orientations of objects.
// frontSide is the way (0, 0, 1) is pointing, upSide is the way 
// (0, 1, 0) is pointing. This represents a unique orientation.

// This class and it's access functions are all thread-safe.
struct Orientation {
public:
	// Rotate an orientation along axis
	void Rotate(const glm::vec3 &axis, float radians);

	void SetUpSide(const glm::vec3 &upSide);

	void SetFrontSide(const glm::vec3 &frontSide);

	virtual glm::vec3 GetUpSide() const;

	virtual glm::vec3 GetFrontSide() const;

	virtual ~Orientation();
private:
	void WriteUpFrontSide(glm::vec3 frontSide, glm::vec3 upSide);

	glm::vec3 frontSide_ = glm::vec3(0, 0, 1);
	glm::vec3 upSide_ = glm::vec3(0, 1, 0);

};

#endif