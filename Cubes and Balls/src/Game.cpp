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
	shared_ptr<Crate> crate2 = make_shared<Crate>();
	crate->SetPosition(vec3(1.0, 0.0f, 0.0f));
	crate2->SetPosition(vec3(0.5f, 0.2f, 0.3f));
	crate2->Rotate(vec3(0, 0, 1), 3.1415f / 5.0f);
	crate2->Rotate(vec3(0, 1, 0), 3.1415f / 5.0f);
	worldState_->AddEntity(crate);
	worldState_->AddEntity(crate2);
	worldState_->camera.SetPosition(vec3(0, 0, 0));
	worldState_->camera.LookAt(vec3(1, 0, 0));
}

void Game::Stop() {}
