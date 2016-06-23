#ifndef CUBES_AND_BALLS_SRC_RENDERER_WORLD_RENDERER_H_
#define CUBES_AND_BALLS_SRC_RENDERER_WORLD_RENDERER_H_

#include <map>

#include "GL/glew.h"

#include "Drawer.h"
#include "ModelLoader.h"
#include "../worldstate/WorldState.h"


class WorldRenderer :
	public Drawer {
public:
	WorldRenderer(const std::weak_ptr<WorldState> &world);

	virtual void Draw();

	~WorldRenderer();
private:
	struct GLEntityState;

	GLEntityState GetGLEntityState_(const std::shared_ptr<Entity> &target);

	std::weak_ptr<WorldState> worldToRender_;

	std::map<std::string, std::unique_ptr<GLEntityState>> stateBuffer_;

	GLuint shaderProgram_;
	ModelLoader modelLoader_;
};

#endif