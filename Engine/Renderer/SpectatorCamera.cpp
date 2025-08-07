//
// Created by brosobad on 23/07/2025.
//

#include "SpectatorCamera.h"

#include "Logger.h"

SpectatorCamera::SpectatorCamera(GLFWwindow* win, glm::vec3 pos)
    : window(win), position(pos), worldUp(0, 1, 0),
      yaw(-90.0f), pitch(0.0f), speed(5.0f), sensitivity(0.2f),
      rotating(false), lastMouseX(0), lastMouseY(0) {
    updateVectors();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void SpectatorCamera::update(float deltaTime) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    aspect = static_cast<float>(width) / static_cast<float>(height);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!rotating) {
            rotating = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
        processMouse();
    } else if (rotating) {
        rotating = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    processKeyboard(deltaTime);
}

glm::mat4 SpectatorCamera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 SpectatorCamera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}
glm::vec3 SpectatorCamera::getPosition() const {
    return position;
}

void SpectatorCamera::processKeyboard(float dt) {
    float velocity = speed * dt;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += right * velocity;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position += worldUp * velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        position -= worldUp * velocity;
}

void SpectatorCamera::processMouse() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    float xoffset = float(xpos - lastMouseX) * sensitivity;
    float yoffset = float(lastMouseY - ypos) * sensitivity;

    lastMouseX = xpos;
    lastMouseY = ypos;

    yaw += xoffset;
    pitch += yoffset;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);
    updateVectors();
}

void SpectatorCamera::updateVectors() {
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(dir);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}