#pragma once

#include "src/components/transform.hpp"

class Camera {
    public:

        Camera();
        Camera(const Vector3&);
        void Move(const Vector3&);
        void RotateYaw(const float&);
        void RotatePitch(const float&);
        void RotateRoll(const float&);

        const Components::Transform& Transform() const;

        Matrix4 GetViewMatrix() const;

    private:
        Components::Transform transform;

        const float MAX_INCLINATION = 0.93f;
        const float MIN_INCLINATION = -0.93f;
};