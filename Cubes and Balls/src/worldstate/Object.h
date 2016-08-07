#ifndef CUBES_AND_BALLS_SRC_WORDSTATE_OBJECT_H_
#define CUBES_AND_BALLS_SRC_WORDSTATE_OBJECT_H_

#include <string>
#include <mutex>

#include "glm/glm.hpp"

#include "../ModelLoader.h"
#include "Orientation.h"

struct Material {
	glm::vec4 color = glm::vec4(0, 0, 0, 1);
};

// This class and it's access functions are all thread-safe.
class Object :
	public Orientation {
public:
	Object() {};

	Object(const Object &toCopy);

	virtual Material GetMaterial() const;

	virtual std::shared_ptr<const Model> GetModel() const;

	virtual glm::vec3 GetPosition() const;

	virtual void SetPosition(const glm::vec3 &position);

	virtual void Move(const glm::vec3 &distance);

	glm::mat4 LocalToWorldSpaceMatrix() const;

	glm::vec3 ToLocalSpace(const glm::vec3 &worldSpacePosition, bool shouldTranslate) const;

	glm::vec3 ToWorldSpace(const glm::vec3 &localSpacePosition, bool shouldTranslate) const;

	virtual ~Object();
private:
	mutable std::mutex positionmtx_;
	glm::vec3 position_ = glm::vec3(0, 0, 0);
};

#endif