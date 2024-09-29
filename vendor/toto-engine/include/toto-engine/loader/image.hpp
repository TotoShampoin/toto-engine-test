#pragma once

#include "toto-engine/gl/glresources.hpp"
#include <filesystem>
#include <vector>

namespace toto {

template <typename TYPE>
struct Image2D {
    std::vector<TYPE> data;
    int width;
    int height;
    int channels;
};

struct TextureParameters {
    GLenum wrap_s = GL_REPEAT;
    GLenum wrap_t = GL_REPEAT;
    GLenum min_filter = GL_LINEAR;
    GLenum mag_filter = GL_LINEAR;
};

Image2D<unsigned char> loadImage2D(const std::filesystem::path& path);

Image2D<float> loadImage2Df(const std::filesystem::path& path);

template <typename TYPE>
GLTexture2D generateTexture2D(const Image2D<TYPE>& image, const TextureParameters& parameters) {
    GLTexture2D texture;
    texture.bind();
    texture.image2D(0, GL_RGBA, image.width, image.height, GL_RGBA, image.data);
    texture.parameter(GL_TEXTURE_WRAP_S, parameters.wrap_s);
    texture.parameter(GL_TEXTURE_WRAP_T, parameters.wrap_t);
    texture.parameter(GL_TEXTURE_MIN_FILTER, parameters.min_filter);
    texture.parameter(GL_TEXTURE_MAG_FILTER, parameters.mag_filter);
    return texture;
}

} // namespace toto
