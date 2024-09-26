#include <glm/glm.hpp>
#include <sys/types.h>
#include <toto-engine/gl/globjects.hpp>
#include <toto-engine/window.hpp>
#include <vector>

using namespace toto;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};

int main(int argc, const char* argv[]) {
    auto window = Window(800, 600, "Hello, World!");
    window.makeContextCurrent();

    Window::initGL();

    std::vector<Vertex> vertices = {};
    std::vector<uint> indices = {};

    GLBuffer vbo;
    GLVertexArray vao;
    GLBuffer ibo;

    vbo.bind(GL_ARRAY_BUFFER);
    vbo.data(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);
    vao.bind();
    vao.setVertexAttrib<Vertex, glm::vec3, offsetof(Vertex, position)>(0);
    vao.setVertexAttrib<Vertex, glm::vec3, offsetof(Vertex, normal)>(1);
    vao.setVertexAttrib<Vertex, glm::vec2, offsetof(Vertex, texcoord)>(2);
    ibo.bind(GL_ELEMENT_ARRAY_BUFFER);
    ibo.data(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        window.swapBuffers();
        window.pollEvents();
    }
    return 0;
}
