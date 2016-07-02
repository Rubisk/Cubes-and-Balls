#ifndef CUBE_AND_BALLS_SRC_PLAYERCONTROLLER_H_
#define CUBE_AND_BALLS_SRC_PLAYERCONTROLLER_H_

#include "worldstate/Player.h"
#include "physics/ForceGenerator.h"

enum MoveDirection {
	M_Forward,
	M_Backward,
	M_Left,
	M_Right,
};

enum RotationDirection {
	R_Up,
	R_Down,
	R_Left,
	R_Right,
};


class PlayerController :
	public ForceGenerator {
public:
	PlayerController(std::shared_ptr<Player> player);

	virtual void GenerateForces(ForceApplier &forceApplier, float timePassed);

	void StartMoving(MoveDirection direction);

	void StopMoving(MoveDirection direction);

	void StartRotating(RotationDirection direction);

	void StopRotating(RotationDirection direction);
private:
	bool moveDirections_[4] = {false, false, false, false};
	bool rotationDirections_[4] = {false, false, false, false};

	std::shared_ptr<Player> player_;
};

#endif