// This class implements checking collisions between models.
// Collision detections has quite a few steps to it, so here's an overview of how the algorithms work.
// To check if an object is colliding, we go over all nearby objects, and check if the object and the nearby object are colliding.
// To check if 2 objects are colliding, we start with a box surrounding both objects.
// We then keep shrinking the box in one direction to the smallest possible box surrounding object 1,
// and then shrink it in one direction to the smallest possible box surrounding object 2.
// If the objects are colliding, this process will stop and we'll get a nice box around the are
// that they are colliding in, otherwise the box will shrink to a box with dimensions 0.

#include "CollisionDetector.h"

#include "../worldstate/Entity.h"

#include <chrono>
#include <iostream>
#include <memory>

#include "glm/gtx/io.hpp"

using namespace std;
using namespace glm;

namespace {

static const float collisionPrecision = 0.00001f;

struct Box {
	vec3 min;
	vec3 max;
};

struct Face {
	vec3 p1;
	vec3 p2;
	vec3 p3;
};

struct Line2D {
	vec2 p1;
	vec2 p2;
};

struct Line3D {
	vec3 p1;
	vec3 p2;
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
	return (length(normalSum) > 0.000001f) ? -normalize(normalSum) : vec3(0, 0, 0);
}

// Returns the intersection point of two lines if it exists.
// If lines are perpendicular, or don't intersect, returns false. Also returns false if lines overlap somewhere.
// Returns true if they do intersect, and writes the intersection point to outputIntersection.
bool GetIntersection(const Line2D &first, const Line2D &second, vec2 &outputIntersection) {
	for (int i = 0; i < 2; i++) {
		if (min(first.p1[i], first.p2[i]) > max(second.p1[i], second.p2[i])) return false;
		if (min(second.p1[i], second.p2[i]) > max(first.p1[i], first.p2[i])) return false;
	}

	mat2 system = transpose(
		mat2{first.p2.x - first.p1.x, second.p1.x - second.p2.x,
		     first.p2.y - first.p1.y, second.p1.y - second.p2.y});
	if (determinant(system) == 0) return false; // perpendicular lines

	vec2 solution = inverse(system) * vec2(second.p1.x - first.p1.x, second.p1.y - first.p1.y);

	if (solution[0] < 0 || solution[0] > 1 || solution[1] < 0 || solution[1] > 1) return false; // lines too short to intersect.
	outputIntersection = (1 - solution[0]) * first.p1 + solution[0] * first.p2;
	return true;
};

bool GetLineMinMaxAtPoint(const Line3D &line, vec2 point, float &outputMinZ, float &outputMaxZ) {
	bool sameX = line.p1.x == line.p2.x;
	bool sameY = line.p1.y == line.p2.y;
	if (sameX && sameY) {
		if (line.p1.x == point.x && line.p1.y == point.y) {
			outputMinZ = min(line.p1.z, line.p2.z);
			outputMaxZ = max(line.p1.z, line.p2.z);
			return true;
		}
		return false;
	}
	else if (sameX || sameY) {
		vec2 solution;
		if (sameX) {
			if (line.p1.x != point.x) return false;
			mat2 system = transpose(mat2{
				1,         1,
				line.p1.y, line.p2.y,
			});
			solution = inverse(system) * vec2(1, point.y);
		}
		else {
			if (line.p1.y != point.y) return false;
			mat2 system = transpose(mat2{
				1,         1,
				line.p1.x, line.p2.x,
			});
			solution = inverse(system) * vec2(1, point.x);
		}
		if (solution[0] < 0 || solution[1] < 0) return false;
		float z = (solution[0] * line.p1 + solution[1] * line.p2).z;
		outputMinZ = z;
		outputMaxZ = z;
		return true;
	}
	else {
		mat2 system = transpose(mat2{
			1,         1,
			line.p1.x, line.p2.x,
		});
		vec2 solution = inverse(system) * vec2(1, point.x);
		if (solution[0] < 0 || solution[1] < 0) return false;
		vec3 lineAtPoint = solution[0] * line.p1 + solution[1] * line.p2;
		if (abs(lineAtPoint.x - point.x) > 0.00001f) return false;
		outputMinZ = lineAtPoint.z;
		outputMaxZ = lineAtPoint.z;
		return true;
	}
}

// Given a point in 2d and a face, finds the minimum/maximum Z value of the face at that (x, y)
// point. Returns false if the face has no point with that (x, y) value, and true otherwise.
// Sets outputMinZ to the minimum face value and sets outputMaxZ with the maximum face value.
bool GetFaceMinMaxAtPoint(const Face &face, vec2 point, float &outputMinZ, float &outputMaxZ) {
	// We know that any point in the face can be written as a*v1 + b*v2 + c*v3, with a + b + c = 1.
	// This gives us a set of linear equations we can solve.
	mat3 system = transpose(mat3{1,         1,         1,
								 face.p1.x, face.p2.x, face.p3.x,
								 face.p1.y, face.p2.y, face.p3.y}
	);
	if (determinant(system) != 0) {
		// One unique solution exists.
		vec3 solution = inverse(system) * vec3(1, point.x, point.y);
		if (solution.x < 0 || solution.y < 0 || solution.z < 0) return false; // Make sure it's in the convex plane
		float z = (solution[0] * face.p1 + solution[1] * face.p2 + solution[2] * face.p3).z;
		outputMinZ = z;
		outputMaxZ = z;
		return true;
	}
	else {
		// The face may appear as a line from this perspective, so let's check for all combinations
		// of 2 points.
		bool success = false;
		float minZ;
		float maxZ;
		Line3D line;
		line.p1 = face.p1;
		line.p2 = face.p2;
		if (GetLineMinMaxAtPoint(line, point, minZ, maxZ)) {
			if (!success || minZ < outputMinZ) outputMinZ = minZ;
			if (!success || maxZ > outputMaxZ) outputMaxZ = maxZ;
			success = true;
		}
		line.p1 = face.p3;

		if (GetLineMinMaxAtPoint(line, point, minZ, maxZ)) {
			if (!success || minZ < outputMinZ) outputMinZ = minZ;
			if (!success || maxZ > outputMaxZ) outputMaxZ = maxZ;
			success = true;
		}
		line.p2 = face.p1;
		if (GetLineMinMaxAtPoint(line, point, minZ, maxZ)) {
			if (!success || minZ < outputMinZ) outputMinZ = minZ;
			if (!success || maxZ > outputMaxZ) outputMaxZ = maxZ;
			success = true;
		}		
		if (!success) return false;
		return true;
	}
}

// Finds the intersection of the lines (x1, x2) and (y1, y2) if there is one,
// then acts like GetValueAtPoint. Returns false if there is no intersection.
bool GetFaceMinMaxAtIntersection(const Face &face,
								 const Line2D &first, const Line2D &second,
								 float &outputMinZ, float &outputMaxZ) {
	vec2 intersection;
	if (!GetIntersection(first, second, intersection)) return false;
	return GetFaceMinMaxAtPoint(face, intersection, outputMinZ, outputMaxZ);
}

// Expands boxToExpand so the portion of the face that is within bounds all fits in boxToExpand.
// Returns true if there is any bit of face in bounds, and false otherwise.
bool ExpandBoxAroundFace(const Face &face, const Box &bounds, Box &boxToExpand) {
	bool inBox[3] = {false, false, false};
	for (int axis = 0; axis < 3; axis++) {
		// Swap axi so we don't have to worry about changing our x/y/z per direction we're looking at.
		Face swappedFace;
		swappedFace.p1 = SwapAxi(face.p1, axis);
		swappedFace.p2 = SwapAxi(face.p2, axis);
		swappedFace.p3 = SwapAxi(face.p3, axis);

		vec3 boxMin = SwapAxi(bounds.min, axis);
		vec3 boxMax = SwapAxi(bounds.max, axis);

		// We know from calculus that the minimum/maximum z is at either:
		// A corner of the face
		// A corner of the (x, y) square we're looking in.
		// An intersection of the face edge with the square edge.

		float outputMinZ;
		float outputMaxZ;

		// Face corners.
		for (vec3 faceCorner : {swappedFace.p1, swappedFace.p2, swappedFace.p3}) {
			if (faceCorner.x > boxMax.x || faceCorner.x < boxMin.x ||
				faceCorner.y > boxMax.y || faceCorner.y < boxMin.y) continue;
			outputMinZ = faceCorner.z;
			outputMaxZ = faceCorner.z;
			inBox[axis] = true;
			if (outputMinZ < boxToExpand.min[axis]) boxToExpand.min[axis] = outputMinZ;
			if (outputMaxZ > boxToExpand.max[axis]) boxToExpand.max[axis] = outputMaxZ;
		}

		// Box corners.
		for (float x : {boxMin.x, boxMax.x}) {
			for (float y : {boxMin.y, boxMax.y}) {
				if (GetFaceMinMaxAtPoint(swappedFace, vec2(x, y), outputMinZ, outputMaxZ)) {
					inBox[axis] = true;
					if (outputMinZ < boxToExpand.min[axis]) boxToExpand.min[axis] = outputMinZ;
					if (outputMaxZ > boxToExpand.max[axis]) boxToExpand.max[axis] = outputMaxZ;
				}
			}
		}

		// Intersections
		Line2D first, second;
		for (vec3 corner1 : {swappedFace.p1, swappedFace.p2, swappedFace.p3}) {
			for (vec3 corner2 : {swappedFace.p1, swappedFace.p2, swappedFace.p3}) {
				if (corner1 == corner2) continue;
				first.p1 = vec2(corner1);
				first.p2 = vec2(corner2);

				for (vec2 secondStart : {vec2(boxMin), vec2(boxMax)}) {
					for (vec2 secondEnd : {vec2(boxMin.x, boxMax.y), vec2(boxMax.x, boxMin.y)}) {
						second.p1 = secondStart;
						second.p2 = secondEnd;
						if (GetFaceMinMaxAtIntersection(swappedFace, first, second, outputMinZ, outputMaxZ)) {
							inBox[axis] = true;
							if (outputMinZ < boxToExpand.min[axis]) boxToExpand.min[axis] = outputMinZ;
							if (outputMaxZ > boxToExpand.max[axis]) boxToExpand.max[axis] = outputMaxZ;
					    }
					}
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
	shared_ptr<const Model> firstModel = first->GetModel();
	shared_ptr<const Model> secondModel = second->GetModel();
	shared_ptr<Entity> firstE = dynamic_pointer_cast<Entity>(first);
	shared_ptr<Entity> secondE = dynamic_pointer_cast<Entity>(second);
	mat4 firstTransform = first->LocalToWorldSpaceMatrix();
	mat4 secondTransform = second->LocalToWorldSpaceMatrix();
	size_t firstModelNumberOfFaces = firstModel->elements.size() / 3;
	size_t secondModelNumberOfFaces = secondModel->elements.size() / 3;
	vector<bool> firstFacesInBounds = vector<bool>(firstModelNumberOfFaces, true);
	vector<bool> secondFacesInBounds = vector<bool>(secondModelNumberOfFaces, true);

	Box box;
	float radius = max(firstModel->maxRadius, secondModel->maxRadius);
	for (int i = 0; i < 3; i++) {
		box.min[i] = min(first->GetPosition()[i], second->GetPosition()[i]) - radius;
		box.max[i] = max(first->GetPosition()[i], second->GetPosition()[i]) + radius;
	}

	while (box.min.x <= box.max.x && box.min.y <= box.max.y && box.min.z <= box.max.z) {
		// Create 2 boxes wrapping around first and second object, looking only in box.
		Box firstBox;
		firstBox.min = box.max;
		firstBox.max = box.min;
		for (int i = 0; i < firstModelNumberOfFaces; i++) {
			if (!firstFacesInBounds[i]) continue;
			Face face = GetFacePositionVerticesFromModel(firstModel, i, firstTransform);
			firstFacesInBounds[i] = ExpandBoxAroundFace(face, box, firstBox);
		}

		Box secondBox;
		secondBox.min = box.max;
		secondBox.max = box.min;
		for (int i = 0; i < secondModelNumberOfFaces ; i++) {
			if (!secondFacesInBounds[i]) continue;
			Face face = GetFacePositionVerticesFromModel(secondModel, i, secondTransform);
			secondFacesInBounds[i] = ExpandBoxAroundFace(face, box, secondBox);
		}

		// Get intersection of the 2 boxes.
		Box newBox;
		for (int i = 0; i < 3; i++) {
			newBox.min[i] = max(box.min[i], max(firstBox.min[i], secondBox.min[i]));
			newBox.max[i] = min(box.max[i], min(firstBox.max[i], secondBox.max[i]));
		}

		if (distance(newBox.min, box.min) < collisionPrecision && distance(newBox.max, box.max) < collisionPrecision) {
			outputCollission.first = first;
			outputCollission.second = second;
			outputCollission.worldPosition = 0.5f * (box.min + box.max);
			vec3 boxSize;
			for (int i = 0; i < 3; i++) boxSize[i] = box.max[i] - box.min[i];
			if (boxSize == vec3(0, 0, 0)) return false;
			vec3 impact;
			impact.x = boxSize.y * boxSize.z;
			impact.y = boxSize.x * boxSize.z;
			impact.z = boxSize.x * boxSize.y;
			impact = normalize(impact);
			for (int i = 0; i < 3; i++) 
				if (first->GetPosition()[i] > second->GetPosition()[i]) 
					impact[i] *= -1;
			outputCollission.impact = impact;
			return true;
		}
		box = newBox;
	}
	return false;
}
