#include "src/components/transform.hpp"

Components::Transform::Transform() 
: position(0.0f), scale(1.0f), rotationEulers(0.0f, -90.0f, 0.0f),
  worldRotationEulers(0.0f),
  forward(0, 0, -1), right(1, 0, 0), up(0, 1, 0) { }

Components::Transform::Transform(const Components::Transform& other)
: position(other.position), scale(other.scale), rotationEulers(other.rotationEulers),
  worldRotationEulers(other.worldRotationEulers),
  forward(other.forward), right(other.right), up(other.up) { }

/**
 * Translates position by given direction
*/
void Components::Transform::Translate(const glm::vec3& dir) {
    position += dir;
}

/**
 * Updates position with new given
*/
void Components::Transform::SetPosition(const glm::vec3& newPos) {
    position.x = newPos.x;
    position.y = newPos.y;
    position.z = newPos.z;
}

/**
 * Updates scale with new given
*/
void Components::Transform::SetScale(const glm::vec3& newScale) {
    scale.x = newScale.x;
    scale.y = newScale.y;
    scale.z = newScale.z;
}

/**
 * Updates scale uniformly
*/
void Components::Transform::SetScale(const float& s) {
    scale = s * scale;
}

/**
 * Overrides current rotation with given
*/
void Components::Transform::SetRotation(const glm::vec3& rot) {
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
void Components::Transform::Rotate(const glm::vec3& deltaRot) {

    glm::vec3 rads( glm::radians(rotationEulers + deltaRot) );
    // forward
    forward = ComputeForward(rads.x, rads.y);
    // right
    right = ComputeRight(rads.z, rads.y);

    // update up
    up = glm::cross(right, forward);
    rotationEulers += deltaRot;
}

void Components::Transform::RotateWorldSpace(const glm::vec3& rotDeltas) {
    worldRotationEulers += rotDeltas;
}

/**
 * Gets position's copy
*/
glm::vec3 Components::Transform::Position() const { return position; }

/**
 * Gets scale's copy
*/
glm::vec3 Components::Transform::Scale() const { return scale;  }

/**
 * Gets rotation's copy
*/
glm::vec3 Components::Transform::Rotation() const { return rotationEulers; }

/**
 * Gets rotation in world space as copy
*/
glm::vec3 Components::Transform::WorldSpaceRotation() const { return worldRotationEulers; }

/**
 * Gets forward's vector copy
*/
glm::vec3 Components::Transform::Forward() const { return forward; }

/**
 * Gets right's vector copy
*/
glm::vec3 Components::Transform::Right() const { return right;  }

/**
 * Gets up's vector copy
*/
glm::vec3 Components::Transform::Up() const { return up; }

/**
 * Gets Model matrix of this transform
*/
glm::mat4 Components::Transform::ModelMatrix() const {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

    model = glm::rotate(model, rotationEulers.z, forward);
    model = glm::rotate(model, rotationEulers.x, right);
    model = glm::rotate(model, rotationEulers.y, up);

    model = glm::scale(model, scale);

    return model;
}

/**
 * Computes new forward vector based on given rotation (radians)
 * @param radX Rotation on X axis
 * @param radY Rotation on Y axis
*/
glm::vec3 Components::Transform::ComputeForward(float radX, float radY) {
    glm::vec3 newForward;
    // forward
    newForward.x = cos(radY);
    newForward.z = sin(radY) * cos(radX);
    newForward.y = sin(radX);

    return newForward;
}

/**
 * Computes new right vector based on given rotation (radians)
 * @param radZ Rotation on X axis
 * @param radY Rotation on Y axis
*/
glm::vec3 Components::Transform::ComputeRight(float radZ, float radY) {
    glm::vec3 newRight;
    // right
    newRight.z = -cos(radY);
    newRight.x = -sin(radY) * cos(radZ);
    newRight.y = sin(radZ);
    return newRight;
}

Components::Transform& Components::Transform::operator= (const Components::Transform& other) {
    this->position = other.position;
    this->scale = other.scale;
    this->rotationEulers = other.rotationEulers;
    this->forward = other.forward;
    this->right = other.right;
    this->up = other.up;
    this->worldRotationEulers = other.worldRotationEulers;
    return *this;
}