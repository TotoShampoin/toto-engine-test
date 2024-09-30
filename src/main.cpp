#include <functional>
#include <glm/ext/quaternion_geometric.hpp>
#include <toto-engine/gl/globjects.hpp>
#include <toto-engine/gl/glresources.hpp>
#include <toto-engine/import-gl.hpp>
#include <toto-engine/mesh.hpp>
#include <toto-engine/utils/light.hpp>
#include <toto-engine/utils/renderer.hpp>
#include <toto-engine/utils/shapes.hpp>
#include <toto-engine/utils/transform.hpp>
#include <toto-engine/window.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <vector>

using namespace toto;

struct Mesh {
    Model model;
    Material material;
    Transform transform {};
};

int main(int argc, const char* argv[]) {
    auto window = Window(800, 600, "Hello, World!");
    window.makeContextCurrent();

    Window::initGL();

    auto renderer = Renderer();

    renderer.useProgram();

    Mesh torus = {
        shape::torus(1, .33333333, 96, 32),
        Material {
                  .albedo = glm::vec3(1.0f, 0.5f, 0.0f),
                  .metallic = 1.0f,
                  .roughness = 0.25f,
                  .ao = 0.05f,
                  },
    };
    Mesh cube = {
        shape::cube(1.0f, 1.0f, 1.0f),
        Material {
                  .albedo = glm::vec3(0.5f, 0.5f, 1.0f),
                  .metallic = 0.0f,
                  .roughness = 0.5f,
                  .ao = 1.0f,
                  },
    };

    std::vector<std::reference_wrapper<Mesh>> meshes {torus, cube};
    uint index = 0;

    auto camera = Camera::Perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    auto light = Light(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

    struct UserData {
        Camera& camera;
        Renderer& renderer;
    } data {camera, renderer};
    glfwSetWindowUserPointer(window.handle(), &data);
    glfwSetWindowSizeCallback(window.handle(), [](GLFWwindow* window, int width, int height) {
        auto& data = *static_cast<UserData*>(glfwGetWindowUserPointer(window));
        data.camera.setPerspective(glm::radians(45.0f), width / static_cast<float>(height), 0.1f, 100.0f);
        data.renderer.setCamera(data.camera);
        data.renderer.setViewport(0, 0, width, height);
    });

    renderer.enableDepthTest();

    cube.transform.position() = glm::vec3(-2.0f, 0.0f, 1.0f);

    camera.transform().position() = glm::vec3(4.0f, 3.0f, 4.0f);
    camera.transform().lookAt(glm::vec3(0.0f));

    light.transform().position() = glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f));
    light.transform().lookAt(glm::vec3(0.0f));

    renderer.setCamera(camera);
    renderer.setLight(light);

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

        torus.transform.rotation() = glm::vec3(0.0f, 0.0f, time);
        cube.transform.rotation() = glm::vec3(time2);

        renderer.clear();

        renderer.render(torus.model, torus.material, torus.transform);
        renderer.render(cube.model, cube.material, cube.transform);

        //

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::Begin(
            "Material", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize
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

        window.swapBuffers();
        window.pollEvents();
        last = now;
    }
    return 0;
}
