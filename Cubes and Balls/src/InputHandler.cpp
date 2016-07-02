#include "InputHandler.h"
#include "Game.h"

#include "physics/ForceApplier.h"

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"


using namespace glm;

void InputHandler::KeyInput(GLFWwindow * window, int key, int scancode, int action, int mods) {
	Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
	switch (key) {
	case GLFW_KEY_W:
		game->worldState_->player->MoveLocal(vec3(0, 0, -0.2));
		break;
	case GLFW_KEY_S:
		game->worldState_->player->MoveLocal(vec3(0, 0, 0.2));
		break;
	case GLFW_KEY_D:
		game->worldState_->player->MoveLocal(vec3(0.2, 0, 0));
		break;
	case GLFW_KEY_A:
		game->worldState_->player->MoveLocal(vec3(-0.2, 0, 0));
		break;
	case GLFW_KEY_I:
		game->worldState_->player->RotateLocal(vec3(1, 0, 0), pi<float>() / 100);
		break;
	case GLFW_KEY_K:
		game->worldState_->player->RotateLocal(vec3(1, 0, 0), -pi<float>() / 100);
		break;
	case GLFW_KEY_J:
		game->worldState_->player->RotateLocal(vec3(0, 1, 0), pi<float>() / 100);
		break;
	case GLFW_KEY_L:
		game->worldState_->player->RotateLocal(vec3(0, 1, 0), -pi<float>() / 100);
		break;
	case GLFW_KEY_F:
		ApplyForce(game->worldState_->GetEntities().front(), vec3(0, 1, 0), vec3(0, 0, 1), 0.02f);
		break;
	case GLFW_KEY_G:
		ApplyForce(game->worldState_->GetEntities().front(), vec3(0, 1, 0), vec3(0, 0, -1), 0.02f);
	}
}

void InputHandler::MouseButtonInput(GLFWwindow * window, int button, int action, int mods) {
	Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
}
