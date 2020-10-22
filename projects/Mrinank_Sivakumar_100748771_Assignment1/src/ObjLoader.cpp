#include "ObjLoader.h"

VertexArrayObject::sptr ObjLoader::LoadObj(const std::string& file) {
	// Opens the file
	std::ifstream obj(file);

	// Throws an error if the file doesn't open
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

	// Iterates through every line in the file
	while (std::getline(obj, line)) {
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#") {
			// Ignores commented lines
		}
		else if (prefix == "v") { // Gets the position vectors of the vertices from the file and pushes it back to our array of position vectors
			ss >> pos_norm.x >> pos_norm.y >> pos_norm.z;
			posVerts.push_back(pos_norm);
		}
		else if (prefix == "vt") { // Gets the texture coords from the file and pushes it back to our array of texture coords
			ss >> tex.x >> tex.y;
			texVerts.push_back(tex);
		}
		else if (prefix == "vn") { // Gets the normal vectors from the file and pushes it back to our array of normal vectors
			ss >> pos_norm.x >> pos_norm.y >> pos_norm.z;
			normVerts.push_back(pos_norm);
		}
		else if (prefix == "f") { // Reads the face data and pushes back the indices of each vertex to their respective arrays
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

		// Iterates through all the indices and adds them with their corresponding vertex info to the mesh
		for (size_t i = 0; i < pos_indices.size(); ++i) {
			mesh.AddIndex(mesh.AddVertex(VertexPosNormTex(posVerts[pos_indices[i] - 1], normVerts[norm_indices[i] - 1], texVerts[tex_indices[i] - 1])));
		}
	}

	// Returns the baked mesh
	return mesh.Bake();
}

