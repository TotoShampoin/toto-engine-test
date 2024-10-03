#include <toto-engine/gl/globjects.hpp>
#include <toto-engine/gl/glresources.hpp>
#include <toto-engine/import-gl.hpp>
#include <toto-engine/mesh.hpp>
#include <toto-engine/utils/deferred_renderer.hpp>
#include <toto-engine/utils/light.hpp>
#include <toto-engine/utils/renderer.hpp>
#include <toto-engine/utils/shapes.hpp>
#include <toto-engine/utils/transform.hpp>
#include <toto-engine/window.hpp>

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "data.hpp"
#include <functional>
#include <vector>

using namespace toto;

int main(int argc, const char* argv[]) {
    int width = 800;
    int height = 600;

    auto window = Window(width, height, "Hello, World!");
    window.makeContextCurrent();
    Window::initGL();
    initImGui(window);

    auto renderer = DeferredRenderer(width, height);

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

    auto camera = Camera::Perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

    auto light = Light(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

    EventData event_data {window, renderer, width, height, camera};
    ImGuiData imgui_data {index, width, meshes};
    event_data.setCallbacks();

    renderer.useDeferredProgram();
    renderer.enableDepthTest();

    cube.transform.position() = glm::vec3(-2.0f, 0.0f, 1.0f);

    camera.transform().position() = glm::vec3(4.0f, 3.0f, 4.0f);
    camera.transform().lookAt(glm::vec3(0.0f));

    light.transform().position() = glm::normalize(glm::vec3(-1.0f, 2.0f, -1.0f));
    light.transform().lookAt(glm::vec3(0.0f));

    renderer.setCamera(camera);
    renderer.setLight(light);

    auto start = glfwGetTime();
    auto last = start;
    while (!window.shouldClose()) {
        auto now = glfwGetTime();
        auto time = now - start;
        auto delta = now - last;

        auto time2 = time * 0.5f;

        torus.transform.rotation() = glm::vec3(0.0f, 0.0f, time);
        cube.transform.rotation() = glm::vec3(time2);

        renderer.beginRender();
        renderer.clear();
        renderer.draw(torus.model, torus.material, torus.transform);
        renderer.draw(cube.model, cube.material, cube.transform);
        renderer.endRender();

        drawImGui(imgui_data, delta);

        window.swapBuffers();
        window.pollEvents();
        last = now;
    }
    return 0;
}
