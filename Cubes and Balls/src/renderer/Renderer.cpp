#include "Renderer.h"

#include "glm/gtc/type_ptr.hpp"

#include "shaders/ShaderLoader.h"

Renderer::Renderer(size_t screenWidth, size_t screenHeight) {
	screenWidth_ = screenWidth;
	screenHeight_ = screenHeight;

	glewExperimental = GL_TRUE;
	glewInit();

	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	shaderProgram_ = ShaderLoader::CreateShaderProgram("src/shaders/standard.vert",
													   "src/shaders/standard.frag");
}

Renderer::~Renderer() {
	glDeleteProgram(shaderProgram_);
}

void Renderer::Draw() {
	glUseProgram(shaderProgram_);
	GLuint cameraUni = glGetUniformLocation(shaderProgram_, "camera");
	GLuint projectionUni = glGetUniformLocation(shaderProgram_, "projection");
	glUniformMatrix4fv(cameraUni, 1, GL_FALSE, glm::value_ptr(cameraMatrix_));
	glUniformMatrix4fv(projectionUni, 1, GL_FALSE, glm::value_ptr(projectionMatrix_));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backGround_.r, backGround_.g, backGround_.b, backGround_.a);

	for (std::shared_ptr<Drawer> drawer : drawers_) drawer->Draw();
}

void Renderer::AddDrawer(const std::shared_ptr<Drawer> &drawer) {
	drawers_.push_front(drawer);
}

void Renderer::RemoveDrawer(const std::shared_ptr<Drawer> &drawer) {
	drawers_.remove(drawer);
}

void Renderer::SetBackGround(const glm::vec4 &backGround) {
	backGround_ = backGround;
}
