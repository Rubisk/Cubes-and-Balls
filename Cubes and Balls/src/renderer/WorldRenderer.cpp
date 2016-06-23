#include "WorldRenderer.h"

#include "glm/gtc/type_ptr.hpp"

#include "shaders/ShaderLoader.h"

using namespace std;
using namespace glm;

WorldRenderer::WorldRenderer(const weak_ptr<WorldState> &world) {
	worldToRender_ = world;
	shaderProgram_ = ShaderLoader::CreateShaderProgram("src/shaders/standard.vert",
													   "src/shaders/standard.frag");

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
		// TODO setup model matrix
		mat4 object;
		glUniformMatrix4fv(modelUni, 1, GL_FALSE, value_ptr(object));

		GLEntityState state = GetGLEntityState_(e);
		glBindVertexArray(state.vao);
		glDrawElements(state.mode, state.count, GL_UNSIGNED_INT, state.location);
		glBindVertexArray(0);
	}
}

WorldRenderer::~WorldRenderer() {
	glDeleteProgram(shaderProgram_);
}

struct WorldRenderer::GLEntityState {
	GLuint vao;

	GLuint mode;
	GLuint count;
	GLvoid* location; 
};

WorldRenderer::GLEntityState WorldRenderer::GetGLEntityState_(const shared_ptr<Entity> &target) {
	if (!(stateBuffer_.find(target->GetModelName()) != stateBuffer_.end())) {
		Model model = modelLoader_.GetModel(target->GetModelName());
		// TODO load model data into a VAO and create a GLEntityState.
		GLEntityState state;
		stateBuffer_[target->GetModelName()] = make_unique<GLEntityState>(state);
	}
	return *stateBuffer_[target->GetModelName()];
}
