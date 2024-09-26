#include "toto-engine/gl/glresources.hpp"
#include <glm/glm.hpp>
#include <string>
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

    std::vector<Vertex> vertices = {
        {  {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        { {1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        { {-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
    };
    std::vector<uint> indices = {0, 1, 3, 1, 2, 3};

    std::string vertex_shader_source = R"glsl(#version 460
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

out vec3 frag_normal;
out vec2 frag_texcoord;

void main() {
    gl_Position = vec4(position, 1.0);
    frag_normal = normal;
    frag_texcoord = texcoord;
})glsl";
    std::string fragment_shader_source = R"glsl(#version 460
in vec3 frag_normal;
in vec2 frag_texcoord;

out vec4 frag_color;

void main() {
    frag_color = vec4(frag_normal, 1.0);
})glsl";

    auto vbo = GLBuffer();
    auto vao = GLVertexArray();
    auto ibo = GLBuffer();
    vbo.bind(GL_ARRAY_BUFFER);
    vbo.data(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);
    vao.bind();
    vao.setVertexAttrib<Vertex, glm::vec3, offsetof(Vertex, position)>(0);
    vao.setVertexAttrib<Vertex, glm::vec3, offsetof(Vertex, normal)>(1);
    vao.setVertexAttrib<Vertex, glm::vec2, offsetof(Vertex, texcoord)>(2);
    ibo.bind(GL_ELEMENT_ARRAY_BUFFER);
    ibo.data(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

    auto vertex_shader = GLShader<GLShaderType::Vertex>();
    vertex_shader.source(vertex_shader_source);
    vertex_shader.compile();

    auto fragment_shader = GLShader<GLShaderType::Fragment>();
    fragment_shader.source(fragment_shader_source);
    fragment_shader.compile();

    auto program = GLProgram();
    program.attachShaders(vertex_shader, fragment_shader);
    program.link();

    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        window.swapBuffers();
        window.pollEvents();
    }
    return 0;
}
