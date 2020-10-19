#include "ObjLoader.h"

VertexArrayObject::sptr ObjLoader::LoadObj(const std::string& file) {
	std::ifstream obj(file);

	if (!obj.is_open()) {
		throw std::runtime_error("Failed to open file");
	}

	std::stringstream ss;
	std::string line;
	std::string prefix;

	MeshBuilder<VertexPosNormTex> mesh;
	std::vector<glm::fvec3> posVerts;
	std::vector<glm::fvec2> texVerts;
	std::vector<glm::fvec3> normVerts;
	std::vector<GLint> pos_indices;
	std::vector<GLint> tex_indices;
	std::vector<GLint> norm_indices;
	
	glm::vec3 pos_norm;
	glm::vec2 tex;
	GLint index;

	while (std::getline(obj, line)) {
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#") {

		}
		else if (prefix == "v") {
			ss >> pos_norm.x >> pos_norm.y >> pos_norm.z;
			posVerts.push_back(pos_norm);
		}
		else if (prefix == "vt") {
			ss >> tex.x >> tex.y;
			texVerts.push_back(tex);
		}
		else if (prefix == "vn") {
			ss >> pos_norm.x >> pos_norm.y >> pos_norm.z;
			normVerts.push_back(pos_norm);
		}
		else if (prefix == "f") {
			int indexType = 0;

			while (ss >> index) {
				if (indexType == 0)
					pos_indices.push_back(index);
				else if (indexType == 1)
					tex_indices.push_back(index);
				else if (indexType == 2)
					norm_indices.push_back(index);

				if (ss.peek() == '/') {
					indexType++;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					indexType++;
					ss.ignore(1, ' ');
				}

				if (indexType > 2)
					indexType = 0;
			}
		}

		for (size_t i = 0; i < pos_indices.size(); ++i) {
			mesh.AddIndex(mesh.AddVertex(VertexPosNormTex(posVerts[pos_indices[i] - 1], normVerts[norm_indices[i] - 1], texVerts[tex_indices[i] - 1])));
		}
	}

	return mesh.Bake();
}

