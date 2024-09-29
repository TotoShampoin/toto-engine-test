#pragma once

#include "toto-engine/gl/glresources.hpp"
#include "toto-engine/loader/shader.hpp"
#include "toto-engine/mesh.hpp"
#include "toto-engine/uniform.hpp"
#include <string>
#include <unordered_map>

namespace toto {

class Renderer {
public:
    Renderer();

    void useProgram();

    void setModelMatrix(const glm::mat4& model_matrix);
    void setViewMatrix(const glm::mat4& view_matrix);
    void setProjectionMatrix(const glm::mat4& projection_matrix);

    void setMaterial(const Material& material);
    void setLight(const glm::vec3& direction, const float& intensity, const glm::vec3& color);

    void render(const Model& model);
    void render(const Mesh& mesh);

private:
    GLProgram _program = loadRenderShaderSource(pbrMaterialVertexShader(), pbrMaterialFragmentShader());
    std::unordered_map<std::string, Uniform> _uniforms;

    void _setMap(const std::string& name, const std::optional<std::reference_wrapper<GLTexture2D>>& map, uint index);
};

} // namespace toto
