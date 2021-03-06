#pragma once

#include <vendor/glm/gtc/matrix_transform.hpp>

#include "src/components/vector3.hpp"
#include "src/components/vector4.hpp"
#include "src/components/quaternion.hpp"

/**
 * Represents a (4x4) matrix
*/
class Matrix4 {

    // to be able to set uniforms in shaders
    friend class Shader;

    public:
        Matrix4();
        Matrix4(const Matrix4&);

        const Matrix4& operator = (const Matrix4&);
        const Matrix4& operator *= (const Matrix4&);
        friend Matrix4 operator * (Matrix4, const Matrix4&);
        
        static Vector4 Multiply(const Matrix4&, const Vector4&);
        static Matrix4 Indentity();
        static Matrix4 FromQuaternion(const Quaternion&);
        static Matrix4 Translate(const Matrix4&, const Vector3&);
        static Matrix4 Scale(const Matrix4&, const Vector3&);
        static Matrix4 Rotate(const Matrix4&, const float&, const Vector3&);
        static Matrix4 LookAt(const Vector3&, const Vector3&, const Vector3&);
        static Matrix4 Perspective(const float&, const float&, const float&, const float&);
        
    private:
        glm::mat4 matrix;

        Matrix4(const glm::mat4&);
};