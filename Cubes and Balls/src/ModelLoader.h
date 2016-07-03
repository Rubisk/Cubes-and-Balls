#ifndef CUBES_AND_BALLS_SRC_RENDERER_MODELLOADER_H_
#define CUBES_AND_BALLS_SRC_RENDERER_MODELLOADER_H_

#include <map>
#include <vector>
#include <memory>

#include "GL/glew.h"


// Models are used to draw by the renderer, and also used for the physics engine to detect collision.
// For now, let's assume they share the same vertices.
// Every vertex is made of 6 coordinates, 3 representing the point and 3 representing the normal vertex for that face,
// which is necessary for lighting the objects.
struct Model {
	float minRadius = 0;
	float maxRadius = 0;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> elements;
};


class ModelLoader {
public:
	static std::shared_ptr<const Model> GetModel(const std::string &name);
private:
	static void TryLoadModel_(const std::string &name);

	static std::map<std::string, std::shared_ptr<const Model>> models_;
};

#endif