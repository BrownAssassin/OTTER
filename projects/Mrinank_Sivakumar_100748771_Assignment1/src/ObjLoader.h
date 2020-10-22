#pragma once

#include "MeshBuilder.h"
#include "VertexArrayObject.h"
#include "VertexTypes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/glm.hpp>
#include <GLM/gtx/euler_angles.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

class ObjLoader {
public:
	static VertexArrayObject::sptr LoadObj(const std::string& file);

protected:
	ObjLoader() = default;
	~ObjLoader() = default;
};