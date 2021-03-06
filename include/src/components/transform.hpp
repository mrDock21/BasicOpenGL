#pragma once

#include <iostream>

#include "src/components/vector4.hpp"
#include "src/components/vector3.hpp"
#include "src/components/quaternion.hpp"
#include "src/components/matrix4.hpp"

namespace Components {
    /**
     * Stores position, scale and orientation of owner
    */
    class Transform {
    public:
        Transform();
        Transform(const Transform&);

        void Translate(const Vector3&);
        void SetPosition(const Vector3&);
        void SetScale(const Vector3&);
        void SetScale(const float&);
        void SetEulerAngles(const Vector3&);
        void RotateEulers(const Vector3&);
        void SetRotation(const Quaternion&);
        void Rotate(const Quaternion&);
        void Rotate(const float&, const Vector3&);
        void SetGlobalRotation(const Quaternion&);
        

        Vector3 Position() const;
        Vector3 Scale() const;
        Vector3 EulerAngles() const;
        Vector3 GlobalEulersAngles() const;
        Vector3 Forward() const;
        Vector3 Right() const;
        Vector3 Up() const;
        Quaternion Rotation() const;
        Quaternion GlobalRotation() const;
        Matrix4 ModelMatrix() const;

        Transform& operator = (const Transform&);
    private:
        Vector3 position, scale;
        Vector3 forward, right, up;
        Quaternion rotation, globalRotation;
        Matrix4 rotationMatrix, globalRotationMatrix;

        void ComputeForwardAndRight();
    };
}