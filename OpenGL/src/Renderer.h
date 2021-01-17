#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


#define ASSERT(x)   if(!(x)) __debugbreak();
#define GLCall(x)   GLClearError();\
                    x;\
                    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();

bool GLLogCall(const char* function, const char* file, unsigned int line);


class Renderer {

public:
    void Clear() const;
    void SetClearColor(float v1 = 0.0f, float v2 = 0.0f, float v3 = 0.0f, float v4 = 0.0f) const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};
