#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <vector>

#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

using namespace std;
# define M_PI           3.14159265358979323846
class Object
{
public:
	int CountVertices = 0;
	string currentObject;
	vector<GLfloat> vertices;
	vector<GLuint> indices;
	vector<GLfloat> verticesForPlane;
	vector<int> indexCheck; //store the index of vectices use to draw the plane
	void setObject(string object);
	vector<GLfloat> getVertices();
	vector<GLfloat> getVerticesForPlane() {
		return verticesForPlane;
	}
	vector<GLuint> getIndices();
	void drawObject();
	void drawPlane();
	void setObjectBasedOnInput(GLFWwindow* window, VBO& vbo, EBO& ebo);
	void UpdateObject(VBO& vbo, EBO& ebo);
	void addVertexToFormPlane(int index);

};