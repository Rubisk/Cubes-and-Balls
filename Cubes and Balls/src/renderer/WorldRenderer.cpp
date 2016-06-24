#include "WorldRenderer.h"

#include "glm/gtc/type_ptr.hpp"

#include "shaders/ShaderLoader.h"

using namespace std;
using namespace glm;

#include <fstream>
WorldRenderer::WorldRenderer(const weak_ptr<WorldState> &world) {
	worldToRender_ = world;
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
	mat4 cameraMatrix_;
	mat4 projectionMatrix_;
	glUniformMatrix4fv(cameraUni, 1, GL_FALSE, value_ptr(cameraMatrix_));
	glUniformMatrix4fv(projectionUni, 1, GL_FALSE, value_ptr(projectionMatrix_));

	for (shared_ptr<Entity> e : worldToRender->GetEntities()) {
		// TODO setup object matrix
		mat4 object;
		glUniformMatrix4fv(modelUni, 1, GL_FALSE, value_ptr(object));
		modelRenderer_.DrawModel(e->GetModelName());
	}
}

WorldRenderer::~WorldRenderer() {
	glDeleteProgram(shaderProgram_);
}

