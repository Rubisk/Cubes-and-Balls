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
	mat4 cameraMatrix_; // = lookAt(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0));
	mat4 projectionMatrix_ = perspective(75.0f, screenWidth_ / screenHeight_, 0.1f, 100.0f);
	glUniformMatrix4fv(cameraUni, 1, GL_FALSE, value_ptr(cameraMatrix_));
	glUniformMatrix4fv(projectionUni, 1, GL_FALSE, value_ptr(projectionMatrix_));

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

