// This class implements checking collisions between models.
// Collision detections has quite a few steps to it, so here's an overview of how the algorithms work.
// To check if an object is colliding, we go over all nearby objects, and check if the object and the nearby object are colliding.
// To check if 2 objects are colliding, we start with a box surrounding both objects.
// We then keep shrinking the box in one direction to the smallest possible box surrounding object 1,
// and then shrink it in one direction to the smallest possible box surrounding object 2.
// If the objects are colliding, this process will stop and we'll get a nice box around the are
// that they are colliding in, otherwise the box will shrink to a box with dimensions 0.

#include "CollisionDetector.h"


using namespace std;
using namespace glm;

namespace {

struct Box {
	vec3 min;
	vec3 max;
};

vec3 SwapAxi(const vec3 &target, int newZ) {
	switch (newZ) {
	case 0:
		return vec3(target.y, target.z, target.x);
	case 1:
		return vec3(target.x, target.z, target.y);
	case 2:
		return vec3(target);
	default:
		throw std::runtime_error("Axis should be in (0, 1, 2).");
	}
}

// Use this to find the value of a model face at a 2d point.
// Returns false if the face doesn't have any value there, and true otherwise.
// If the face has a value at that point, it writes the value to the corresponding input float.
// The other 2 floats are used to specify the point to look at.
bool GetValueAtPoint(const vec3 &v1, const vec3 &v2, const vec3 &v3, float x, float y, float &outputZ) {
	// We know that any point in the face can be written as a*v1 + b*v2 + c*v3, with a + b + c = 1.
	// This gives us a set of linear equations we can solve.
	mat3 system = {1, 1, 1,
		v1.x, v2.x, v3.x,
		v1.y, v2.y, v3.y};
	if (determinant(system) == 0) return false;
	mat3 solution = inverse(system);
	vec3 proportions = solution * vec3(1, x, y);
	if (proportions.x < 0 || proportions.y < 0 || proportions.z < 0) return false;
	outputZ = (proportions[0] * v1 + proportions[1] * v2 + proportions[2] * v3).z;
	return true;
}

// Finds the intersection of the lines (x1, x2) and (y1, y2) if there is one,
// then acts like GetValueAtPoint. Returns false if there is no intersection.
bool GetValueAtIntersection(const vec3 &v1, const vec3 &v2, const vec3 &v3,
							const vec2 &x1, const vec2 &x2, const vec2 &y1, const vec2 &y2,
							float &outputZ) {
	mat2 system = {x2.x - x1.x, y1.x - y2.x,
				   x2.y - x1.y, y1.y - y1.y};
	if (determinant(system) == 0) return false;
	vec2 solution = inverse(system) * vec2(y1.x - x1.x, y1.y - x1.y);

	if (solution[0] < 0 || solution[0] > 1 || solution[1] < 0 || solution[1] > 1) return false; // No intersection
	vec2 intersection = x1 + solution[0] * x2;
	return GetValueAtPoint(v1, v2, v3, intersection.x, intersection.y, outputZ);
}

// Wraps a box around an object, as small as possible.
void WrapBox(shared_ptr<Object> target, Box &outputBox) {
	shared_ptr<const Model> model = target->GetModel();
	mat4 transform = target->LocalToWorldSpaceMatrix();

	vec3 v1, v2, v3;
	Box output;
	// If the object's minimum is higher than the original boxes maximum, 
	// the box gets shrinked to nothing. Same for maximum.
	output.min = outputBox.max;
	output.max = outputBox.min;

	// Go over all vertices, and get their min/max x, y, z values.
	// Then take the minimum over all minima, and maximum over all maxima.
	std::vector<GLuint>::const_iterator it = model->elements.begin();
	while (it != model->elements.end()) {
		// Load 3 vertices from model data representing the face.
		v1 = vec3(model->vertices[*it], model->vertices[*it + 1], model->vertices[*it + 2]);
		it++;
		v2 = vec3(model->vertices[*it], model->vertices[*it + 1], model->vertices[*it + 2]);
		it++;
		v3 = vec3(model->vertices[*it], model->vertices[*it + 1], model->vertices[*it + 2]);
		it++;

		// The vertices in model are in local space, but the box is in world space.
		v1 = vec3(transform * vec4(v1, 1));
		v2 = vec3(transform * vec4(v2, 1));
		v3 = vec3(transform * vec4(v3, 1));

		for (int axis = 0; axis < 3; axis++) {
			// Swap axi so we don't have to worry about changing our x/y/z per direction we're looking at.
			vec3 swappedv1 = SwapAxi(v1, axis);
			vec3 swappedv2 = SwapAxi(v2, axis);
			vec3 swappedv3 = SwapAxi(v3, axis);

			vec3 boxMin = SwapAxi(outputBox.min, axis);
			vec3 boxMax = SwapAxi(outputBox.max, axis);

			// We know from calculus that the minimum/maximum z is at either:
			// A corner of the face
			// A corner of the (x, y) square we're looking in.
			// An intersection of the face edge with the square edge.

			float outputZ;
			// Face corners.
			for (vec3 faceCorner : {swappedv1, swappedv2, swappedv3}) {
				if (faceCorner.x > boxMax.x || faceCorner.x < boxMin.x ||
					faceCorner.y > boxMax.y || faceCorner.y < boxMin.y) continue;
				if (GetValueAtPoint(v1, v2, v3, faceCorner.x, faceCorner.y, outputZ)) {
					if (outputZ < output.min[axis]) output.min[axis] = outputZ;
					if (outputZ > output.max[axis]) output.min[axis] = outputZ;
				}
			}

			// Box corners.
			for (float x : {boxMin.x, boxMax.x}) {
				for (float y : {boxMin.y, boxMax.y}) {
					if (GetValueAtPoint(v1, v2, v3, x, y, outputZ)) {
						if (outputZ < output.min[axis]) output.min[axis] = outputZ;
						if (outputZ > output.max[axis]) output.min[axis] = outputZ;
					}
				}
			}

			// Intersections
			for (vec3 corner1 : {swappedv1, swappedv2, swappedv3}) {
				for (vec3 corner2 : {swappedv1, swappedv2, swappedv3}) {
					if (vec2(corner1) == vec2(corner2)) continue;
					// Seperate code for each of the 4 box-edges since those are to ugly to iterate on.
					if (GetValueAtIntersection(swappedv1, swappedv2, swappedv3, vec2(corner1), vec2(corner2),
											   vec2(boxMin), vec2(boxMin.x, boxMax.y), outputZ)) {
						if (outputZ < output.min[axis]) output.min[axis] = outputZ;
						if (outputZ > output.max[axis]) output.min[axis] = outputZ;
					}
					if (GetValueAtIntersection(swappedv1, swappedv2, swappedv3, vec2(corner1), vec2(corner2),
											   vec2(boxMin), vec2(boxMax.x, boxMin.y), outputZ)) {
						if (outputZ < output.min[axis]) output.min[axis] = outputZ;
						if (outputZ > output.max[axis]) output.min[axis] = outputZ;
					}
					if (GetValueAtIntersection(swappedv1, swappedv2, swappedv3, vec2(corner1), vec2(corner2),
											   vec2(boxMax), vec2(boxMin.x, boxMax.y), outputZ)) {
						if (outputZ < output.min[axis]) output.min[axis] = outputZ;
						if (outputZ > output.max[axis]) output.min[axis] = outputZ;
					}
					if (GetValueAtIntersection(swappedv1, swappedv2, swappedv3, vec2(corner1), vec2(corner2),
											   vec2(boxMax), vec2(boxMax.x, boxMin.y), outputZ)) {
						if (outputZ < output.min[axis]) output.min[axis] = outputZ;
						if (outputZ > output.max[axis]) output.min[axis] = outputZ;
					}
				}
			}
		}
	}
	outputBox = output;
}

}

void CollisionDetector::SetWorldState(shared_ptr<WorldState> world) {
	world_ = world;
}

bool CollisionDetector::IsColliding(shared_ptr<Object> toTest, vec3 &outputCenterOfCollision, shared_ptr<Object> &outputCollider) {
	if (world_ == nullptr) return false;
	vec3 centerOfCollision;
	for (shared_ptr<Object> object : world_->GetObjects()) {
		if (Colliding(object, toTest, centerOfCollision)) {
			outputCollider = object;
			outputCenterOfCollision = centerOfCollision;
			return true;
		}
	}
	return false;
}

bool CollisionDetector::Colliding(shared_ptr<Object> first, shared_ptr<Object> second, vec3 &outputCenterOfCollision) {
	Box box;
	box.min = vec3(-10, -10, -10); // TODO setup proper min/maxing for object size.
	box.max = vec3(10, 10, 10);
	while (box.min.x < box.max.x && box.min.y < box.max.y && box.min.z < box.max.z) {
		Box newBox = box;
		WrapBox(first, newBox);
		WrapBox(second, newBox);
		if (newBox.min == box.min && newBox.max == box.max) {
			outputCenterOfCollision = 0.5f * (box.min + box.max);
			return true;
		}
	}
	return false;
}
