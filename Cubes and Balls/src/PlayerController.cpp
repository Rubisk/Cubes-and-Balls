#include "PlayerController.h"

using namespace std;
using namespace glm;

PlayerController::PlayerController(shared_ptr<Player> player) {
	player_ = player;
}

void PlayerController::GenerateForces(std::shared_ptr<ForceApplier> forceApplier, float timePassed) {
	if (moveDirections_[M_Forward]) forceApplier->AddForceLocal(player_, vec3(0, 0, -10), vec3(0, 0, 0), timePassed);
	if (moveDirections_[M_Backward]) forceApplier->AddForceLocal(player_, vec3(0, 0, 10), vec3(0, 0, 0), timePassed);
	if (moveDirections_[M_Left]) forceApplier->AddForceLocal(player_, vec3(-10, 0, 0), vec3(0, 0, 0), timePassed);
	if (moveDirections_[M_Right]) forceApplier->AddForceLocal(player_, vec3(10, 0, 0), vec3(0, 0, 0), timePassed);

	if (rotationDirections_[R_Up]) forceApplier->AddForceLocal(player_, vec3(0, 0.3, 0), vec3(0, 0, -1), timePassed);
	if (rotationDirections_[R_Down]) forceApplier->AddForceLocal(player_, vec3(0, -0.3, 0), vec3(0, 0, -1), timePassed);
	if (rotationDirections_[R_Left]) forceApplier->AddForceLocal(player_, vec3(-0.3, 0, 0), vec3(0, 0, -1), timePassed);
	if (rotationDirections_[R_Right]) forceApplier->AddForceLocal(player_, vec3(0.3, 0, 0), vec3(0, 0, -1), timePassed);
}

void PlayerController::StartMoving(MoveDirection direction) {
	moveDirections_[direction] = true;
}

void PlayerController::StopMoving(MoveDirection direction) {
	moveDirections_[direction] = false;
}

void PlayerController::StartRotating(RotationDirection direction) {
	rotationDirections_[direction] = true;
}

void PlayerController::StopRotating(RotationDirection direction) {
	rotationDirections_[direction] = false;
}
