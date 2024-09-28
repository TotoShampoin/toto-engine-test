#pragma once
#include "glpointer.hpp"
#include "toto-engine/import-gl.hpp"
#include <stdexcept>
#include <string>
#include <vector>

namespace toto {

enum class GLShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    TessControl = GL_TESS_CONTROL_SHADER,
    TessEvaluation = GL_TESS_EVALUATION_SHADER,
    Compute = GL_COMPUTE_SHADER
};

enum class GLTextureTarget {
    Texture1D = GL_TEXTURE_1D,
    Texture2D = GL_TEXTURE_2D,
    Texture3D = GL_TEXTURE_3D,
    Texture1DArray = GL_TEXTURE_1D_ARRAY,
    Texture2DArray = GL_TEXTURE_2D_ARRAY,
    TextureRectangle = GL_TEXTURE_RECTANGLE,
    TextureCubeMap = GL_TEXTURE_CUBE_MAP,
    TextureCubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
    Texture2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
    Texture2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};

template <GLShaderType SHADER_TYPE>
class GLShader : public GLPointer<
                     []() { return glCreateShader(static_cast<GLenum>(SHADER_TYPE)); },
                     [](GLuint shader) { glDeleteShader(shader); }> {
public:
    inline void source(const std::string& source) const {
        const char* c_source = source.c_str();
        glShaderSource(this->handle(), 1, &c_source, nullptr);
    }

    inline void compile() const {
        glCompileShader(this->handle());
        GLint success;
        glGetShaderiv(this->handle(), GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar info_log[512];
            glGetShaderInfoLog(this->handle(), 512, nullptr, info_log);
            throw std::runtime_error(info_log);
        }
    }
};

class GLProgram
    : public GLPointer<[]() { return glCreateProgram(); }, [](GLuint program) { glDeleteProgram(program); }> {
public:
    template <GLShaderType SHADER_TYPE>
    void attachShader(const GLShader<SHADER_TYPE>& shader) const {
        glAttachShader(this->handle(), shader.handle());
    }
    template <GLShaderType... SHADER_TYPES>
    void attachShaders(const GLShader<SHADER_TYPES>&... shaders) const {
        (attachShader(shaders), ...);
    }

    void link() const {
        glLinkProgram(this->handle());
        GLint success;
        glGetProgramiv(this->handle(), GL_LINK_STATUS, &success);
        if (!success) {
            GLchar info_log[512];
            glGetProgramInfoLog(this->handle(), 512, nullptr, info_log);
            throw std::runtime_error(info_log);
        }
    }

    static void use(const GLProgram& program) { glUseProgram(program.handle()); }
    static void unuse() { glUseProgram(0); }

    void use() const { use(*this); }
};

template <GLTextureTarget TARGET, GLsizei N = 1>
class GLTexture : public GLPointerArray<
                      [](GLsizei n, GLuint* textures) { glGenTextures(n, textures); },
                      [](GLsizei n, GLuint* textures) { glDeleteTextures(n, textures); }, N> {
public:
    inline static void bind(const GLTexture& texture) { glBindTexture(static_cast<GLenum>(TARGET), texture.handle()); }
    inline static void unbind() { glBindTexture(static_cast<GLenum>(TARGET), 0); }

    inline void bind() const { bind(*this); }

    void parameter(GLenum parameter, GLint value) const {
        bind(*this);
        glTexParameteri(static_cast<GLenum>(TARGET), parameter, value);
    }

    template <GLenum INTERNAL_FORMAT, GLenum FORMAT, GLenum TYPE>
    void image2D(GLint level, GLsizei width, GLsizei height, const void* data) const {
        bind(*this);
        glTexImage2D(static_cast<GLenum>(TARGET), level, INTERNAL_FORMAT, width, height, 0, FORMAT, TYPE, data);
    }

    template <GLenum INTERNAL_FORMAT, GLenum FORMAT, typename TYPE>
    void image2D(GLint level, GLsizei width, GLsizei height, const std::vector<TYPE> data) const {
        constexpr GLenum type = [&] {
            if constexpr (std::is_same_v<TYPE, GLubyte>) {
                return GL_UNSIGNED_BYTE;
            } else if constexpr (std::is_same_v<TYPE, GLushort>) {
                return GL_UNSIGNED_SHORT;
            } else if constexpr (std::is_same_v<TYPE, GLuint>) {
                return GL_UNSIGNED_INT;
            } else if constexpr (std::is_same_v<TYPE, GLbyte>) {
                return GL_BYTE;
            } else if constexpr (std::is_same_v<TYPE, GLshort>) {
                return GL_SHORT;
            } else if constexpr (std::is_same_v<TYPE, GLint>) {
                return GL_INT;
            } else if constexpr (std::is_same_v<TYPE, GLfloat>) {
                return GL_FLOAT;
            } else {
                static_assert(false, "Invalid type");
            }
        };
        image2D<INTERNAL_FORMAT, FORMAT, type>(level, width, height, reinterpret_cast<const void*>(data.data()));
    }
};

using GLTexture2D = GLTexture<GLTextureTarget::Texture2D>;

} // namespace toto
