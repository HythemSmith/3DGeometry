#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <vector>
using namespace std;

class Object
{
private:
	string currentObject;
	vector<GLfloat> vertices;
	vector<GLuint> indices;
public:
	void setObject(string object);
	vector<GLfloat> getVertices();
	vector<GLuint> getIndices();
};