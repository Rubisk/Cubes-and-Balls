#include "Entity.h"

#include "glm/gtx/rotate_vector.hpp"

void Entity::Tick(float time) {
	position += time * speed;
	orientation.upSide = glm::rotate(orientation.upSide, rotationSpeed, rotationAxis);
	orientation.rightSide = glm::rotate(orientation.rightSide, rotationSpeed, rotationAxis);
}
