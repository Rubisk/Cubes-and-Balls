#include "Game.h"

#include "worldstate/WorldState.h"
#include "worldstate/Crate.h"
#include "renderer/Renderer.h"
#include "renderer/WorldRenderer.h"

using namespace std;

void Game::Setup() {
	if (!glfwInit()) throw std::runtime_error("GLFW failed to initialize");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window_ = glfwCreateWindow(800, 600, "Cubes and Balls", nullptr, nullptr);
	glfwMakeContextCurrent(window_);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) throw std::runtime_error("Glew failed to initialize");

	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	worldState_ = make_shared<WorldState>();
	worldRenderer_ = make_shared<WorldRenderer>(worldState_);
	renderer_ = make_shared<Renderer>(800, 600);
	renderer_->AddDrawer(worldRenderer_);
}

void Game::Start() {
	while (true) {
		renderer_->Draw();
		glfwPollEvents();
	}
}

void Game::SetupTempWorldState() {
	shared_ptr<Crate> crate = make_shared<Crate>();
	crate->position = glm::vec3(0.0f, 0.0f, 0.5f);
}

void Game::Stop() {}
