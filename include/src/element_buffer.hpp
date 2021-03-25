#pragma once

#include <iostream>

#include <glad/include/glad/glad.h>

#include "src/vertex_buffer.hpp"

class ElementBuffer {
    public:
        ElementBuffer();
        ElementBuffer(uint[], const u_long&, const BufferUsage&);
        void Use() const;
        u_long GetArraySize() const;
        u_long GetBytesSize() const;
    private:
        uint ID;
        u_long arraySize, bytesSize;
};