#include "Renderer.h"

Renderer::Renderer(size_t screenWidth, size_t screenHeight) {
	screenWidth_ = screenWidth;
	screenHeight_ = screenHeight;

	glewExperimental = GL_TRUE;
	glewInit();

	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
}

Renderer::~Renderer() {
}

void Renderer::Draw() {
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
