#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <cmath>

#define TINYOBJLOADER_IMPLEMENTATION
#define M_PI 3.14159f 
#include "tiny_obj_loader.h"


float radius = 0.5f;
float angle = 0.0f;

float scale_x = 1.f;
float scale_y = 1.f;
float scale_z = 1.f;

float axis_x = 0;
float axis_y = 1;
float axis_z = 0;


void Key_Callback(GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mods)
{
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		scale_x -= 0.1f;
		scale_y -= 0.1f;
		scale_z -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		scale_x += 0.1f;
		scale_y += 0.1f;
		scale_z += 0.1f;
	}
}

int main(void) {

	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(600, 600, "Juan Morcwel D. Sy-wico", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	glfwSetKeyCallback(window, Key_Callback);

	std::fstream vertSrc("Shaders/sample.vert");
	std::stringstream vertBuff;
	vertBuff << vertSrc.rdbuf();
	std::string vertS = vertBuff.str();
	const char* v = vertS.c_str();

	std::fstream fragSrc("Shaders/sample.frag");
	std::stringstream fragBuff;
	fragBuff << fragSrc.rdbuf();
	std::string fragS = fragBuff.str();
	const char* f = fragS.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &v, NULL);
	glCompileShader(vertexShader);

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &f, NULL);
	glCompileShader(fragShader);

	GLuint shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vertexShader);
	glAttachShader(shaderProg, fragShader);
	glLinkProgram(shaderProg);

	std::string path = "3D/bunny.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material;
	std::string warning, error;

	tinyobj::attrib_t attributes;

	bool success = tinyobj::LoadObj(
		&attributes,
		&shapes,
		&material,
		&warning,
		&error,
		path.c_str()
	);


	std::vector<GLuint> mesh_indices;
	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
	}

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * attributes.vertices.size(), &attributes.vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);


		angle += 0.01f;


		glUseProgram(shaderProg);


		for (int i = 0; i < 3; ++i) {
			float offsetAngle = (i * 2 * M_PI) / 3;
			float xPos = radius * cos(angle + offsetAngle);
			float yPos = radius * sin(angle + offsetAngle);


			float rotationAngle = atan2(yPos, xPos);

			glm::mat4 transformation_matrix = glm::mat4(1.0f);
			transformation_matrix = glm::translate(transformation_matrix, glm::vec3(xPos, yPos, 0.0f));
			transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_x, scale_y, scale_z));
			transformation_matrix = glm::rotate(transformation_matrix, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));

			unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));


			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}
