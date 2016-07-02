#ifndef CUBES_AND_BALLS_SRC_PHYSICS_FORCEAPPLIER_H_
#define CUBES_AND_BALLS_SRC_PHYSICS_FORCEAPPLIER_H_

#include <memory>

#include "glm/glm.hpp"

#include "../worldstate/Entity.h"

// Class holding utility methods for applying forces to entities.
// One newton unit is a unit*kilogram/seconds^2
void ApplyForce(const std::shared_ptr<Entity> &e,
				const glm::vec3 &force,
				const glm::vec3 &localPosition,
				float duration);

#endif