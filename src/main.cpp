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
#include "toto-engine/loader/image.hpp"
#include "toto-engine/utils/skybox.hpp"
#include <functional>
#include <vector>

using namespace toto;

int main(int argc, const char* argv[]) {
    int width = 800;
    int height = 600;

    auto window = Window(width, height, "Hello, World!");
    window.makeContextCurrent();
    Window::initGL();

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

    auto camera = Camera::Perspective(glm::radians(75.0f), (float)width / height, 0.1f, 100.0f);

    auto light = Light(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

    auto skybox = Skybox(generateTexture2D(
        // loadImage2Df("res/zwartkops_curve_morning_4k.hdr"),
        loadImage2Df("res/klippad_sunrise_2_4k.hdr"),
        {
            .wrap_s = GL_CLAMP_TO_EDGE,
            .wrap_t = GL_CLAMP_TO_EDGE,
            .min_filter = GL_LINEAR,
            .mag_filter = GL_LINEAR,
        }
    ));

    bool is_locked = false;
    bool imgui_has_focus = false;

    glm::vec2 camera_angles = glm::vec2(0.0f);

    EventData event_data {window, renderer, width, height, camera};
    ImGuiData imgui_data {index, width, meshes};
    event_data.setCallbacks();
    initImGui(window);

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

        // torus.transform.rotation() = glm::vec3(0.0f, 0.0f, time);
        // cube.transform.rotation() = glm::vec3(time2);

        // auto axis = event_data.getAxis() * glm::vec2(-1, 1);

        if (event_data.mouse_left_pressed && !is_locked && !ImGui::GetIO().WantCaptureMouse) {
            glfwSetInputMode(window.handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            is_locked = true;
        }
        if (event_data.escape_pressed && is_locked) {
            glfwSetInputMode(window.handle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            is_locked = false;
        }

        auto axis = event_data.getMouseDelta();

        event_data.update();

        if (is_locked) {
            camera_angles += glm::vec2(delta * axis.y, delta * axis.x);
        }

        camera.transform().position() = glm::vec3(                          //
            5.0f * glm::cos(camera_angles[1]) * glm::cos(camera_angles[0]), //
            5.0f * glm::sin(camera_angles[0]),                              //
            5.0f * glm::sin(camera_angles[1]) * glm::cos(camera_angles[0])  //
        );
        camera.lookAt(glm::vec3(0.0f));

        renderer.setViewport(0, 0, width, height);

        renderer.setCamera(camera);

        renderer.unbindGBuffer();

        renderer.clear();
        skybox.render(camera);

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
