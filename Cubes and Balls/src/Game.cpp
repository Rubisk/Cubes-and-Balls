#include "Game.h"

#include "glm/gtc/constants.hpp"

#include "InputHandler.h"
#include "worldstate/WorldState.h"
#include "worldstate/Crate.h"
#include "renderer/Renderer.h"
#include "renderer/WorldRenderer.h"
#include "physics/PhysicsEngine.h"
#include "physics/ForceApplier.h"

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
	physics_ = make_shared<PhysicsEngine>();
	physics_->Start(worldState_, 60);
	playerController = make_shared<PlayerController>(worldState_->player);
	physics_->RegisterForceGenerator(playerController);

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
	Stop();
}

void Game::SetupTempWorldState() {
	shared_ptr<Crate> crate = make_shared<Crate>();
	shared_ptr<Crate> crate2 = make_shared<Crate>();
	crate->SetPosition(vec3(0.0, 1.0f, 0.0f));
	crate2->SetPosition(vec3(0.0, -0.5f, 0.45f));
	crate2->SetSpeed(vec3(0, 5.5f, 0));
	worldState_->AddEntity(crate);
	worldState_->AddEntity(crate2);
	worldState_->player->SetPosition(vec3(2, 0.25f, 0));
	worldState_->player->LookAt(vec3(0, 0.25f, 0));
}

void Game::Stop() {
	physics_->Stop();
}
