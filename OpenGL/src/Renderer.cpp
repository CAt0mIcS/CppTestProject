#include "Renderer.h"

#include <iostream>

void GLClearError() {

    while (glGetError() != GL_NO_ERROR);

}


bool GLLogCall(const char* function, const char* file, unsigned int line) {

    while (GLenum error = glGetError()) {

        std::cout << "[OpenGL Error] (" << error << ") " << function << " " << file << ": " << line << '\n';
        return false;
    }
    return true;
}


void Renderer::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}


void Renderer::SetClearColor(float v1, float v2, float v3, float v4) const
{
    GLCall(glClearColor(v1, v2, v3, v4));
}


void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {

    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); //Count (6) = amount of indices to draw //Buffer is already bound, because of that: nullptr

}
