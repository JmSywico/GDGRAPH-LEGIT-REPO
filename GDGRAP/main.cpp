#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

float x_mod = 0;
float y_mod = 0;

float x;
float y;
float z;
float thetaX = 0;
float thetaY = 0;

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
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		x += 0.005f; // Adjust the step size for smoother movement
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		x -= 0.005f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		y += 0.005f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		y -= 0.005f;
	}

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

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		thetaX -= 5.f; 
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		thetaX += 5.f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		thetaY -= 5.f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		thetaY += 5.f;
	}
}


int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1000, 1000, "Juan Morcwel D. Sy-wico", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
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


	GLfloat vertices[]{
		0.f,    0.5f,   0.f
		- 0.5f,  0.f,    0.f,
		0.5f,   0.f,    0.f
	};

	GLuint indices[]{
		0, 1, 2
	};



	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * attributes.vertices.size(),
		&attributes.vertices[0],
		GL_STATIC_DRAW
		//GL_DYNAMIC_DRAW if needs to move.
	);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(void*)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLuint) * mesh_indices.size(),
		mesh_indices.data(),
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Modifier for the model's x Position


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		/*x_mod += 0.001f;*/

		//unsigned int xLoc = glGetUniformLocation(shaderProg, "x");
		//glUniform1f(xLoc, x_mod);

		//unsigned int yLoc = glGetUniformLocation(shaderProg, "y");
		//glUniform1f(yLoc, y_mod);

		//
		//unsigned int xLoc = glGetUniformLocation(shaderProg, "x");
		//glUniform1f(xLoc, x_mod);

		//unsigned int yLoc = glGetUniformLocation(shaderProg, "y");
		//glUniform1f(yLoc, y_mod);


			//Matrix
			//glm::mat3 identity_matrix3 = glm::mat3(1.0f);
		glm::mat4 identity_matrix4 = glm::mat4(1.0f);

		glm::mat4 transformation_matrix = glm::translate(identity_matrix4,
			glm::vec3(x, y, z)
		);

		transformation_matrix = glm::scale(transformation_matrix,
			glm::vec3(scale_x, scale_y, scale_z)
		);

		transformation_matrix = glm::rotate(transformation_matrix,
			glm::radians(thetaX),
			glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
		);

		transformation_matrix = glm::rotate(transformation_matrix,
			glm::radians(thetaY),
			glm::normalize(glm::vec3(axis_x + 1.f, axis_y - 1.f, axis_z))
		);

		unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");

		glUniformMatrix4fv(transformLoc,
			1,
			GL_FALSE,
			glm::value_ptr(transformation_matrix));


		glBindVertexArray(VAO);
		glUseProgram(shaderProg);
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}