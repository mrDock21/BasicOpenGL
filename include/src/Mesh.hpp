#pragma once

#include <iostream>

#include <glad/include/glad/glad.h>

#include "src/vertex_buffer.hpp"
#include "src/element_buffer.hpp"
#include "src/Shader.hpp"
#include "src/Texture.hpp"

class Mesh {
    public:
        Mesh();
        Mesh(float[], const u_long&, const u_long&, uint[], const u_long&);
        Mesh(float[], const u_long&, const u_long&);
        void SetShader(const Shader&);
        void SetTexture(const Texture&);
        void SetAttribute(const int&, const int&, const uint&, const bool&, const u_long&, const u_long&) const;
        void Use() const;
        void Render() const;
        void Draw() const;

        const Shader& GetShader();
        const Texture& GetTexture();
    private:
        uint VAO;
        Shader shader;
        Texture texture;
        VertexBuffer VBO;
        ElementBuffer EBO;
        bool usesEBO;
};