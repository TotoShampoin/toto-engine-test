#include "toto-engine/mesh.hpp"
#include "toto-engine/utils/camera.hpp"
#include "toto-engine/utils/deferred_renderer.hpp"
#include "toto-engine/window.hpp"

struct Mesh {
    toto::Model model;
    toto::Material material;
    toto::Transform transform {};
};

struct EventData {
    toto::Window& window;
    toto::DeferredRenderer& renderer;
    int& width;
    int& height;
    toto::Camera& camera;
    float& camera_fov;
    bool left = false, right = false, up = false, down = false;
    bool mouse_left = false, mouse_right = false;
    bool mouse_left_pressed = false, mouse_right_pressed = false;
    bool mouse_left_released = false, mouse_right_released = false;
    bool escape_pressed = false;
    glm::vec2 mouse_position;
    glm::vec2 mouse_last_position;

    void setCallbacks();

    glm::vec2 getAxis() const { return glm::vec2(right - left, up - down); }
    glm::vec2 getMouseDelta() const { return mouse_position - mouse_last_position; }

    void update() {
        mouse_last_position = mouse_position;
        mouse_left_pressed = false;
        mouse_right_pressed = false;
        escape_pressed = false;
    }
};

struct ImGuiData {
    uint& index;
    int& width;
    std::vector<std::reference_wrapper<Mesh>>& meshes;
    float& exposure;
};

void initImGui(const toto::Window& window);

void drawImGui(ImGuiData& data, double delta);
