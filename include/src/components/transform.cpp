#include "src/components/transform.hpp"

Transform::Transform() 
: position(0.0f), scale(1.0f), rotationEulers(0.0f, -90.0f, 0.0f),
  worldRotationEulers(0.0f),
  forward(0, 0, -1), right(1, 0, 0), up(0, 1, 0) { }

/**
 * Translates position by given direction
*/
void Transform::Translate(const glm::vec3& dir) {
    position += dir;
}

/**
 * Updates position with new given
*/
void Transform::SetPosition(const glm::vec3& newPos) {
    position.x = newPos.x;
    position.y = newPos.y;
    position.z = newPos.z;
}

/**
 * Updates scale with new given
*/
void Transform::SetScale(const glm::vec3& newScale) {
    scale.x = newScale.x;
    scale.y = newScale.y;
    scale.z = newScale.z;
}

/**
 * Updates scale uniformly
*/
void Transform::SetScale(const float& s) {
    scale = s * scale;
}

void Transform::SetRotation(const glm::vec3& rot) {
    glm::vec3 rads(glm::radians(rot));
    rotationEulers.x = rot.x;
    rotationEulers.y = rot.y;
    rotationEulers.z = rot.z;

    forward = ComputeForward(rads.x, rads.y);
    right = ComputeRight(rads.z, rads.y);
    // update up
    up = glm::cross(right, forward);
}

/**
 * Rotates scale by amount given
*/
void Transform::Rotate(const glm::vec3& deltaRot) {

    glm::vec3 rads( glm::radians(rotationEulers + deltaRot) );
    // forward
    forward = ComputeForward(rads.x, rads.y);
    // right
    right = ComputeRight(rads.z, rads.y);

    // update up
    up = glm::cross(right, forward);
    rotationEulers += deltaRot;
}

void Transform::RotateWorldSpace(const glm::vec3& rotDeltas) {
    worldRotationEulers += rotDeltas;
}

/**
 * Gets position's copy
*/
glm::vec3 Transform::Position() const { return position; }

/**
 * Gets scale's copy
*/
glm::vec3 Transform::Scale() const { return scale;  }

/**
 * Gets rotation's copy
*/
glm::vec3 Transform::Rotation() const { return rotationEulers; }

/**
 * Gets rotation in world space as copy
*/
glm::vec3 Transform::WorldSpaceRotation() const { return worldRotationEulers; }

/**
 * Gets forward's vector copy
*/
glm::vec3 Transform::Forward() const { return forward; }

/**
 * Gets right's vector copy
*/
glm::vec3 Transform::Right() const { return right;  }

/**
 * Gets up's vector copy
*/
glm::vec3 Transform::Up() const { return up; }

glm::mat4 Transform::ModelMatrix() const {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

    model = glm::rotate(model, rotationEulers.z, forward);
    model = glm::rotate(model, rotationEulers.x, right);
    model = glm::rotate(model, rotationEulers.y, up);

    model = glm::scale(model, scale);

    return model;
}

glm::vec3 Transform::ComputeForward(float radX, float radY) {
    glm::vec3 newForward;
    // forward
    newForward.x = cos(radY);
    newForward.z = sin(radY) * cos(radX);
    newForward.y = sin(radX);

    return newForward;
}

glm::vec3 Transform::ComputeRight(float radZ, float radY) {
    glm::vec3 newRight;
    // right
    newRight.z = -cos(radY);
    newRight.x = -sin(radY) * cos(radZ);
    newRight.y = sin(radZ);
    return newRight;
}