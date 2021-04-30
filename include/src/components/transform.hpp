#pragma once

#include <iostream>

#include <vendor/glm/glm.hpp>
#include <vendor/glm/gtc/matrix_transform.hpp>

// TODO test this to see if this actualliy works...
namespace Components {
    class Transform {
    public:
        Transform();
        Transform(const Transform&);

        void Translate(const glm::vec3&);
        void SetPosition(const glm::vec3&);
        void SetScale(const glm::vec3&);
        void SetScale(const float&);
        void SetRotation(const glm::vec3&);
        void Rotate(const glm::vec3&);
        void RotateWorldSpace(const glm::vec3&);

        glm::vec3 Position() const;
        glm::vec3 Scale() const;
        glm::vec3 Rotation() const;
        glm::vec3 WorldSpaceRotation() const;
        glm::vec3 Forward() const;
        glm::vec3 Right() const;
        glm::vec3 Up() const;
        glm::mat4 ModelMatrix() const;

        Transform& operator = (const Transform&);
    private:
        glm::vec3 position, scale, rotationEulers;
        glm::vec3 worldRotationEulers;
        glm::vec3 forward, right, up;

        static glm::vec3 ComputeForward(float, float);
        static glm::vec3 ComputeRight(float, float);
    };
}