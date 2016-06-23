#ifndef CUBES_AND_BALLS_SRC_RENDERER_WORLD_RENDERER_H_
#define CUBES_AND_BALLS_SRC_RENDERER_WORLD_RENDERER_H_

#include "GL/glew.h"

#include "Drawer.h"
#include "../world/World.h"

class Camera; // TODO make a camera class

class WorldRenderer :
	public Drawer {
public:
	WorldRenderer(std::weak_ptr<World> world, std::weak_ptr<Camera> camera);

	virtual void Draw();

	~WorldRenderer();
private:
	std::weak_ptr<World> world_;
	std::weak_ptr<Camera> camera_;

	GLuint shaderProgram_;
};

#endif