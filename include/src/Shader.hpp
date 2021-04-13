#pragma once

#include <iostream>
#include <string>
#include <map>
#include <glad/include/glad/glad.h>

#include <vendor/glm/glm.hpp>
#include <vendor/glm/gtc/type_ptr.hpp>

class Shader {
    public:
        Shader();
        Shader(const std::string&, const std::string&);
        void Use() const;

        void SetUniform(const std::string&, const float&);
        void SetUniform(const std::string&, const int&);
        void SetUniform(const std::string&, const glm::vec2&);
        void SetUniform(const std::string&, const glm::vec3&);
        void SetUniform(const std::string&, const glm::vec4&);
        void SetUniform(const std::string&, const glm::mat4x4&);
    private:
        uint CompileShader(const std::string&, uint) const;
        bool OperationSucceded() const;
        bool OperationSucceded(uint) const;

        int GetUniformLocation(const std::string&);

        uint _ProgramID;
        std::map<std::string, int> uniforms;
};
