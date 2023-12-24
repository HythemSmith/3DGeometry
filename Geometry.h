#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class VertexChecker {
public:
    VertexChecker() {}

    bool checkVertexClick(GLFWwindow* window, double xpos, double ypos, const glm::mat4& camMatrix);

    bool checkEdgeClick(GLFWwindow* window, double xpos, double ypos, const glm::mat4& view, const glm::mat4& projection) {
        // Your implementation of checkEdgeClick function
        // ...
    }
};
