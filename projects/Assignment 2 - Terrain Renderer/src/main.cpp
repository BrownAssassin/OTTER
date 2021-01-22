#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLFWwindow* window;

unsigned char* heightmap;
unsigned char* sand;
unsigned char* grass;
unsigned char* stone;
unsigned char* snow;
unsigned char* water;
int width, height;

void loadImage() {
	int channels;
	stbi_set_flip_vertically_on_load(true);

	sand = stbi_load("sand.jpg",
		&width,
		&height,
		&channels,
		0);
	if (sand) {
		std::cout << "Image LOADED" << width << " " << height << std::endl;
	}
	else {
		std::cout << "Failed to load image!" << std::endl;
	}

	grass = stbi_load("grass.jpg",
		&width,
		&height,
		&channels,
		0);
	if (grass) {
		std::cout << "Image LOADED" << width << " " << height << std::endl;
	}
	else {
		std::cout << "Failed to load image!" << std::endl;
	}

	stone = stbi_load("stone.jpg",
		&width,
		&height,
		&channels,
		0);
	if (stone) {
		std::cout << "Image LOADED" << width << " " << height << std::endl;
	}
	else {
		std::cout << "Failed to load image!" << std::endl;
	}

	snow = stbi_load("snow.jpg",
		&width,
		&height,
		&channels,
		0);
	if (snow) {
		std::cout << "Image LOADED" << width << " " << height << std::endl;
	}
	else {
		std::cout << "Failed to load image!" << std::endl;
	}
	
	water = stbi_load("water.jpg",
		&width,
		&height,
		&channels,
		0);
	if (water) {
		std::cout << "Image LOADED" << width << " " << height << std::endl;
	}
	else {
		std::cout << "Failed to load image!" << std::endl;
	}

	heightmap = stbi_load("heightmap.bmp",
		&width,
		&height,
		&channels,
		0);
	if (heightmap) {
		std::cout << "Image LOADED" << width << " " << height << std::endl;
	}
	else {
		std::cout << "Failed to load image!" << std::endl;
	}
}

bool initGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to Initialize GLFW" << std::endl;
		return false;
	}

	window = glfwCreateWindow(800, 800, "Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	return true;
}

bool initGLAD() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		std::cout << "Failed to initialize Glad" << std::endl;
		return false;
	}
}

GLuint terrain_shader_program, water_shader_program;

bool loadShaders() {
	std::string tvert_shader_str;
	std::ifstream tvs_stream("Terrain.vert", std::ios::in);
	if (tvs_stream.is_open()) {
		std::string Line = "";
		while (getline(tvs_stream, Line))
			tvert_shader_str += "\n" + Line;
		tvs_stream.close();
	}
	else {
		printf("Could not open vertex shader!!\n");
		return false;
	}
	const char* tvs_str = tvert_shader_str.c_str();

	std::string tfrag_shader_str;
	std::ifstream tfs_stream("Terrain.frag", std::ios::in);
	if (tfs_stream.is_open()) {
		std::string Line = "";
		while (getline(tfs_stream, Line))
			tfrag_shader_str += "\n" + Line;
		tfs_stream.close();
	}
	else {
		printf("Could not open fragment shader!!\n");
		return false;
	}
	const char* tfs_str = tfrag_shader_str.c_str();
	
	GLuint tvs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(tvs, 1, &tvs_str, NULL);
	glCompileShader(tvs);
	GLuint tfs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(tfs, 1, &tfs_str, NULL);
	glCompileShader(tfs);

	terrain_shader_program = glCreateProgram();
	glAttachShader(terrain_shader_program, tfs);
	glAttachShader(terrain_shader_program, tvs);
	glLinkProgram(terrain_shader_program);

	std::string wvert_shader_str;
	std::ifstream wvs_stream("Water.vert", std::ios::in);
	if (wvs_stream.is_open()) {
		std::string Line = "";
		while (getline(wvs_stream, Line))
			wvert_shader_str += "\n" + Line;
		wvs_stream.close();
	}
	else {
		printf("Could not open vertex shader!!\n");
		return false;
	}
	const char* wvs_str = wvert_shader_str.c_str();

	std::string wfrag_shader_str;
	std::ifstream wfs_stream("Water.frag", std::ios::in);
	if (wfs_stream.is_open()) {
		std::string Line = "";
		while (getline(wfs_stream, Line))
			wfrag_shader_str += "\n" + Line;
		wfs_stream.close();
	}
	else {
		printf("Could not open fragment shader!!\n");
		return false;
	}
	const char* wfs_str = wfrag_shader_str.c_str();

	GLuint wvs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(wvs, 1, &wvs_str, NULL);
	glCompileShader(wvs);
	GLuint wfs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(wfs, 1, &wfs_str, NULL);
	glCompileShader(wfs);

	water_shader_program = glCreateProgram();
	glAttachShader(water_shader_program, wfs);
	glAttachShader(water_shader_program, wvs);
	glLinkProgram(water_shader_program);

	return true;
}

float rx = 0.0f;
float ry = 0.0f;
float tz = 0.0f;
GLuint filter_mode = GL_NEAREST;

void keyboard() {
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		ry += 0.1;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		ry -= 0.1;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		rx -= 0.1;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		rx += 0.1;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		tz += 0.001;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		tz -= 0.001;
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		if (filter_mode == GL_LINEAR) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			filter_mode = GL_NEAREST;
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			filter_mode = GL_LINEAR;
		}
	}
}

GLfloat* points2;
GLfloat* uvs2;
int numVertices;

void genPlane(int d) {
	numVertices = 6 * (d * d);
	points2 = new GLfloat[3 * numVertices];
	uvs2 = new GLfloat[2 * numVertices];
	GLfloat x, y, z;
	int i = 0;
	x = -1.0;
	y = -0.5;
	z = 1.0;

	int uvcounter = 0;

	for (int col = 0; col < d; col++)
	for (int row = 0; row < d; row++) {

		points2[i++] = x + row * (2.0 / d);
		points2[i++] = y;
		points2[i++] = z - col * (2.0 / d);

		uvs2[uvcounter++] = 0.0 + row * (1.0 / d);
		uvs2[uvcounter++] = 0.0 + col * (1.0 / d);

		points2[i++] = points2[i - 3] + (2.0 / d);
		points2[i++] = y;
		points2[i++] = points2[i - 3];

		uvs2[uvcounter++] = uvs2[uvcounter - 2] + 1.0 / d;
		uvs2[uvcounter++] = uvs2[uvcounter - 2];

		points2[i++] = points2[i - 6];
		points2[i++] = y;
		points2[i++] = points2[i - 3] - (2.0 / d);

		uvs2[uvcounter++] = uvs2[uvcounter - 4];
		uvs2[uvcounter++] = uvs2[uvcounter - 2] + 1.0 / d;


		points2[i++] = points2[i - 6];
		points2[i++] = y;
		points2[i++] = points2[i - 6];

		uvs2[uvcounter++] = uvs2[uvcounter - 4];
		uvs2[uvcounter++] = uvs2[uvcounter - 4];

		points2[i++] = points2[i - 3];
		points2[i++] = y;
		points2[i++] = points2[i - 3] - (2.0 / d);

		uvs2[uvcounter++] = uvs2[uvcounter - 2];
		uvs2[uvcounter++] = uvs2[uvcounter - 4];


		points2[i++] = points2[i - 9];
		points2[i++] = y;
		points2[i++] = points2[i - 3];

		uvs2[uvcounter++] = uvs2[uvcounter - 6];
		uvs2[uvcounter++] = uvs2[uvcounter - 6];
	}
}



int main() {
	
	//Initialize GLFW
	if (!initGLFW())
		return 1;
	
	//Initialize GLAD
	if (!initGLAD())
		return 1;

	// Mesh data
	genPlane(100);

	// Color data
	static const GLfloat colors[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f
	};
	
	//VBO
	GLuint pos_vbo = 0;
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, (3 * numVertices) * sizeof(GLfloat), points2, GL_STATIC_DRAW);

	GLuint color_vbo = 1;
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	GLuint uv_vbo = 2;
	glGenBuffers(1, &uv_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
	glBufferData(GL_ARRAY_BUFFER, (2 * numVertices) * sizeof(GLfloat), uvs2, GL_STATIC_DRAW);

	// VAO
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	loadImage();

	/////// TEXTURE 0
	GLuint heightmapHandle;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &heightmapHandle);
	glBindTexture(GL_TEXTURE_2D, heightmapHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, heightmap);

	stbi_image_free(heightmap);

	/////// TEXTURE 1
	GLuint sandHandle;

	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &sandHandle);
	glBindTexture(GL_TEXTURE_2D, sandHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, sand);

	stbi_image_free(sand);

	/////// TEXTURE 2
	GLuint grassHandle;

	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &grassHandle);
	glBindTexture(GL_TEXTURE_2D, grassHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, grass);

	stbi_image_free(grass);

	/////// TEXTURE 3
	GLuint stoneHandle;

	glActiveTexture(GL_TEXTURE3);
	glGenTextures(1, &stoneHandle);
	glBindTexture(GL_TEXTURE_2D, stoneHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, stone);

	stbi_image_free(stone);

	/////// TEXTURE 4
	GLuint snowHandle;

	glActiveTexture(GL_TEXTURE4);
	glGenTextures(1, &snowHandle);
	glBindTexture(GL_TEXTURE_2D, snowHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, snow);

	stbi_image_free(snow);

	/////// TEXTURE 5
	GLuint waterHandle;

	glActiveTexture(GL_TEXTURE5);
	glGenTextures(1, &waterHandle);
	glBindTexture(GL_TEXTURE_2D, waterHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1000, 1000, 0, GL_RGB, GL_UNSIGNED_BYTE, water);

	stbi_image_free(water);

	// Load your shaders
	if (!loadShaders())
		return 1;

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glm::mat4 Projection = 
		glm::perspective(glm::radians(45.0f), 
		(float)width / (float)height, 0.0001f, 100.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(1, 3, 4),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 Model2 = glm::mat4(1.0f);

	Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));
	Model2 = glm::translate(Model2, glm::vec3(0.0f, 0.25f, 0.0f));


	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model;
	glm::mat4 mvp2 = Projection * View * Model2;

	GLuint MatrixID = glGetUniformLocation(terrain_shader_program, "MVP");
	GLuint MatrixID2 = glGetUniformLocation(water_shader_program, "MVP2");

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	
	GLuint loc = glGetUniformLocation(terrain_shader_program, "time");
	GLuint loc2 = glGetUniformLocation(water_shader_program, "time2");
	GLfloat time = 0.0;
	GLfloat time2 = 0.0;

	///// Game loop /////
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		keyboard();

		Model = glm::rotate(Model, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		Model = glm::rotate(Model, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, tz));
		mvp = Projection * View * Model;
		mvp2 = Projection * View * Model * Model2;
		rx = ry = tz = 0;

		glUseProgram(terrain_shader_program);

		glUniform1i(glGetUniformLocation(terrain_shader_program, "heightmapSampler"), 0);
		glUniform1i(glGetUniformLocation(terrain_shader_program, "sandSampler"), 1);
		glUniform1i(glGetUniformLocation(terrain_shader_program, "grassSampler"), 2);
		glUniform1i(glGetUniformLocation(terrain_shader_program, "stoneSampler"), 3);
		glUniform1i(glGetUniformLocation(terrain_shader_program, "snowSampler"), 4);
		
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		glUniform1f(loc, time);
		time += 0.1;

		glDrawArrays(GL_TRIANGLES, 0, numVertices);


		glUseProgram(water_shader_program);

		glUniform1i(glGetUniformLocation(water_shader_program, "waterSampler"), 5);

		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &mvp2[0][0]);

		glUniform1f(loc2, time2);
		time2 += 0.1;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
		
		glDisable(GL_BLEND);

		glfwSwapBuffers(window);
	}
	return 0;
}
