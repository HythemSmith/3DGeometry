#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <vector>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include "Object.h"

using namespace std;


const unsigned int width = 1000;
const unsigned int height = 800;
Object object;
// Creates camera object
Camera camera(width, height, glm::vec3(0.0f, 0.3f, 2.0f));

bool checkVertexClick(double xpos, double ypos, const glm::mat4& camMatrix, const glm::vec3& cameraPos) {
	// Convert screen coordinates to clip space
	float clipX = (2.0f * xpos) / width - 1.0f;
	float clipY = 1.0f - (2.0f * ypos) / height;
	glm::vec2 clipCoords(clipX, clipY);

	for (size_t i = 0; i < object.vertices.size(); i+=7) {
		glm::vec4 vertexPos = glm::vec4(object.vertices[i],object.vertices[i + 1], 
										object.vertices[i + 2], 1.0f);
		glm::vec4 vertexWorld = camMatrix * vertexPos;
		glm::vec4 vertexNDC = vertexWorld / vertexWorld.w;
		// Perform a simple distance check with clicked point and each vertex
		float distance = glm::distance(glm::vec2(clipCoords), glm::vec2(vertexNDC));

		// Set a threshold for intersection (adjust as needed)
		float intersectionThreshold = 0.1f;

		if (distance < intersectionThreshold) {
			object.addVertexToFormPlane(i);
			return true; // Hit found
		}
	}
	std::cout << "No hit" << std::endl << endl;
	return false; // No hit found
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glm::mat4 camMatrix = camera.camMatrix; // Example function to get the combined matrix
		glm::vec3 cameraPos = camera.Position; // Get camera position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		checkVertexClick(xpos, ypos, camMatrix, cameraPos);
	}
}

int main()
{
	// Initialize GLFW
	glfwInit();
	
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24); // Request a 24-bit depth buffer

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	object.setObject("cube");
	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");


	// Generates Vertex Array Objects and binds them separately for different objects
	VAO VAO1;
	VAO1.Bind();

	// Generate and link VBO and EBO for the first object
	VBO VBO1(object.getVertices());
	EBO EBO1(object.getIndices());
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 7 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 4, GL_FLOAT, 7 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind VAO1 to prevent accidental modification
	VAO1.Unbind();

	// Create another VAO for the second object
	VAO VAO2;
	VAO2.Bind();

	// Generate and link VBO for the second object
	VBO VBO2(object.getVerticesForPlane());
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 7 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 4, GL_FLOAT, 7 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind VAO2 to prevent accidental modification
	VAO2.Unbind();
	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Choose objects
		object.setObjectBasedOnInput(window, VBO1, EBO1);
		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		object.drawObject();
		VAO1.Unbind();
		// Bind the VAO so OpenGL knows to use it
		VAO2.Bind();
		if (object.CountVertices == 3) {
			VBO2.Bind();
			VBO2.UpdateBufferData(object.getVerticesForPlane());
			object.drawPlane();
		}
		VAO2.Unbind();
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	VAO2.Delete();
	VBO2.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}