#include "Game.h"

#include "world/World.h"
#include "renderer/Renderer.h"

void Game::Start() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window_ = glfwCreateWindow(800, 600, "Cubes and Balls", nullptr, nullptr);
	glfwMakeContextCurrent(window_);

	World world;
	Renderer renderer(800, 600);
	// WorldRenderer worldrenderer;
}

void Game::Stop() {}
