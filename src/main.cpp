#include <toto-engine/gl/globjects.hpp>
#include <toto-engine/gl/glresources.hpp>
#include <toto-engine/import-gl.hpp>
#include <toto-engine/mesh.hpp>
#include <toto-engine/utils/shapes.hpp>
#include <toto-engine/window.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace toto;

int main(int argc, const char* argv[]) {
    auto window = Window(800, 600, "Hello, World!");
    window.makeContextCurrent();

    Window::initGL();

    // auto model = quad();
    auto model = cube();

    auto vertex_shader = GLShader<GLShaderType::Vertex>();
    vertex_shader.source(pbrMaterialVertexShader());
    vertex_shader.compile();

    auto fragment_shader = GLShader<GLShaderType::Fragment>();
    fragment_shader.source(pbrMaterialFragmentShader());
    fragment_shader.compile();

    auto program = GLProgram();
    program.attachShaders(vertex_shader, fragment_shader);
    program.link();
    program.use();

    uint u_model = glGetUniformLocation(program.handle(), "u_model");
    uint u_view = glGetUniformLocation(program.handle(), "u_view");
    uint u_projection = glGetUniformLocation(program.handle(), "u_projection");

    glm::mat4 model_matrix = glm::mat4(1.0f);
    glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(u_model, 1, GL_FALSE, &model_matrix[0][0]);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, &view_matrix[0][0]);
    glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection_matrix[0][0]);

    glEnable(GL_DEPTH_TEST);

    auto start = glfwGetTime();
    auto last = start;
    while (!window.shouldClose()) {
        auto now = glfwGetTime();
        auto time = now - start;
        auto delta = now - last;

        model_matrix = glm::rotate<float>(model_matrix, glm::radians(90.0f) * delta, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(u_model, 1, GL_FALSE, &model_matrix[0][0]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model.vbo.bind();
        program.use();
        glDrawElements(GL_TRIANGLES, model.index_count, GL_UNSIGNED_INT, nullptr);

        window.swapBuffers();
        window.pollEvents();
        last = now;
    }
    return 0;
}
