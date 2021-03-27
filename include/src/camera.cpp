#include "src/camera.hpp"

//using namespace CustomOps;

/**
 * Initializes camera at position (0, 0, 3) and looking towards negative Z
*/
Camera::Camera() 
    : forward(0, 0, -1), right(1, 0, 0), up(0, 1, 0), 
      pos(0, 0, 3), rotationEulers(0.0f, -180.0f, 0.0f) 
      { }

/**
 * Initializes camera at given position
 * @param initialPos Initial position
*/
Camera::Camera(const glm::vec3& initialPos) : Camera() { 
    pos = initialPos;
}

/**
 * Will translate camera with given direction
 * @param dir Direction to move towards
*/
void Camera::Move(const glm::vec3& dir) {
    pos += dir;
}

/**
 * Will rotate camera's forward with given angles
 * @param rot Delta euler angles rotation
*/
void Camera::Rotate(const glm::vec3& rot) {
    glm::vec3 newForward;
    float radX, radY, angleX(rot.x), angleY(rot.y);

    if (angleY != 0 || angleX != 0) {
        newForward = forward;
        rotationEulers.y += angleY;
        rotationEulers.x += angleX;

        if (rotationEulers.x > 89 && angleX > 0)
            rotationEulers.x = 89.0f;
        else if (rotationEulers.x < -89 && angleX < 0)
            rotationEulers.x = -89.0f;

        radX = glm::radians(rotationEulers.x);
        radY = glm::radians(rotationEulers.y);

        newForward.x = cos(radY);
        newForward.z = sin(radY) * cos(radX);
        newForward.y = sin(radX);

        forward = newForward;

        right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
        up = glm::cross(right, forward);
    }
}

const glm::vec3& Camera::GetPosition() const {
    return pos;
}

glm::vec3 Camera::GetForward() const {
    return forward;
}

glm::vec3 Camera::GetRight() const {
    return right;
}

glm::vec3 Camera::GetUp() const {
    return up;
}

/**
 * Gets view matrix
*/
glm::mat4 Camera::GetViewMatrix() const {
    // we send position, target, and genric up
    return glm::lookAt(pos, pos + forward, glm::vec3(0, 1, 0));
}

/**
 * Handles rotation in local Y-axis
*/
void Camera::JawRotate(const float& angle) {
    glm::vec3 newForward(forward);
    float rad;

    rotationEulers.y += angle;
    rad = glm::radians(rotationEulers.y);

    newForward.x = cos(rad);
    newForward.z = sin(rad);

    forward = newForward;

    right = glm::cross(forward, up);

    std::cout << "[CAMERA::ROT] Y=" << rotationEulers.y << std::endl;
}

/**
 * Handles rotation in local X-axis
*/
void Camera::PitchRotate(const float& angle) {
    glm::vec3 newForward(forward);
    float rad;

    rotationEulers.x += angle;

    if (rotationEulers.x > 89 && angle > 0)
        // avoid flipping over when looking upwards
        rotationEulers.x = 89.0f;
    else if (rotationEulers.x < -89 && angle < 0)
        // avoid flipping over when looking downwards
        rotationEulers.x = -89.0f;
    rad = glm::radians(rotationEulers.x);

    newForward.z = cos(rad) * CustomOps::SignBin(forward.z);
    newForward.y = sin(rad);

    forward = newForward;
    up = glm::cross(right, forward);
}