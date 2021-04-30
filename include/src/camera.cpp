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
 * Will rotate camera's forward with given angles
 * @param rot Delta euler angles rotation
*/
void Camera::Rotate(const glm::vec3& rot) {
    glm::vec3 rotationEulers = transform.Rotation();
    float angleX(rot.x), angleY(rot.y);

    if (angleY != 0 || angleX != 0) {
        rotationEulers.y += angleY;
        rotationEulers.x += angleX;

        if (rotationEulers.x > 89 && angleX > 0)
            rotationEulers.x = 89.0f;
        else if (rotationEulers.x < -89 && angleX < 0)
            rotationEulers.x = -89.0f;

        transform.SetRotation(rotationEulers);
    }
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
    return glm::lookAt(pos, pos + forward, glm::vec3(0, 1, 0));
}