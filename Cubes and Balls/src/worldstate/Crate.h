#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_CRATE_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_CRATE_H_

#include <string>

#include "Entity.h"


class Crate :
	public Entity {
public:
	std::shared_ptr<const Model> GetModel() const;

	virtual Material GetMaterial() const;

	virtual float GetWeight() const;
};

#endif