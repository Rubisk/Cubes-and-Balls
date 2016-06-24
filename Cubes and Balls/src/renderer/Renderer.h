#ifndef CUBES_AND_BALLS_SRC_RENDERER_RENDERER_H_
#define CUBES_AND_BALLS_SRC_RENDERER_RENDERER_H_

#include <memory>
#include <forward_list>

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "Drawer.h"


// Main object that handles drawing onto the screen. Assumes a valid OpenGL context is
// setup for the thread upon creation. To draw, create a Drawer object and implement 
// it's Draw function, then add it to the renderer.
// The renderer object controls the camera and projection transformations, and the shader program. 
// (i.e., drawers should
// only modify object space and world space.)
class Renderer {
public:
	Renderer(size_t screenWidth, size_t screenHeight);

	~Renderer();

	void Draw();

	void AddDrawer(const std::shared_ptr<Drawer> &drawer);

	void RemoveDrawer(const std::shared_ptr<Drawer> &drawer);

	void SetBackGround(const glm::vec4 &backGround);
private:
	size_t screenWidth_;
	size_t screenHeight_;

	glm::vec4 backGround_{0, 1, 0, 0};
	std::forward_list<std::shared_ptr<Drawer>> drawers_;
};

#endif