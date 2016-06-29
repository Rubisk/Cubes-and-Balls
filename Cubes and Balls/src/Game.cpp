#include "Game.h"
#include "InputHandler.h"

#include "worldstate/WorldState.h"
#include "worldstate/Crate.h"
#include "renderer/Renderer.h"
#include "renderer/WorldRenderer.h"

using namespace std;
using namespace glm;

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
	worldRenderer_ = make_shared<WorldRenderer>(worldState_, 800.0f, 600.0f);
	renderer_ = make_shared<Renderer>(800, 600);
	renderer_->AddDrawer(worldRenderer_);

	glfwSetWindowUserPointer(window_, this);
	
	glfwSetKeyCallback(window_, static_cast<GLFWkeyfun>(InputHandler::KeyInput));
	glfwSetMouseButtonCallback(window_, static_cast<GLFWmousebuttonfun>(InputHandler::MouseButtonInput));
}

void Game::Start() {
	while (!glfwWindowShouldClose(window_)) {
		renderer_->Draw();
		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
}

void Game::SetupTempWorldState() {
	shared_ptr<Crate> crate = make_shared<Crate>();
	crate->position = vec3(20.0f, 20.0f, 20.0f);
	worldState_->AddEntity(crate);
	worldState_->camera.GoTo(vec3(0, 0, 0));
	worldState_->camera.LookAt(vec3(20, 20, 20));
}

void Game::Stop() {}
