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
	glm::vec4 clipCoords(clipX, clipY, -1.0f, 1.0f);

	// Inverse transformation from clip space to eye space
	glm::mat4 invCamMatrix = glm::inverse(camMatrix);
	glm::vec4 eyeCoords = invCamMatrix * clipCoords;
	eyeCoords.z = -1.0f; // Set depth to near plane

	// Ray direction in world space
	glm::vec4 rayWorld = glm::normalize(eyeCoords);
	rayWorld.w = 0.0f;
	//cout << rayWorld.x << " " << rayWorld.y << " " << rayWorld.z<< endl;
	// Check for intersection with object's vertices
	for (size_t i = 0; i < object.vertices.size(); i+=6) {
		glm::vec4 vertexPos = glm::vec4(object.vertices[i],object.vertices[i + 1], 
										object.vertices[i + 2], 1.0f);
		glm::vec4 vertexWorld = camMatrix * vertexPos;
		//cout << vertexWorld.x << " " << vertexWorld.y << " " << " " << vertexWorld.z << endl;
		// Perform intersection test (e.g., distance comparison, bounding box check, etc.)
		// Here, you can check if the ray intersects with the vertex position
		// For instance, compute the distance between ray and vertex positions
		float distance = glm::distance(rayWorld, vertexWorld);

		// Adjust threshold for intersection based on your needs
		float intersectionThreshold = 0.1f;

		if (distance < intersectionThreshold) {
			std::cout << "Clicked on vertex " << i << std::endl;
			return true; // Hit found
		}
	}
	cout << "failed" << endl;
	return false; // No hit found
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glm::mat4 camMatrix = camera.camMatrix; // Example function to get the combined matrix
		glm::vec3 cameraPos = camera.Position; // Get camera position
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				std::cout << camMatrix[i][j] << " ";
			}
			std::cout << std::endl;
		}
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


	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(object.getVertices());
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(object.getIndices());
	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	//VBO1.Unbind();
	//EBO1.Unbind();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

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
		// --------------------------Check code here---------------------------------------//









		// --------------------------Check code here---------------------------------------//
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		
		// Draw primitives, number of indices, datatype of indices, index of indices
		object.drawObject();
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}