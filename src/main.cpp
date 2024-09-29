#include "toto-engine/utils/renderer.hpp"
#include <GLFW/glfw3.h>
#include <toto-engine/gl/globjects.hpp>
#include <toto-engine/gl/glresources.hpp>
#include <toto-engine/import-gl.hpp>
#include <toto-engine/mesh.hpp>
#include <toto-engine/utils/shapes.hpp>
#include <toto-engine/window.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

using namespace toto;

int main(int argc, const char* argv[]) {
    auto window = Window(800, 600, "Hello, World!");
    window.makeContextCurrent();

    Window::initGL();

    Renderer renderer;

    renderer.useProgram();

    auto model = torus(1, .33333333, 96, 32);
    auto material = Material {
        .albedo = glm::vec3(1.0f, 0.5f, 0.0f),
        .metallic = 1.0f,
        .roughness = 0.25f,
        .ao = 0.05f,
    };

    glm::mat4 model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view_matrix = glm::lookAt(glm::vec3(4.0f, 4.0f, 4.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::vec3 light_direction = glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f));
    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    renderer.setModelMatrix(model_matrix);
    renderer.setViewMatrix(view_matrix);
    renderer.setProjectionMatrix(projection_matrix);
    renderer.setMaterial(material);
    renderer.setLight(light_direction, 1.0f, light_color);

    struct UserData {
        Renderer& renderer;
    } data {renderer};
    glfwSetWindowUserPointer(window.handle(), &data);
    glfwSetWindowSizeCallback(window.handle(), [](GLFWwindow* window, int width, int height) {
        auto& data = *static_cast<UserData*>(glfwGetWindowUserPointer(window));
        data.renderer.setProjectionMatrix(
            glm::perspective(glm::radians(45.0f), width / static_cast<float>(height), 0.1f, 100.0f)
        );
        glViewport(0, 0, width, height);
    });

    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.handle(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    auto start = glfwGetTime();
    auto last = start;
    while (!window.shouldClose()) {
        auto now = glfwGetTime();
        auto time = now - start;
        auto delta = now - last;

        auto time2 = time * 0.5f;
        auto time3 = -time * 0.5f;

        model_matrix = glm::rotate<float>(glm::mat4(1.0f), time3, glm::vec3(1.0f, 0.0f, 0.0f));
        light_direction = glm::normalize(glm::vec3(glm::cos(time2), glm::sin(time2), -1.0f));
        view_matrix = glm::lookAt(
            glm::vec3(4.0f * glm::cos(time), 2.0f, 4.0f * glm::sin(time)), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
        );
        renderer.setModelMatrix(model_matrix);
        renderer.setViewMatrix(view_matrix);

        renderer.setMaterial(material);
        renderer.setLight(light_direction, 1.0f, light_color);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.render(model);

        //

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::Begin(
            "Material", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize
        );

        ImGui::ColorEdit3("Albedo", &material.albedo.x);
        ImGui::SliderFloat("Roughness", &material.roughness, 0.0f, 1.0f);
        ImGui::SliderFloat("Metallic", &material.metallic, 0.0f, 1.0f);
        ImGui::SliderFloat("AO", &material.ao, 0.0f, 1.0f);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffers();
        window.pollEvents();
        last = now;
    }
    return 0;
}
