#include "toto-engine/loader/shader.hpp"
#include "toto-engine/uniform.hpp"
#include <GLFW/glfw3.h>
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

    auto model = torus(1, .5, 96, 48);

    auto program = loadRenderShaderSource(pbrMaterialVertexShader(), pbrMaterialFragmentShader());
    program.use();

    auto u_model = Uniform(program, "u_model");
    auto u_view = Uniform(program, "u_view");
    auto u_projection = Uniform(program, "u_projection");

    auto u_albedo = Uniform(program, "u_albedo");
    auto u_roughness = Uniform(program, "u_roughness");
    auto u_metallic = Uniform(program, "u_metallic");
    auto u_ao = Uniform(program, "u_ao");
    auto u_light_direction = Uniform(program, "u_light_direction");
    auto u_light_color = Uniform(program, "u_light_color");

    glm::mat4 model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view_matrix = glm::lookAt(glm::vec3(4.0f, 4.0f, 4.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    u_model.set(model_matrix);
    u_view.set(view_matrix);
    u_projection.set(projection_matrix);

    u_albedo.set(glm::vec3(1.0f, 0.5f, 0.0f));
    u_roughness.set(0.25f);
    u_metallic.set(1.0f);
    u_ao.set(0.05f);
    u_light_direction.set(glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)));
    u_light_color.set(glm::vec3(1.0f, 1.0f, 1.0f));

    struct UserData {
        Uniform& u_projection;
    } data {u_projection};
    glfwSetWindowUserPointer(window.handle(), &data);
    glfwSetWindowSizeCallback(window.handle(), [](GLFWwindow* window, int width, int height) {
        auto& data = *static_cast<UserData*>(glfwGetWindowUserPointer(window));
        data.u_projection.set(glm::perspective(glm::radians(45.0f), width / static_cast<float>(height), 0.1f, 100.0f));
        glViewport(0, 0, width, height);
    });

    glEnable(GL_DEPTH_TEST);

    auto start = glfwGetTime();
    auto last = start;
    while (!window.shouldClose()) {
        auto now = glfwGetTime();
        auto time = now - start;
        auto delta = now - last;

        auto time2 = time * 0.5f;

        // model_matrix = glm::rotate<float>(model_matrix, glm::radians(90.0f) * delta, glm::vec3(0.0f, 1.0f, 0.0f));
        // u_model.set(model_matrix);

        view_matrix = glm::lookAt(
            glm::vec3(4.0f * glm::cos(time), 4.0f, 4.0f * glm::sin(time)), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
        );
        u_view.set(view_matrix);

        u_light_direction.set(glm::normalize(glm::vec3(glm::cos(time2), -1.0f, glm::sin(time2))));

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
