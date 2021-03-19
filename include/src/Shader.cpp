#include "src/Shader.hpp"

Shader::Shader() : _ProgramID(0) { }

Shader::Shader(const std::string& vertexSrc, const std::string& fragSrc) {
    uint vertexShader = CompileShader(vertexSrc, GL_VERTEX_SHADER),
         fragmentShader = CompileShader(fragSrc, GL_FRAGMENT_SHADER);

    if (vertexShader == 0 || fragmentShader == 0) {
        std::cout << "[SHADER-COMPILE-ERROR] Couldn't compile shader source!" << std::endl;
        _ProgramID = 0;
        return;
    }
    _ProgramID = glCreateProgram();

    glAttachShader(_ProgramID, vertexShader);
    glAttachShader(_ProgramID, fragmentShader);
    glLinkProgram(_ProgramID);

    if (!OperationSucceded())
        std::cout << "[SHADER-CREATION-ERROR] Couldn't link program!" << std::endl;
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use() const {
    glUseProgram(_ProgramID);
}

void Shader::SetFloat(const std::string& uniform, float value) const {
    int location;
    Use();
    location = glGetUniformLocation(_ProgramID, uniform.c_str());
    glUniform1f(location, value);
}

void Shader::SetInt(const std::string& uniform, int value) const {
    int location;
    Use();
    location = glGetUniformLocation(_ProgramID, uniform.c_str());
    glUniform1i(location, value);
}

void Shader::SetVec2(const std::string& uniform, float x, float y) const {
    int location;
    Use();
    location = glGetUniformLocation(_ProgramID, uniform.c_str());
    glUniform2f(location, x, y);
}

void Shader::SetVec3(const std::string& uniform, float x, float y, float z) const {
    int location;
    Use();
    location = glGetUniformLocation(_ProgramID, uniform.c_str());
    glUniform3f(location, x, y, z);
}

void Shader::SetVec4(const std::string& uniform, float x, float y, float z, float w) const {
    int location;
    Use();
    location = glGetUniformLocation(_ProgramID, uniform.c_str());
    glUniform4f(location, x, y, z, w);
}

void Shader::SetMat4x4(const std::string& uniform, const glm::mat4x4& mat) const {
    int location;
    Use();
    location = glGetUniformLocation(_ProgramID, uniform.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

uint Shader::CompileShader(const std::string& shaderSrc, uint shaderType) const {
    uint id = glCreateShader(shaderType);
    const char* src = shaderSrc.c_str();

    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    if (!OperationSucceded(id))
        return 0;
    return id;
}

bool Shader::OperationSucceded() const {
    char infolog[512];
    int success;

    glGetShaderiv(_ProgramID, GL_LINK_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(_ProgramID, sizeof(infolog), NULL, infolog);
        std::cout << "[SHADER-PROGRAM-ERROR] " << infolog << std::endl;
        return false;
    }
    return true;
}

bool Shader::OperationSucceded(uint shaderID) const {
    char infolog[512];
    int success;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shaderID, sizeof(infolog), NULL, infolog);
        std::cout << "[SHADER-COMPILATION-ERROR] " << infolog << std::endl;
        return false;
    }
    return true;
}