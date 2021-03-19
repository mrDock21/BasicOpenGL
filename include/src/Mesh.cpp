#include "src/Mesh.hpp"

Mesh::Mesh() { }

Mesh::Mesh(float* verts, const u_long& vertsSize, uint* indices, const u_long& indicesSize) {
    // vertex array object
    glGenVertexArrays(1, &VAO);
    Use();
    // vertex buffer
    VertexBuffer vbo(verts, vertsSize, BufferUsage::Static);
    vbo.Use();
    // elements (index) buffer
    // element array object -> Gets binded to current VAO
    ElementBuffer evo(indices, indicesSize, BufferUsage::Static);
    evo.Use();
}

void Mesh::SetShader(const Shader& s) {
    shader = s;
}

void Mesh::SetTexture(const Texture& t) {
    texture = t;
}

/**
 * Configures and enables given attribute's index
 * @param index Zero-based attribute index to enabled
 * @param componentCount Number of elements that forme a single entry of the attribute
 * @param glDataType Generic number type of OpenGL
 * @param normalized Wether the data is normalized or not
 * @param stride Bytes separation between each component
 * @param startOffset Initial offset (in bytes) of where the firs entry of component is found 
*/
void Mesh::SetAttribute(
    const int& index, 
    const int& componentCount, 
    const uint& glDataType, 
    const bool& normalized, 
    const u_long& stride, 
    const u_long& startOffset) const {
    
    // make sure VAO is binded
    Use();
    // attribute
    glVertexAttribPointer(index, componentCount, glDataType, normalized, stride, (void*) startOffset);
    // enable
    glEnableVertexAttribArray(index);
}

void Mesh::Use() const {
    glBindVertexArray(VAO);
}

void Mesh::Render() const {
    Use();
    shader.Use();
    texture.Use();
}

const Shader& Mesh::GetShader() {
    return shader;
}

const Texture& Mesh::GetTexture() {
    return texture;
}