#include "src/camera.hpp"

//using namespace CustomOps;

/**
 * Initializes camera at position (0, 0, 3) and looking towards negative Z
*/
Camera::Camera() 
    : transform()
      { }

/**
 * Initializes camera at given position
 * @param initialPos Initial position
*/
Camera::Camera(const glm::vec3& initialPos) : Camera() { 
    transform.SetPosition(initialPos);
}

void Camera::Move(const glm::vec3& dir) {
    transform.Translate(dir);
}

/**
 * Will rotate camera's local X axis with angle
 * @param angleX Angle (degrees) to rotate in X axis
*/
void Camera::RotatePitch(const float& angleX) {
    glm::quat oldRot = transform.Rotation();
    glm::vec3 forward,
              deltaRot(angleX,  0.0f, 0.0f);

    transform.RotateEulers(deltaRot);
    forward = transform.Forward();

    if (forward.y > 0.95f || forward.y < -0.95f)
        transform.SetRotation(oldRot);
}

/**
 * Will rotate camera's global Y axis with angle
 * @param angleY Angle (degrees) to rotate in global Y axis
*/
void Camera::RotateYaw(const float& angleY) {
    transform.Rotate(angleY, glm::vec3(0.0f, 1.0f, 0.0f));
}

/**
 * Will rotate camera's local Z axis with angle
 * @param angleZ Angle (degrees) to rotate in Z axis
*/
void Camera::RotateRoll(const float& angleZ) {
    transform.RotateEulers(glm::vec3(0.0f,  0.0f, angleZ));
}

const Components::Transform& Camera::Transform() const {
    return transform;
}

/**
 * Gets view matrix
*/
glm::mat4 Camera::GetViewMatrix() const {
    glm::vec3 pos(transform.Position()),
              forward(transform.Forward());
    // we send position, target, and genric up
    return glm::lookAt(pos, pos + forward, transform.Up());
}