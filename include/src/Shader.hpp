#pragma once

#include <iostream>
#include <string>
#include <glad/include/glad/glad.h>

#include <vendor/glm/glm.hpp>
#include <vendor/glm/gtc/type_ptr.hpp>

class Shader {
    public:
        Shader();
        Shader(const std::string&, const std::string&);
        void Use() const;
        void SetFloat(const std::string&, float) const;
        void SetInt(const std::string&, int) const;
        void SetVec2(const std::string&, float, float) const;
        void SetVec3(const std::string&, float, float, float) const;
        void SetVec4(const std::string&, float, float, float, float) const;
        void SetVec2(const std::string&, const glm::vec2&) const;
        void SetVec3(const std::string&, const glm::vec3&) const;
        void SetVec4(const std::string&, const glm::vec4&) const;
        void SetMat4x4(const std::string&, const glm::mat4x4&) const;
    private:
        uint CompileShader(const std::string&, uint) const;
        bool OperationSucceded() const;
        bool OperationSucceded(uint) const;

        uint _ProgramID;
};
