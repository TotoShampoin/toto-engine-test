#include "toto-engine/loader/shader.hpp"
#include "toto-engine/uniform.hpp"
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

    auto model = cube();

    auto program = loadRenderShaderSource(pbrMaterialVertexShader(), pbrMaterialFragmentShader());
    program.use();

    auto u_model = Uniform(program, "u_model");
    auto u_view = Uniform(program, "u_view");
    auto u_projection = Uniform(program, "u_projection");

    glm::mat4 model_matrix = glm::mat4(1.0f);
    glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    u_model.set(model_matrix);
    u_view.set(view_matrix);
    u_projection.set(projection_matrix);

    glEnable(GL_DEPTH_TEST);

    auto start = glfwGetTime();
    auto last = start;
    while (!window.shouldClose()) {
        auto now = glfwGetTime();
        auto time = now - start;
        auto delta = now - last;

        model_matrix = glm::rotate<float>(model_matrix, glm::radians(90.0f) * delta, glm::vec3(0.0f, 1.0f, 0.0f));
        u_model.set(model_matrix);

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
