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

    void setCallbacks();
};

struct ImGuiData {
    uint& index;
    int& width;
    std::vector<std::reference_wrapper<Mesh>>& meshes;
};

void initImGui(const toto::Window& window);

void drawImGui(ImGuiData& data, double delta);
