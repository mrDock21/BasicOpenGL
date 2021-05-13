#include "src/components/transform.hpp"

Components::Transform::Transform() 
: position(0.0f), scale(1.0f) {
    glm::quat identity = glm::quat();
    // initialize both rotations in zeros
    SetRotation(identity);
    SetGlobalRotation(identity);
}

Components::Transform::Transform(const Components::Transform& other)
: position(other.position), scale(other.scale),
  forward(other.forward), right(other.right), up(other.up), 
  rotation(other.rotation), globalRotation(other.globalRotation), 
  rotationMatrix(other.rotationMatrix), globalRotationMatrix(other.globalRotationMatrix) { }

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
 * Overrides current rotation with given, in degrees
*/
void Components::Transform::SetEulerAngles(const glm::vec3& eulers) {
    SetRotation(glm::quat(glm::radians(eulers)));
}

/**
 * Rotates in LOCAL space by amount euler angles given
 * @param deltaEulers Rotation made (euler angles) in degrees
*/
void Components::Transform::RotateEulers(const glm::vec3& deltaEulers) {
    glm::quat rot(glm::radians(deltaEulers));
    Rotate(rot);
}

/**
 * Overrides current rotation with given
*/
void Components::Transform::SetRotation(const glm::quat& rot) {

    rotation = rot;
    rotationMatrix = glm::toMat4(rotation);

    ComputeForwardAndRight();
}

/**
 * Rotates in LOCAL space by amount given
 * @param deltaRot Rotation made (quaternion)
*/
void Components::Transform::Rotate(const glm::quat& deltaRot) {
    rotation = rotation * deltaRot;
    rotationMatrix = glm::toMat4(rotation);

    ComputeForwardAndRight();
}

/**
 * Rotates in GLOBAL space by angle in given axis
 * @param angleDelta angle (degrees) to rotate
 * @param axis Rotation will be done around given axis
*/
void Components::Transform::Rotate(const float& angleDelta, const glm::vec3& axis) {
    globalRotation = globalRotation * glm::angleAxis(glm::radians(angleDelta), axis);
    globalRotationMatrix = glm::toMat4(globalRotation);

    ComputeForwardAndRight();
}

void Components::Transform::SetGlobalRotation(const glm::quat& globalRot) {
    globalRotation = globalRot;
    globalRotationMatrix = glm::toMat4(globalRotation);

    ComputeForwardAndRight();
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
glm::quat Components::Transform::Rotation() const { return rotation; }

/**
 * Gets local rotation as euler angles
*/
glm::vec3 Components::Transform::EulerAngles() const { 
    return glm::degrees( glm::eulerAngles(rotation) ); 
}

/**
 * Gets rotation in world space as copy
*/
glm::quat Components::Transform::GlobalRotation() const { return globalRotation; }

/**
 * Gets global rotation as euler angles
*/
glm::vec3 Components::Transform::GlobalEulersAngles() const {
    return glm::degrees( glm::eulerAngles(globalRotation) );
}

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
    // at last: translate
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    // third: global space rotation
    model = model * globalRotationMatrix;
    // second: rotate in local space
    model = model * rotationMatrix;
    // first scale
    model = glm::scale(model, scale);

    return model;
}

/**
 * Computes new forward vector based on given rotation (radians)
 * @param radX Rotation on X axis
 * @param radY Rotation on Y axis
*/
void Components::Transform::ComputeForwardAndRight() {
    glm::mat4 rotMat = globalRotationMatrix * rotationMatrix;
    // rotate global forward by local rot
    forward = rotMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    // rotate global right by local rot
    right = rotMat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    // rotate global up by local rot
    up = rotMat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}

Components::Transform& Components::Transform::operator= (const Components::Transform& other) {
    this->position = other.position;
    this->scale = other.scale;

    this->rotation = other.rotation;
    this->rotationMatrix = other.rotationMatrix;

    this->globalRotation = other.globalRotation;
    this->globalRotationMatrix = other.globalRotationMatrix;

    this->forward = other.forward;
    this->right = other.right;
    this->up = other.up;
    return *this;
}