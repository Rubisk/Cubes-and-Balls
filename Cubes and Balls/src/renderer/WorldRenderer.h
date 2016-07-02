#ifndef CUBES_AND_BALLS_SRC_RENDERER_WORLDRENDERER_H_
#define CUBES_AND_BALLS_SRC_RENDERER_WORLDRENDERER_H_

#include <map>

#include "GL/glew.h"

#include "Drawer.h"
#include "ModelRenderer.h"
#include "../worldstate/WorldState.h"


class WorldRenderer :
	public Drawer {
public:
	WorldRenderer(const std::weak_ptr<WorldState> &world, float screenWidth, float screenHeight);

	virtual void Draw();

	~WorldRenderer();
private:
	ModelRenderer modelRenderer_;
	std::weak_ptr<WorldState> worldToRender_;
	GLuint shaderProgram_;

	float screenWidth_, screenHeight_;
};

#endif