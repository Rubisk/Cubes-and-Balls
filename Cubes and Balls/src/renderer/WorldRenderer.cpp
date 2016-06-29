#include "WorldRenderer.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include "glm/gtx/io.hpp"

#include "shaders/ShaderLoader.h"

using namespace std;
using namespace glm;


WorldRenderer::WorldRenderer(const weak_ptr<WorldState> &world, float screenWidth, float screenHeight) {
	worldToRender_ = world;
	screenHeight_ = screenHeight;
	screenWidth_ = screenWidth;
	shaderProgram_ = ShaderLoader::CreateShaderProgram("./src/renderer/shaders/standard.vert",
													   "./src/renderer/shaders/standard.frag");

}

void WorldRenderer::Draw() {
	if (worldToRender_.expired()) return;

	shared_ptr<WorldState> worldToRender = worldToRender_.lock();

	glUseProgram(shaderProgram_);
	GLuint modelUni = glGetUniformLocation(shaderProgram_, "model");
	GLuint cameraUni = glGetUniformLocation(shaderProgram_, "camera");
	GLuint projectionUni = glGetUniformLocation(shaderProgram_, "projection");

	mat4 cameraMatrix = lookAt(worldToRender->camera.GetPosition(),
							   worldToRender->camera.GetPosition() + worldToRender->camera.GetOrientation().frontSide,
							   worldToRender->camera.GetOrientation().upSide);

	mat4 projectionMatrix = perspective(pi<float>() / 2, screenWidth_ / screenHeight_, 0.1f, 100.0f);
	glUniformMatrix4fv(cameraUni, 1, GL_FALSE, value_ptr(cameraMatrix));
	glUniformMatrix4fv(projectionUni, 1, GL_FALSE, value_ptr(projectionMatrix));

	for (shared_ptr<Entity> e : worldToRender->GetEntities()) {
		// TODO setup object matrix
		mat4 object = translate(mat4(), e->position);
		glUniformMatrix4fv(modelUni, 1, GL_FALSE, value_ptr(object));
		modelRenderer_.DrawModel(e->GetModelName());
	}
}

WorldRenderer::~WorldRenderer() {
	glDeleteProgram(shaderProgram_);
}

