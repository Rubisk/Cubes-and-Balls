#include "Game.h"

#include "worldstate/WorldState.h"
#include "renderer/Renderer.h"
#include "renderer/WorldRenderer.h"

using namespace std;

void Game::Start() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window_ = glfwCreateWindow(800, 600, "Cubes and Balls", nullptr, nullptr);
	glfwMakeContextCurrent(window_);

	shared_ptr<WorldState> world = make_shared<WorldState>();
	Renderer renderer(800, 600);
	shared_ptr<WorldRenderer> worldRenderer = make_shared<WorldRenderer>(world);
	renderer.AddDrawer(worldRenderer);
}

void Game::Stop() {}
