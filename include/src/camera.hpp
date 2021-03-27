#pragma once

#include <iostream>
#include <math.h>

#include <vendor/glm/glm.hpp>
#include <vendor/glm/gtc/matrix_transform.hpp>

#include "src/number_operations.hpp"

class Camera {
    public:

        Camera();
        Camera(const glm::vec3&);

        void Move(const glm::vec3&);
        void Rotate(const glm::vec3&);

        const glm::vec3& GetPosition() const;
        glm::vec3 GetForward() const;
        glm::vec3 GetRight() const;
        glm::vec3 GetUp() const;

        glm::mat4 GetViewMatrix() const;

    private:
        void JawRotate(const float&);
        void PitchRotate(const float&);

        glm::vec3 forward, right, up;
        glm::vec3 pos, rotationEulers;

        const float MAX_INCLINATION = 0.93f;
        const float MIN_INCLINATION = -0.93f;
};