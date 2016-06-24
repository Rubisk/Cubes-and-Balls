#ifndef CUBES_AND_BALLS_SRC_WORLDSTATE_CRATE_H_
#define CUBES_AND_BALLS_SRC_WORLDSTATE_CRATE_H_

#include <string>

#include "Entity.h"


class Crate :
	public Entity {
public:
	virtual std::string GetModelName();

	~Crate();
};

#endif