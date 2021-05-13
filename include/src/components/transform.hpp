#pragma once

#include <iostream>

#include <vendor/glm/glm.hpp>
#include <vendor/glm/gtc/matrix_transform.hpp>
#include <vendor/glm/gtc/quaternion.hpp>
#include <vendor/glm/gtx/quaternion.hpp>

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
        void SetEulerAngles(const glm::vec3&);
        void RotateEulers(const glm::vec3&);
        void SetRotation(const glm::quat&);
        void Rotate(const glm::quat&);
        void Rotate(const float&, const glm::vec3&);
        void SetGlobalRotation(const glm::quat&);
        

        glm::vec3 Position() const;
        glm::vec3 Scale() const;
        glm::vec3 EulerAngles() const;
        glm::vec3 GlobalEulersAngles() const;
        glm::vec3 Forward() const;
        glm::vec3 Right() const;
        glm::vec3 Up() const;
        glm::quat Rotation() const;
        glm::quat GlobalRotation() const;
        glm::mat4 ModelMatrix() const;

        Transform& operator = (const Transform&);
    private:
        glm::vec3 position, scale;
        glm::vec3 forward, right, up;
        glm::quat rotation, globalRotation;
        glm::mat4 rotationMatrix, globalRotationMatrix;

        void ComputeForwardAndRight();
    };
}