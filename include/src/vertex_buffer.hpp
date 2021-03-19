#pragma once

#include <iostream>

#include <glad/include/glad/glad.h>

enum class BufferUsage { 
    Static, Dynamic 
};

uint getGlBufferUsage(const BufferUsage&);

class VertexBuffer {
    public:
        VertexBuffer();
        VertexBuffer(float[], const u_long&, const BufferUsage&);
        void Use() const;
    private:
        uint ID;
};