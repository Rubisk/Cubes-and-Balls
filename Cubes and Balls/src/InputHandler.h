#ifndef CUBES_AND_BALLS_SRC_INPUTHANDLER_H_
#define CUBES_AND_BALLS_SRC_INPUTHANDLER_H_

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class InputHandler {
public:
	static void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void MouseButtonInput(GLFWwindow* window, int button, int action, int mods);
};

#endif