#ifndef CUBES_AND_BALLS_SRC_GAME_H_
#define CUBES_AND_BALLS_SRC_GAME_H_

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "worldstate/WorldState.h"
#include "renderer/Renderer.h"
#include "renderer/WorldRenderer.h"
#include "physics/PhysicsEngine.h"

// Handles creation and deletion of a renderer, world, IO/system, and
// is there to make sure the parts can communicate with each other.
class Game {
public:
	void Setup();

	void Start();

	void SetupTempWorldState();

	void Stop();
private:
	friend class InputHandler;

	std::shared_ptr<WorldState> worldState_;
	std::shared_ptr<Renderer> renderer_;
	std::shared_ptr<WorldRenderer> worldRenderer_;
	std::shared_ptr<PhysicsEngine> physics_;

	GLFWwindow* window_;
};

#endif