#include "InputHandler.h"
#include "Game.h"

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"


using namespace glm;

void InputHandler::KeyInput(GLFWwindow * window, int key, int scancode, int action, int mods) {
	Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
	switch (key) {
	case GLFW_KEY_W:
		game->worldState_->camera.Move(vec3(0, 0, 0.2));
		break;
	case GLFW_KEY_S:
		game->worldState_->camera.Move(vec3(0, 0, -0.2));
		break;
	case GLFW_KEY_D:
		game->worldState_->camera.Move(vec3(0.2, 0, 0));
		break;
	case GLFW_KEY_A:
		game->worldState_->camera.Move(vec3(-0.2, 0, 0));
		break;
	case GLFW_KEY_I:
		game->worldState_->camera.Rotate(vec3(1, 0, 0), -pi<float>() / 100);
		break;
	case GLFW_KEY_K:
		game->worldState_->camera.Rotate(vec3(1, 0, 0), pi<float>() / 100);
		break;
	}
}

void InputHandler::MouseButtonInput(GLFWwindow * window, int button, int action, int mods) {
	Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
}
