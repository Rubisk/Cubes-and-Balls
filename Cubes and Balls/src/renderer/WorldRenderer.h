#ifndef CUBES_AND_BALLS_SRC_RENDERER_WORLD_RENDERER_H_
#define CUBES_AND_BALLS_SRC_RENDERER_WORLD_RENDERER_H_

#include "GL/glew.h"

#include "Drawer.h"
#include "../worldstate/WorldState.h"


class WorldRenderer :
	public Drawer {
public:
	WorldRenderer(std::weak_ptr<WorldState> world);

	virtual void Draw();

	~WorldRenderer();
private:
	std::weak_ptr<WorldState> worldState_;

	GLuint shaderProgram_;
};

#endif