#include "data.hpp"
#include "toto-engine/window.hpp"
#include <GLFW/glfw3.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

void EventData::setCallbacks() {
    glfwSetWindowUserPointer(window.handle(), this);
    glfwSetFramebufferSizeCallback(window.handle(), [](GLFWwindow* window, int width, int height) {
        auto data = static_cast<EventData*>(glfwGetWindowUserPointer(window));
        data->camera.setPerspective(glm::radians(45.0f), width / static_cast<float>(height), 0.1f, 100.0f);
        data->renderer.setCamera(data->camera);
        data->renderer.setViewport(0, 0, width, height);
        data->width = width;
        data->height = height;
    });
}

void initImGui(const toto::Window& window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.handle(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void drawImGui(ImGuiData& data, double delta) {
    auto [index, width, meshes] = data;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::Begin(
        "Benchmark", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoFocusOnAppearing
    );
    ImGui::Text("FPS: %.1f", 1.0f / delta);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(width, 0), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::Begin(
        "Material", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoFocusOnAppearing
    );

    ImGui::InputInt("Mesh", reinterpret_cast<int*>(&index));
    if (index < 0) {
        index = 0;
    } else if (index >= meshes.size()) {
        index = meshes.size() - 1;
    }
    ImGui::ColorEdit3("Albedo", &meshes[index].get().material.albedo.x);
    ImGui::SliderFloat("Roughness", &meshes[index].get().material.roughness, 0.0f, 1.0f);
    ImGui::SliderFloat("Metallic", &meshes[index].get().material.metallic, 0.0f, 1.0f);
    ImGui::SliderFloat("AO", &meshes[index].get().material.ao, 0.0f, 1.0f);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
