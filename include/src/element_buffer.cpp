#include "src/element_buffer.hpp"

ElementBuffer::ElementBuffer() {

}

/**
 * Constructs a new vertex buffer object
 * @param indices Vertices of this vertex buffer
 * @param size byte size of "indices"
 * @param glUsage OpenGL draw mode constant
 */
ElementBuffer::ElementBuffer(uint* indices, const u_long& size, const BufferUsage& glUsage) {
    // vertex buffer
    glGenBuffers(1, &ID);
    Use();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, getGlBufferUsage(glUsage));
}

/**
 * Binds this ElementBuffer
*/
void ElementBuffer::Use() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}