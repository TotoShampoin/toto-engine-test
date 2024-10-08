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
        data->camera.setPerspective(glm::radians(data->camera_fov), width / static_cast<float>(height), 0.1f, 100.0f);
        data->renderer.setCamera(data->camera);
        data->renderer.setViewport(0, 0, width, height);
        data->width = width;
        data->height = height;
    });
    glfwSetKeyCallback(window.handle(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto data = static_cast<EventData*>(glfwGetWindowUserPointer(window));
        if (key == GLFW_KEY_W) {
            data->up = action != GLFW_RELEASE;
        }
        if (key == GLFW_KEY_S) {
            data->down = action != GLFW_RELEASE;
        }
        if (key == GLFW_KEY_A) {
            data->left = action != GLFW_RELEASE;
        }
        if (key == GLFW_KEY_D) {
            data->right = action != GLFW_RELEASE;
        }
        if (key == GLFW_KEY_ESCAPE) {
            data->escape_pressed = action == GLFW_PRESS;
        }
    });
    glfwSetMouseButtonCallback(window.handle(), [](GLFWwindow* window, int button, int action, int mods) {
        auto data = static_cast<EventData*>(glfwGetWindowUserPointer(window));
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            data->mouse_left = action != GLFW_RELEASE;
            data->mouse_left_pressed = action == GLFW_PRESS;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            data->mouse_right = action != GLFW_RELEASE;
            data->mouse_right_pressed = action == GLFW_PRESS;
        }
    });
    glfwSetCursorPosCallback(window.handle(), [](GLFWwindow* window, double xpos, double ypos) {
        auto data = static_cast<EventData*>(glfwGetWindowUserPointer(window));
        data->mouse_position = glm::vec2(xpos, ypos);
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
