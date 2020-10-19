#pragma once

#include "MeshBuilder.h"
#include "MeshFactory.h"

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