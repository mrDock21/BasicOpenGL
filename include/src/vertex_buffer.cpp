#include "src/vertex_buffer.hpp"

uint getGlBufferUsage(const BufferUsage& usage) {
    switch (usage)
    {
        case BufferUsage::Static:
            return GL_STATIC_DRAW;
        case BufferUsage::Dynamic:
            return GL_DYNAMIC_DRAW;
    }
    return 0;
}

VertexBuffer::VertexBuffer() {

}

/**
 * Constructs a new vertex buffer object
 * @param verts Vertices of this vertex buffer
 * @param size byte size of "verts"
 * @param glUsage OpenGL draw mode constant
 */
VertexBuffer::VertexBuffer(float* verts, const u_long& size, const BufferUsage& glUsage) {
    // vertex buffer
    glGenBuffers(1, &ID);
    Use();
    glBufferData(GL_ARRAY_BUFFER, size, verts, getGlBufferUsage(glUsage));
}

/**
 * Binds this VertexBuffer
*/
void VertexBuffer::Use() const {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}