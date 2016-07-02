#ifndef CUBES_AND_BALLS_SRC_RENDERER_MODELRENDERER_H_
#define CUBES_AND_BALLS_SRC_RENDERER_MODELRENDERER_H_

#include <map>
#include <queue>
#include <memory>

#include "../worldstate/Entity.h"
#include "../ModelLoader.h"

class ModelRenderer {
public:
	void DrawModel(const std::string &modelName);

	~ModelRenderer();
private:
	// Struct used to "store" a GL setup for a particular model.
	// Holds a Vertex Array Object with the vertices for the model and some information
	// needed to call glDrawElements
	struct GLModelState {
		GLuint vao;

		GLuint mode;
		GLuint count;
		GLvoid* location;
	};

	GLModelState GetGLModelState_(const std::string &modelName);
	std::map<std::string, GLModelState> stateBuffer_;
	ModelLoader modelLoader_;

	std::queue<GLuint> buffersToDelete_;
};

#endif