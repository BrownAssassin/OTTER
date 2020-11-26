#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream> //03
#include <string> //03

#include <GLM/glm.hpp> //04
#include <glm/gtc/matrix_transform.hpp> //04

#define STB_IMAGE_IMPLEMENTATION //07
#include <stb_image.h> //07

#include "OBJLoader.h"

GLFWwindow* window;

unsigned char* image;
int width, height;

void loadImage() {
	int channels;
	stbi_set_flip_vertically_on_load(true);
	image = stbi_load("box.bmp",
		&width,
		&height,
		&channels,
		0);
	if (image) {
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

	//Create a new GLFW window
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


GLuint shader_program;

bool loadShaders() {
	// Read Shaders from file
	std::string vert_shader_str;
	std::ifstream vs_stream("vertex_shader.glsl", std::ios::in);
	if (vs_stream.is_open()) {
		std::string Line = "";
		while (getline(vs_stream, Line))
			vert_shader_str += "\n" + Line;
		vs_stream.close();
	}
	else {
		printf("Could not open vertex shader!!\n");
		return false;
	}
	const char* vs_str = vert_shader_str.c_str();

	std::string frag_shader_str;
	std::ifstream fs_stream("frag_shader.glsl", std::ios::in);
	if (fs_stream.is_open()) {
		std::string Line = "";
		while (getline(fs_stream, Line))
			frag_shader_str += "\n" + Line;
		fs_stream.close();
	}
	else {
		printf("Could not open fragment shader!!\n");
		return false;
	}
	const char* fs_str = frag_shader_str.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_str, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_str, NULL);
	glCompileShader(fs);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);

	return true;
}

//INPUT handling
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



int main() {
	//Initialize GLFW
	if (!initGLFW())
		return 1;

	//Initialize GLAD
	if (!initGLAD())
		return 1;

	std::vector<Vertex> mesh = loadOBJ("icosphere.obj");

	const unsigned int nrOfVertices = mesh.size();
	const unsigned int nrOfIndices = 0;

	Vertex* vertexArray = new Vertex[nrOfVertices];
	for (size_t i = 0; i < nrOfVertices; i++)
	{
		vertexArray[i] = mesh.data()[i];
	}

	GLuint* indexArray = new GLuint[nrOfIndices];
	for (size_t i = 0; i < nrOfIndices; i++)
	{
		indexArray[i] = indexArray[i];
	}

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	//Create VAO
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

	//GEN EBO AND BIND AND SEND DATA
	if (nrOfIndices > 0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);
	}

	//SET VERTEXATTRIBPOINTERS AND ENABLE
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);
	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	//BIND VAO 0
	glBindVertexArray(0);

	///////////////////////////


	// Load your shaders
	if (!loadShaders())
		return 1;

	// Lecture 04
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glm::mat4 Projection = 
		glm::perspective(glm::radians(45.0f), 
		(float)width / (float)height, 0.0001f, 100.0f);
	
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 3), // Camera is at (0,0,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
		
	// Get handles for our matrix uniforms
	// Only during the initialisation
	GLuint mvpID = glGetUniformLocation(shader_program, "MVP");
	GLuint modelID = glGetUniformLocation(shader_program, "Model");
	GLuint viewID = glGetUniformLocation(shader_program, "View");
	GLuint projectionID = glGetUniformLocation(shader_program, "Projection");

	// Lighting settings and materials
	GLfloat eyePos[] = { 0.0f, 0.0f, 3.0f };
	GLfloat lightPos[] = { -1.0f, 1.0f, 3.0f };

	GLuint eyeID = glGetUniformLocation(shader_program, "eye_pos");
	GLuint lightID = glGetUniformLocation(shader_program, "light_pos");

	
	

	
	// Face culling
//	glEnable(GL_CULL_FACE);
//	glFrontFace(GL_CCW);

	// glCullFace(GL_FRONT); //GL_BACK, GL_FRONT_AND_BACK


	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	
	/////// TEXTURE
	glUniform1i(glGetUniformLocation(shader_program, "myTextureSampler"), 0);

	
	///// Game loop /////
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_program);

		//Model = glm::mat4(1.0f);

		keyboard();

		Model = glm::rotate(Model, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		Model = glm::rotate(Model, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, tz));
		rx = ry = tz = 0;
		
		glBindVertexArray(VAO);

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)

		glUniformMatrix4fv(modelID, 1, false, &Model[0][0]);
		glUniformMatrix4fv(viewID, 1, false, &View[0][0]);
		glUniformMatrix4fv(projectionID, 1, false, &Projection[0][0]);
		glUniform3fv(eyeID, 1, &eyePos[0]);
		glUniform3fv(lightID, 1, &lightPos[0]);
		
		
		//RENDER
		if (nrOfIndices == 0)
			glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		else
			glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
	}
	return 0;

}