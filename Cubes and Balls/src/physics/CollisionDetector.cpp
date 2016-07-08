// This class implements checking collisions between models.
// Collision detections has quite a few steps to it, so here's an overview of how the algorithms work.
// To check if an object is colliding, we go over all nearby objects, and check if the object and the nearby object are colliding.
// To check if 2 objects are colliding, we start with a box surrounding both objects.
// We then keep shrinking the box in one direction to the smallest possible box surrounding object 1,
// and then shrink it in one direction to the smallest possible box surrounding object 2.
// If the objects are colliding, this process will stop and we'll get a nice box around the are
// that they are colliding in, otherwise the box will shrink to a box with dimensions 0.

#include "CollisionDetector.h"

#include <chrono>
#include <iostream>
#include "glm/gtx/io.hpp"

using namespace std;
using namespace glm;

namespace {

struct Box {
	vec3 min;
	vec3 max;
};

struct Face {
	vec3 p1;
	vec3 p2;
	vec3 p3;
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

// Load 3 vertices from model data representing the face.
Face GetFacePositionVerticesFromModel(shared_ptr<const Model> model, int faceNumber, const mat4 &transform) {
	Face face;
	// TODO introduce proper vertex data size constants etc.
	int faceIndex = faceNumber * 3;

	int v1Pos = model->elements[faceIndex] * 6;
	int v2Pos = model->elements[faceIndex + 1] * 6;
	int v3Pos = model->elements[faceIndex + 2] * 6;

	face.p1 = vec3(model->vertices[v1Pos], model->vertices[v1Pos + 1], model->vertices[v1Pos + 2]);
	face.p2 = vec3(model->vertices[v2Pos], model->vertices[v2Pos + 1], model->vertices[v2Pos + 2]);
	face.p3 = vec3(model->vertices[v3Pos], model->vertices[v3Pos + 1], model->vertices[v3Pos + 2]);

	face.p1 = vec3(transform * vec4(face.p1, 1));
	face.p2 = vec3(transform * vec4(face.p2, 1));
	face.p3 = vec3(transform * vec4(face.p3, 1));

	return face;
}

// Loads the normal data from a face of a model.
vec3 GetFaceNormalFromModel(shared_ptr<const Model> model, int faceNumber) {
	int faceIndex = faceNumber * 3;
	int pos = model->elements[faceIndex] * 6 + 3;
	return vec3(model->vertices[pos], model->vertices[pos + 1], model->vertices[pos + 2]);
}

// Given a model, and a bool array of the faces colliding, 
// returns the direction of impact of the collision (inverted interpolation of normals of faces).
vec3 GetCollisionDirection(shared_ptr<const Model> model, const vector<bool> &faces) {
	vec3 normalSum;
	for (int i = 0; i < faces.size(); i++) {
		if (!faces[i]) continue;
		vec3 faceNormal = GetFaceNormalFromModel(model, i);
		normalSum += faceNormal;
	}
	return -normalize(normalSum);
}

// Use this to find the value of a model face at a 2d point.
// Returns false if the face doesn't have any value there, and true otherwise.
// If the face has a value at that point, it writes the value to the corresponding input float.
// The other 2 floats are used to specify the point to look at.
bool GetValueAtPoint(const vec3 &v1, const vec3 &v2, const vec3 &v3, float x, float y, float &outputZ) {
	// We know that any point in the face can be written as a*v1 + b*v2 + c*v3, with a + b + c = 1.
	// This gives us a set of linear equations we can solve.
	mat3 system = transpose(mat3{1, 1, 1,
		v1.x, v2.x, v3.x,
		v1.y, v2.y, v3.y});
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
				   x2.y - x1.y, y1.y - y2.y};
	if (determinant(system) == 0) return false;
	vec2 solution = inverse(system) * vec2(y1.x - x1.x, y1.y - x1.y);

	if (solution[0] < 0 || solution[0] > 1 || solution[1] < 0 || solution[1] > 1) return false; // No intersection
	vec2 intersection = x1 + solution[0] * x2;
	return GetValueAtPoint(v1, v2, v3, intersection.x, intersection.y, outputZ);
}

// Expands boxToExpand so the portion of the face that is within bounds all fits in boxToExpand.
// Returns true if there is any bit of face in bounds, and false otherwise.
bool ExpandBoxToFace(const Face &face, const Box &bounds, Box &boxToExpand) {
	bool inBox[3];
	for (int axis = 0; axis < 3; axis++) {
		// Swap axi so we don't have to worry about changing our x/y/z per direction we're looking at.
		vec3 swappedv1 = SwapAxi(face.p1, axis);
		vec3 swappedv2 = SwapAxi(face.p2, axis);
		vec3 swappedv3 = SwapAxi(face.p3, axis);

		vec3 boxMin = SwapAxi(bounds.min, axis);
		vec3 boxMax = SwapAxi(bounds.max, axis);

		// We know from calculus that the minimum/maximum z is at either:
		// A corner of the face
		// A corner of the (x, y) square we're looking in.
		// An intersection of the face edge with the square edge.

		float outputZ;
		inBox[axis] = false;
		// Face corners.
		for (vec3 faceCorner : {swappedv1, swappedv2, swappedv3}) {
			if (faceCorner.x > boxMax.x || faceCorner.x < boxMin.x ||
				faceCorner.y > boxMax.y || faceCorner.y < boxMin.y) continue;
			inBox[axis] = true;
			outputZ = faceCorner.z;
			if (outputZ < boxToExpand.min[axis]) boxToExpand.min[axis] = outputZ;
			if (outputZ > boxToExpand.max[axis]) boxToExpand.max[axis] = outputZ;
		}

		// Box corners.
		for (float x : {boxMin.x, boxMax.x}) {
			for (float y : {boxMin.y, boxMax.y}) {
				if (GetValueAtPoint(swappedv1, swappedv2, swappedv3, x, y, outputZ)) {
					inBox[axis] = true;
					if (outputZ < boxToExpand.min[axis]) boxToExpand.min[axis] = outputZ;
					if (outputZ > boxToExpand.max[axis]) boxToExpand.max[axis] = outputZ;
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
					inBox[axis] = true;
					if (outputZ < boxToExpand.min[axis]) boxToExpand.min[axis] = outputZ;
					if (outputZ > boxToExpand.max[axis]) boxToExpand.max[axis] = outputZ;
				}
				if (GetValueAtIntersection(swappedv1, swappedv2, swappedv3, vec2(corner1), vec2(corner2),
										   vec2(boxMin), vec2(boxMax.x, boxMin.y), outputZ)) {
					inBox[axis] = true;
					if (outputZ < boxToExpand.min[axis]) boxToExpand.min[axis] = outputZ;
					if (outputZ > boxToExpand.max[axis]) boxToExpand.max[axis] = outputZ;
				}
				if (GetValueAtIntersection(swappedv1, swappedv2, swappedv3, vec2(corner1), vec2(corner2),
										   vec2(boxMax), vec2(boxMin.x, boxMax.y), outputZ)) {
					inBox[axis] = true;
					if (outputZ < boxToExpand.min[axis]) boxToExpand.min[axis] = outputZ;
					if (outputZ > boxToExpand.max[axis]) boxToExpand.max[axis] = outputZ;
				}
				if (GetValueAtIntersection(swappedv1, swappedv2, swappedv3, vec2(corner1), vec2(corner2),
										   vec2(boxMax), vec2(boxMax.x, boxMin.y), outputZ)) {
					inBox[axis] = true;
					if (outputZ < boxToExpand.min[axis]) boxToExpand.min[axis] = outputZ;
					if (outputZ > boxToExpand.max[axis]) boxToExpand.max[axis] = outputZ;
				}
			}
		}
	}
	return inBox[0] && inBox[1] && inBox[2];
}

} // namespace

void CollisionDetector::SetWorldState(shared_ptr<WorldState> world) {
	world_ = world;
}

bool CollisionDetector::IsCollidingQ(std::shared_ptr<Object> toTest, Collision &outputCollission) {
	if (world_ == nullptr) return false;
	Collision outputCollission_;
	for (shared_ptr<Object> object : world_->GetObjectsInSphere(toTest->GetPosition(), toTest->GetModel()->maxRadius)) {
		if (object == toTest) continue;
		if (CollidingQ(object, toTest, outputCollission_)) {
			outputCollission = outputCollission_;
			return true;
		}
	}
	return false;
}

bool CollisionDetector::CollidingQ(shared_ptr<Object> first, shared_ptr<Object> second, Collision &outputCollission) {
	Box box;
	box.min = vec3(-10, -10, -10); // TODO setup proper min/maxing for object size.
	box.max = vec3(10, 10, 10);

	shared_ptr<const Model> firstModel = first->GetModel();
	shared_ptr<const Model> secondModel = second->GetModel();
	mat4 firstTransform = first->LocalToWorldSpaceMatrix();
	mat4 secondTransform = second->LocalToWorldSpaceMatrix();
	size_t firstModelNumberOfFaces = firstModel->elements.size() / 3;
	size_t secondModelNumberOfFaces = secondModel->elements.size() / 3;
	vector<bool> firstFacesInBounds = vector<bool>(firstModelNumberOfFaces, true);
	vector<bool> secondFacesInBounds = vector<bool>(secondModelNumberOfFaces, true);

	while (box.min.x < box.max.x && box.min.y < box.max.y && box.min.z < box.max.z) {
		// Create 2 boxes wrapping around first and second object, looking only in box.
		Box firstBox;
		firstBox.min = box.max;
		firstBox.max = box.min;
		for (int i = 0; i < firstModelNumberOfFaces; i++) {
			if (!firstFacesInBounds[i]) continue;
			Face face = GetFacePositionVerticesFromModel(firstModel, i, firstTransform);
			firstFacesInBounds[i] = ExpandBoxToFace(face, box, firstBox);
		}

		Box secondBox;
		secondBox.min = box.max;
		secondBox.max = box.min;
		for (int i = 0; i < secondModelNumberOfFaces ; i++) {
			if (!secondFacesInBounds[i]) continue;
			Face face = GetFacePositionVerticesFromModel(secondModel, i, secondTransform);
			secondFacesInBounds[i] = ExpandBoxToFace(face, box, secondBox);
		}


		// Get intersection of the 2 boxes.
		Box newBox;
		for (int i = 0; i < 3; i++) {
			newBox.min[i] = max(firstBox.min[i], secondBox.min[i]);
			newBox.max[i] = min(firstBox.max[i], secondBox.max[i]);
		}

		if (newBox.min == box.min && newBox.max == box.max) {
			outputCollission.first = first;
			outputCollission.second = second;
			outputCollission.worldPosition = 0.5f * (box.min + box.max);
			outputCollission.impactDirectionAtFirst = GetCollisionDirection(firstModel, firstFacesInBounds);
			outputCollission.impactDirectionAtSecond = GetCollisionDirection(secondModel, secondFacesInBounds);
			return true;
		}
		box = newBox;
	}
	return false;
}
