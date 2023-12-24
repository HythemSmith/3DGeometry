#include "Object.h"

vector<GLfloat> Object::getVertices() {
    return this->vertices;
}
vector<GLuint> Object::getIndices() {
    return this->indices;
}

void Object::setObject(string object) {
    this->currentObject = object;
    this->vertices.clear();
    this->indices.clear();
    this->verticesForPlane.clear();
    this->indexCheck.clear();
    CountVertices = 0;
    if (object == "cube") {
        this->vertices = {
            // Vertices
            -0.5f, -0.2f,  0.5f,  0.0f, 1.f, 0.f, 1.f,// 0: Bottom-left front
             0.5f, -0.2f,  0.5f,  0.0f, 1.f, 0.f, 1.f,// 1: Bottom-right front
             0.5f,  0.8f,  0.5f,  0.0f, 1.f, 0.f, 1.f,// 2: Top-right front
            -0.5f,  0.8f,  0.5f,  0.0f, 1.f, 0.f, 1.f,// 3: Top-left front
            -0.5f, -0.2f, -0.5f,  0.0f, 1.f, 0.f, 1.f,// 4: Bottom-left back
             0.5f, -0.2f, -0.5f,  0.0f, 1.f, 0.f, 1.f,// 5: Bottom-right back
             0.5f,  0.8f, -0.5f,  0.0f, 1.f, 0.f, 1.f,// 6: Top-right back
            -0.5f,  0.8f, -0.5f,  0.0f, 1.f, 0.f,1.f // 7: Top-left back
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
            // Front left
            -0.5f, 0.0f,  0.5f,     0.f, 0.871f, 1.f, 1.f,
            // Back left
            -0.5f, 0.0f, -0.5f,     0.118f, 0.812f, 0.216f, 1.f,
            // Back right
            0.5f, 0.0f, -0.5f,     0.871f, 0.247f, 0.82f, 1.f,
            // Front right
            0.5f, 0.0f,  0.5f,     1.f, 0.f, 0.f, 1.f,
            // Apex
            0.0f, 0.8f,  0.0f,     0.059f, 0.678f, 0.667f, 1.f
        };
        this->indices = {
           0, 1, 2, 3,
           0, 1, 4,
           1, 2, 4,
           2, 3, 4,
           3, 0, 4
        };
    }
    if (object == "wireframe sphere") {
        int sectors = 60;
        int stacks = 60;
        float radius = 1.f;
        float sectorStep = 2 * M_PI / sectors;
        float stackStep = M_PI / stacks;

        for (int i = 0; i <= stacks; ++i) {
            float stackAngle = M_PI / 2 - i * stackStep;
            float xy = radius * cos(stackAngle);
            float z = radius * sin(stackAngle);

            for (int j = 0; j <= sectors; ++j) {
                float sectorAngle = j * sectorStep;

                float x = xy * cos(sectorAngle);
                float y = xy * sin(sectorAngle);

                this->vertices.push_back(x);
                this->vertices.push_back(y);
                this->vertices.push_back(z);
                this->vertices.push_back(0.0f);
                this->vertices.push_back(1.0f);
                this->vertices.push_back(0.0f);
                this->vertices.push_back(1.0f);
            }
        }

        for (int i = 0; i < stacks; ++i) {
            int k1 = i * (sectors + 1);
            int k2 = k1 + sectors + 1;

            for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
                if (i != 0) {
                    this->indices.push_back(k1);
                    this->indices.push_back(k2);
                    this->indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1)) {
                    this->indices.push_back(k1 + 1);
                    this->indices.push_back(k2);
                    this->indices.push_back(k2 + 1);
                }
            }
        }
    }
    
}

void Object::drawObject() {
    if (this->currentObject == "cube" || this->currentObject == "paramid") {
        glDrawElements(GL_LINE_STRIP, this->indices.size(), GL_UNSIGNED_INT, 0);
        return;
    }
    if (this->currentObject == "wireframe sphere") {
        glDrawElements(GL_LINES, this->indices.size(), GL_UNSIGNED_INT, 0);
        return;
    }

}
void Object::drawPlane() {
    if (CountVertices == 3) {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

void Object::setObjectBasedOnInput(GLFWwindow* window, VBO& vbo, EBO& ebo) {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        setObject("cube");
        UpdateObject(vbo, ebo);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        setObject("paramid");
        UpdateObject(vbo, ebo);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        setObject("wireframe sphere");
        UpdateObject(vbo, ebo);
    }
    // ... Other object selection conditions ...
}
void Object::UpdateObject(VBO& vbo, EBO& ebo) {
    vbo.Bind();
    ebo.Bind();
    vbo.UpdateBufferData(getVertices());
    ebo.UpdateBufferData(getIndices());
}

void Object::addVertexToFormPlane(int index) {
    if (CountVertices != 3) {
        auto check = std::find(indexCheck.begin(), indexCheck.end(), index);
        if (check != indexCheck.end()) {
            // Value found
            cout << "You have already chosen this vertex!!!\n\n";
            return;
        }
        else {
            // Value not found
            indexCheck.push_back(index);
            CountVertices++;
            cout << "Vertex added successfully!!!\n\n";
        }
    }
    if (CountVertices == 3) {
        for (int i = 0; i < indexCheck.size(); i++) {
            verticesForPlane.insert(verticesForPlane.end(), { vertices[indexCheck[i]], vertices[indexCheck[i] + 1], vertices[indexCheck[i] + 2]});
            verticesForPlane.insert(verticesForPlane.end(), { 0.851f, 0.722f, 0.231f,  .5f });
        }
        return;
    }
    
}