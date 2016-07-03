#include "Crate.h"

using namespace std;

shared_ptr<const Model> Crate::GetModel() const {
	return ModelLoader::GetModel("crate");
}

float Crate::GetWeight() const {
	return 1.0f;
}
