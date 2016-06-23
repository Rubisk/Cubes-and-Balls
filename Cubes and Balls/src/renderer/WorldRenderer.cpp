#include "WorldRenderer.h"

#include "glm/gtc/type_ptr.hpp"

#include "shaders/ShaderLoader.h"

using namespace std;
using namespace glm;

WorldRenderer::WorldRenderer(weak_ptr<World> world, weak_ptr<Camera> camera) {
	world_ = world;
	camera_ = camera;
	shaderProgram_ = ShaderLoader::CreateShaderProgram("src/shaders/standard.vert",
													   "src/shaders/standard.frag");

}

void WorldRenderer::Draw() {
	glUseProgram(shaderProgram_);
	GLuint cameraUni = glGetUniformLocation(shaderProgram_, "camera");
	GLuint projectionUni = glGetUniformLocation(shaderProgram_, "projection");
	mat4 cameraMatrix_;
	mat4 projectionMatrix_;
	glUniformMatrix4fv(cameraUni, 1, GL_FALSE, value_ptr(cameraMatrix_));
	glUniformMatrix4fv(projectionUni, 1, GL_FALSE, value_ptr(projectionMatrix_));
}

WorldRenderer::~WorldRenderer() {
	glDeleteProgram(shaderProgram_);
}
