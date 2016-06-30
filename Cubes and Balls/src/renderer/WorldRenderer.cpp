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

	mat4 cameraMatrix = inverse(worldToRender->camera.GetViewToWorldMatrix());
	mat4 projectionMatrix = perspective(pi<float>() / 3, screenWidth_ / screenHeight_, 0.1f, 100.0f);

	glUniformMatrix4fv(cameraUni, 1, GL_FALSE, value_ptr(cameraMatrix));
	glUniformMatrix4fv(projectionUni, 1, GL_FALSE, value_ptr(projectionMatrix));

	for (shared_ptr<Entity> e : worldToRender->GetEntities()) {
		// TODO setup object matrix
		vec3 up = e->orientation.GetUpSide();
		vec3 front = e->orientation.GetFrontSide();
		assert(up == normalize(up));
		mat4 object = mat4(vec4(normalize(cross(up, front)), 0),
						   vec4(up, 0),
						   vec4(front, 0),
						   vec4(0, 0, 0, 1));
		object = translate(object, e->position);
		glUniformMatrix4fv(modelUni, 1, GL_FALSE, value_ptr(object));
		modelRenderer_.DrawModel(e->GetModelName());
	}
}

WorldRenderer::~WorldRenderer() {
	glDeleteProgram(shaderProgram_);
}

