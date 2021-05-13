#pragma once

#include <iostream>
#include <math.h>

#include <vendor/glm/glm.hpp>
#include <vendor/glm/gtc/matrix_transform.hpp>

#include "src/number_operations.hpp"
#include "src/components/transform.hpp"

class Camera {
    public:

        Camera();
        Camera(const glm::vec3&);
        void Move(const glm::vec3&);
        void RotateYaw(const float&);
        void RotatePitch(const float&);
        void RotateRoll(const float&);

        const Components::Transform& Transform() const;

        glm::mat4 GetViewMatrix() const;

    private:
        Components::Transform transform;

        const float MAX_INCLINATION = 0.93f;
        const float MIN_INCLINATION = -0.93f;
};