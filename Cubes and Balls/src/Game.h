#ifndef CUBES_AND_BALLS_SRC_GAME_H_
#define CUBES_AND_BALLS_SRC_GAME_H_

#include "GL/glew.h"
#include "GLFW/glfw3.h"

// Handles creation and deletion of a renderer, world, IO/system, and
// is there to make sure the parts can communicate with each other.
class Game {
public:
	void Start();

	void Stop();
private:
	GLFWwindow* window_;
};

#endif