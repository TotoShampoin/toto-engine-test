#pragma once
#include "glpointer.hpp"
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

} // namespace toto
