#include "Object.h"

vector<GLfloat> Object::getVertices() {
    return this->vertices;
}
vector<GLuint> Object::getIndices() {
    return this->indices;
}

void Object::setObject(string object) {
	if (object == "cube") {
		this->vertices = {
            // Vertices
            -0.5f, -0.2f,  0.5f,  0.0f, 1.f, 0.f,// 0: Bottom-left front
             0.5f, -0.2f,  0.5f,  0.0f, 1.f, 0.f,// 1: Bottom-right front
             0.5f,  0.8f,  0.5f,  0.0f, 1.f, 0.f,// 2: Top-right front
            -0.5f,  0.8f,  0.5f,  0.0f, 1.f, 0.f,// 3: Top-left front
            -0.5f, -0.2f, -0.5f,  0.0f, 1.f, 0.f,// 4: Bottom-left back
             0.5f, -0.2f, -0.5f,  0.0f, 1.f, 0.f,// 5: Bottom-right back
             0.5f,  0.8f, -0.5f,  0.0f, 1.f, 0.f,// 6: Top-right back
            -0.5f,  0.8f, -0.5f,  0.0f, 1.f, 0.f // 7: Top-left back
        };
        this->indices = {
            // Front face
            0, 1, 2, 3, 0,// GL_LINE_LOOP for front face

            // Back face
            4, 7, 6, 5, 4,  // GL_LINE_LOOP for back face

            // Top face
            0, 3, 7, 6, 2, 1, 5 // GL_LINE_LOOP for top face
        };
	}
    if (object == "paramid") {
        this->vertices = {
            // Vertices
            -0.5f, 0.0f,  0.5f,     0.f, 0.871f, 1.f,
            -0.5f, 0.0f, -0.5f,     0.118f, 0.812f, 0.216f,
            0.5f, 0.0f, -0.5f,     0.871f, 0.247f, 0.82f,
            0.5f, 0.0f,  0.5f,     1.f, 0.f, 0.f,	
            0.0f, 0.8f,  0.0f,     0.059f, 0.678f, 0.667f
        };
        this->indices = {
           0, 1, 2, 3,
           0, 1, 4,
           1, 2, 4,
           2, 3, 4,
           3, 0, 4
        };
    }
}