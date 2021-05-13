#pragma once

#include <iostream>

#include <glad/include/glad/glad.h>

#include "src/vertex_buffer.hpp"
#include "src/element_buffer.hpp"
#include "src/shading/material.hpp"
#include "src/Texture.hpp"
#include "src/components/transform.hpp"

class Mesh {
    public:
        Mesh();
        Mesh(float[], const u_long&, const u_long&, uint[], const u_long&);
        Mesh(float[], const u_long&, const u_long&);
        void SetMaterial(const Material&);
        void SetTexture(const Texture&);
        void SetAttribute(const int&, const int&, const uint&, const bool&, const u_long&, const u_long&) const;
        void Use() const;
        void Render() const;
        void Draw() const;

        template<class T>
        void SetUniform(const std::string&, const T&);

        const Components::Transform& Transform();
        const Texture& GetTexture() const;
    private:
        uint VAO;
        Components::Transform transform;
        Material material;
        Texture texture;
        VertexBuffer VBO;
        ElementBuffer EBO;
        bool usesEBO;
};

/**
 * Sets uniform in inner material
*/
template<class T>
void Mesh::SetUniform(const std::string& uniform, const T& value) {
    material.SetUniform<T>(uniform, value);
}