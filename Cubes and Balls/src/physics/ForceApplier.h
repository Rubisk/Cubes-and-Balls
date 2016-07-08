#ifndef CUBES_AND_BALLS_SRC_PHYSICS_FORCEAPPLIER_H_
#define CUBES_AND_BALLS_SRC_PHYSICS_FORCEAPPLIER_H_

#include <memory>
#include <list>

#include "glm/glm.hpp"

#include "../worldstate/Entity.h"

// Class holding utility methods for applying forces to entities.
// One newton unit is a unit*kilogram/seconds^2
class ForceApplier {
public:
	struct Force {
		Force(std::shared_ptr<Entity> e, 
			  glm::vec3 &forceVec, 
			  glm::vec3 &localPosition, 
			  float duration) :
			e(e), forceVec(forceVec), localPosition(localPosition), duration(duration) {};

		std::shared_ptr<Entity> e;
		glm::vec3 forceVec;
		glm::vec3 localPosition;

		// Duration to apply the force. If this is 0, the force is considered to be applied during an "infinitisemal"
		// amount of time, with total energy as if the duration was 1 second. (u physics people said it was ok)
		float duration;
	};

	void AddForce(std::shared_ptr<Entity> e,
				  glm::vec3 &forceVec,
				  glm::vec3 &localPosition,
				  float duration);

	void AddForce(Force &f);

	void AddForceLocal(std::shared_ptr<Entity> e,
					   glm::vec3 &forceVec,
					   glm::vec3 &localPosition,
					   float duration);

	void AddForceLocal(Force &f);

	void UpdateForces(float timePassed);
private:
	// Used for storing info on what forces to update.

	void ApplyForce_(Force &f);

	std::list<Force> forcesToApply_;
};

#endif