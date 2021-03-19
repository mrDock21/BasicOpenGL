#pragma once

#include <iostream>

#include <glad/include/glad/glad.h>

#include "src/vertex_buffer.hpp"

class ElementBuffer {
    public:
        ElementBuffer();
        ElementBuffer(uint[], const u_long&, const BufferUsage&);
        void Use() const;
    private:
        uint ID;
};