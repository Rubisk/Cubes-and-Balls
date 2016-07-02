#ifndef CUBES_AND_BALLS_SRC_RENDERER_SHADERS_SHADERLOADER_H_
#define CUBES_AND_BALLS_SRC_RENDERER_SHADERS_SHADERLOADER_H_

#include "GL/glew.h"

class ShaderLoader {
public:
	// Loads a vertex and fragment shader from vertexPath and fragmentPath
	// and returns a GL shader program location.
	// Note that responsibility for cleaning up the program is by the object calling
	// this function.
	static GLuint CreateShaderProgram(const std::string vertexPath, const std::string fragmentPath);
};

#endif