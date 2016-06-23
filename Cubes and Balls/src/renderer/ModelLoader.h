#ifndef CUBES_AND_BALLS_SRC_RENDERER_MODEL_LOADER_H_
#define CUBES_AND_BALLS_SRC_RENDERER_MODEL_LOADER_H_

#include <map>
#include <vector>


// Provides a Model for a renderer to draw.
// Coordinates are precisely the size of the entity the model represents.
struct Model {
	std::vector<GLfloat> vertices;
	std::vector<GLuint> elements;
};


class ModelLoader {
public:
	Model GetModel(const std::string &name);

	~ModelLoader();
private:
	std::map<std::string, Model> models_;
};

#endif