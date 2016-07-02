#include "InputHandler.h"

#include "Game.h"
#include "worldstate/Entity.h"
#include "physics/ForceApplier.h"

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"


using namespace glm;

void InputHandler::KeyInput(GLFWwindow * window, int key, int scancode, int action, int mods) {
	Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
	switch (key) {
	case GLFW_KEY_W:
		if (action == GLFW_PRESS)
			game->playerController->StartMoving(M_Forward);
		else if (action == GLFW_RELEASE)
			game->playerController->StopMoving(M_Forward);
		break;
	case GLFW_KEY_S:
		if (action == GLFW_PRESS)
			game->playerController->StartMoving(M_Backward);
		else if (action == GLFW_RELEASE)
			game->playerController->StopMoving(M_Backward);
		break;
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)
			game->playerController->StartMoving(M_Right);
		else if (action == GLFW_RELEASE)
			game->playerController->StopMoving(M_Right);
		break;
	case GLFW_KEY_A:
		if (action == GLFW_PRESS)
			game->playerController->StartMoving(M_Left);
		else if (action == GLFW_RELEASE)
			game->playerController->StopMoving(M_Left);
		break;
	case GLFW_KEY_I:
		if (action == GLFW_PRESS)
			game->playerController->StartRotating(R_Up);
		else if (action == GLFW_RELEASE)
			game->playerController->StopRotating(R_Up);
		break;
	case GLFW_KEY_K:
		if (action == GLFW_PRESS)
			game->playerController->StartRotating(R_Down);
		else if (action == GLFW_RELEASE)
			game->playerController->StopRotating(R_Down);
		break;
	case GLFW_KEY_J:
		if (action == GLFW_PRESS)
			game->playerController->StartRotating(R_Left);
		else if (action == GLFW_RELEASE)
			game->playerController->StopRotating(R_Left);
		break;
	case GLFW_KEY_L:
		if (action == GLFW_PRESS)
			game->playerController->StartRotating(R_Right);
		else if (action == GLFW_RELEASE)
			game->playerController->StopRotating(R_Right);
		break;
	}
}

void InputHandler::MouseButtonInput(GLFWwindow * window, int button, int action, int mods) {
	Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
}
