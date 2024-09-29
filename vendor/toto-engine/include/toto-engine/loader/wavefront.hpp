#pragma once

#include "toto-engine/mesh.hpp"
#include <filesystem>

namespace toto {

/**
 * @warning NOT IMPLEMENTED
 */
std::vector<Mesh> loadWavefront(const std::filesystem::path& path);

} // namespace toto
