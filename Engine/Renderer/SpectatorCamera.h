//
// Created by brosobad on 23/07/2025.
//

#ifndef SPECTATOR_CAMERA_H
#define SPECTATOR_CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SpectatorCamera {
public:
    SpectatorCamera(GLFWwindow* window, glm::vec3 pos = {0, 0, 3});
    void update(float deltaTime);
    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;
    glm::mat4 getProjectionMatrix() const;

private:
    GLFWwindow* window;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    bool rotating;
    double lastMouseX, lastMouseY;

    float fov = 45.0f;
    float aspect = 800.0f/600.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    void updateVectors();
    void processKeyboard(float dt);
    void processMouse();
};


#endif //SPECTATOR_CAMERA_H
