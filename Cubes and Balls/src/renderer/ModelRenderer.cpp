#include "GL/glew.h"

#include "ModelRenderer.h"

using namespace std;

void ModelRenderer::DrawModel(const string &modelName) {
	GLModelState state = GetGLModelState_(modelName);
	glBindVertexArray(state.vao);
	glDrawElements(state.mode, state.count, GL_UNSIGNED_INT, state.location);
	glBindVertexArray(0);
}

ModelRenderer::~ModelRenderer() {
	// Clean up VAO's in GLModelStates.
	for (std::map<std::string, GLModelState>::iterator mapIt = stateBuffer_.begin();
		 mapIt != stateBuffer_.end(); mapIt++) {
		glDeleteVertexArrays(1, &mapIt->second.vao);
	}

	// Then delete opengl vertex/element buffers used to store model data.
	while (!buffersToDelete_.empty()) {
		GLuint bufferToDelete = buffersToDelete_.front();
		glDeleteBuffers(1, &bufferToDelete);
		buffersToDelete_.pop();
	}
}

ModelRenderer::GLModelState ModelRenderer::GetGLModelState_(const string &target) {
	if (stateBuffer_.find(target) == stateBuffer_.end()) {
		Model model = modelLoader_.GetModel(target);
		GLModelState state;
		GLuint vertexBuffer;
		GLuint elementBuffer;

		state.count = model.elements.size();
		state.location = 0;
		state.mode = GL_TRIANGLES;

		if (model.vertices.size() == 0) return state;

		glGenVertexArrays(1, &state.vao);
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &elementBuffer);

		buffersToDelete_.push(vertexBuffer);
		buffersToDelete_.push(elementBuffer);

		glBindVertexArray(state.vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.elements.size() * sizeof(GLuint), &model.elements[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(GLfloat), &model.vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*) 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*) 3);

		glBindVertexArray(0);

		stateBuffer_[target] = state;

	}
	return  stateBuffer_[target];
}
